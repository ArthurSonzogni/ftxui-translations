// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/dom/canvas.hpp"

#include <algorithm>               // for max, min
#include <cmath>                   // for abs
#include <cstdint>                 // for uint8_t
#include <cstdlib>                 // for abs
#include <ftxui/screen/color.hpp>  // for Color
#include <functional>              // for function
#include <map>                     // for map
#include <memory>                  // for make_shared
#include <utility>                 // for move, pair
#include <vector>                  // for vector

#include "ftxui/dom/elements.hpp"     // for Element, canvas
#include "ftxui/dom/node.hpp"         // for Node
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/image.hpp"     // for Image
#include "ftxui/screen/pixel.hpp"     // for Pixel
#include "ftxui/screen/screen.hpp"    // for Pixel, Screen
#include "ftxui/screen/string.hpp"    // for Utf8ToGlyphs
#include "ftxui/util/ref.hpp"         // for ConstRef

namespace ftxui {

namespace {

// Base UTF8 pattern:
// 11100010 10100000 10000000 // empty

// Pattern for the individual dots:
// ┌──────┬───────┐
// │dot1  │ dot4  │
// ├──────┼───────┤
// │dot2  │ dot5  │
// ├──────┼───────┤
// │dot3  │ dot6  │
// ├──────┼───────┤
// │dot0-1│ dot0-2│
// └──────┴───────┘
// 11100010 10100000 10000001 // dot1
// 11100010 10100000 10000010 // dot2
// 11100010 10100000 10000100 // dot3
// 11100010 10100001 10000000 // dot0-1
// 11100010 10100000 10001000 // dot4
// 11100010 10100000 10010000 // dot5
// 11100010 10100000 10100000 // dot6
// 11100010 10100010 10000000 // dot0-2

// NOLINTNEXTLINE
uint8_t g_map_braille[2][4][2] = {
    {
        {0b00000000, 0b00000001},  // NOLINT | dot1
        {0b00000000, 0b00000010},  // NOLINT | dot2
        {0b00000000, 0b00000100},  // NOLINT | dot3
        {0b00000001, 0b00000000},  // NOLINT | dot0-1
    },
    {
        {0b00000000, 0b00001000},  // NOLINT | dot4
        {0b00000000, 0b00010000},  // NOLINT | dot5
        {0b00000000, 0b00100000},  // NOLINT | dot6
        {0b00000010, 0b00000000},  // NOLINT | dot0-2
    },
};

// NOLINTNEXTLINE
std::vector<std::string> g_map_block = {
    " ", "▘", "▖", "▌", "▝", "▀", "▞", "▛",
    "▗", "▚", "▄", "▙", "▐", "▜", "▟", "█",
};

// NOLINTNEXTLINE
const std::map<std::string, uint8_t> g_map_block_inversed = {
    {" ", 0b0000}, {"▘", 0b0001}, {"▖", 0b0010}, {"▌", 0b0011},
    {"▝", 0b0100}, {"▀", 0b0101}, {"▞", 0b0110}, {"▛", 0b0111},
    {"▗", 0b1000}, {"▚", 0b1001}, {"▄", 0b1010}, {"▙", 0b1011},
    {"▐", 0b1100}, {"▜", 0b1101}, {"▟", 0b1110}, {"█", 0b1111},
};

constexpr auto nostyle = [](Pixel& /*pixel*/) {};

}  // namespace

/// @brief コンストラクタ。
/// @param width キャンバスの幅。セルは2x4の点字ドットです。
/// @param height キャンバスの高さ。セルは2x4の点字ドットです。
Canvas::Canvas(int width, int height)
    : width_(width),
      height_(height),
      storage_(width_ * height_ / 8 /* NOLINT */) {}

/// @brief セルの内容を取得します。
/// @param x セルのx座標。
/// @param y セルのy座標。
Pixel Canvas::GetPixel(int x, int y) const {
  auto it = storage_.find(XY{x, y});
  return (it == storage_.end()) ? Pixel() : it->second.content;
}

/// @brief 点字ドットを描画します。
/// @param x ドットのx座標。
/// @param y ドットのy座標。
/// @param value ドットが塗りつぶされているかどうか。
void Canvas::DrawPoint(int x, int y, bool value) {
  DrawPoint(x, y, value, [](Pixel& /*pixel*/) {});
}

/// @brief 点字ドットを描画します。
/// @param x ドットのx座標。
/// @param y ドットのy座標。
/// @param value ドットが塗りつぶされているかどうか。
/// @param color ドットの色。
void Canvas::DrawPoint(int x, int y, bool value, const Color& color) {
  DrawPoint(x, y, value, [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 点字ドットを描画します。
/// @param x ドットのx座標。
/// @param y ドットのy座標。
/// @param value ドットが塗りつぶされているかどうか。
/// @param style セルのスタイル。
void Canvas::DrawPoint(int x, int y, bool value, const Stylizer& style) {
  Style(x, y, style);
  if (value) {
    DrawPointOn(x, y);
  } else {
    DrawPointOff(x, y);
  }
}

/// @brief 点字ドットを描画します。
/// @param x ドットのx座標。
/// @param y ドットのy座標。
void Canvas::DrawPointOn(int x, int y) {
  if (!IsIn(x, y)) {
    return;
  }
  Cell& cell = storage_[XY{x / 2, y / 4}];
  if (cell.type != CellType::kBraille) {
    cell.content.character = "⠀";  // 3 bytes.
    cell.type = CellType::kBraille;
  }

  cell.content.character[1] |= g_map_braille[x % 2][y % 4][0];  // NOLINT
  cell.content.character[2] |= g_map_braille[x % 2][y % 4][1];  // NOLINT
}

/// @brief 点字ドットを消去します。
/// @param x ドットのx座標。
/// @param y ドットのy座標。
void Canvas::DrawPointOff(int x, int y) {
  if (!IsIn(x, y)) {
    return;
  }
  Cell& cell = storage_[XY{x / 2, y / 4}];
  if (cell.type != CellType::kBraille) {
    cell.content.character = "⠀";  // 3 byt
    cell.type = CellType::kBraille;
  }

  cell.content.character[1] &= ~(g_map_braille[x % 2][y % 4][0]);  // NOLINT
  cell.content.character[2] &= ~(g_map_braille[x % 2][y % 4][1]);  // NOLINT
}

/// @brief 点字ドットを切り替えます。塗りつぶされたものは消去され、それ以外は描画されます。
/// @param x ドットのx座標。
/// @param y ドットのy座標。
void Canvas::DrawPointToggle(int x, int y) {
  if (!IsIn(x, y)) {
    return;
  }
  Cell& cell = storage_[XY{x / 2, y / 4}];
  if (cell.type != CellType::kBraille) {
    cell.content.character = "⠀";  // 3 byt
    cell.type = CellType::kBraille;
  }

  cell.content.character[1] ^= g_map_braille[x % 2][y % 4][0];  // NOLINT
  cell.content.character[2] ^= g_map_braille[x % 2][y % 4][1];  // NOLINT
}

/// @brief 点字ドットで線を描画します。
/// @param x1 最初のドットのx座標。
/// @param y1 最初のドットのy座標。
/// @param x2 2番目のドットのx座標。
/// @param y2 2番目のドットのy座標。
void Canvas::DrawPointLine(int x1, int y1, int x2, int y2) {
  DrawPointLine(x1, y1, x2, y2, [](Pixel& /*pixel*/) {});
}

/// @brief 点字ドットで線を描画します。
/// @param x1 最初のドットのx座標。
/// @param y1 最初のドットのy座標。
/// @param x2 2番目のドットのx座標。
/// @param y2 2番目のドットのy座標。
/// @param color 線の色。
void Canvas::DrawPointLine(int x1, int y1, int x2, int y2, const Color& color) {
  DrawPointLine(x1, y1, x2, y2,
                [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 点字ドットで線を描画します。
/// @param x1 最初のドットのx座標。
/// @param y1 最初のドットのy座標。
/// @param x2 2番目のドットのx座標。
/// @param y2 2番目のドットのy座標。
/// @param style 線のスタイル。
void Canvas::DrawPointLine(int x1,
                           int y1,
                           int x2,
                           int y2,
                           const Stylizer& style) {
  const int dx = std::abs(x2 - x1);
  const int dy = std::abs(y2 - y1);
  const int sx = x1 < x2 ? 1 : -1;
  const int sy = y1 < y2 ? 1 : -1;
  const int length = std::max(dx, dy);

  if (!IsIn(x1, y1) && !IsIn(x2, y2)) {
    return;
  }
  if (dx + dx > width_ * height_) {
    return;
  }

  int error = dx - dy;
  for (int i = 0; i < length; ++i) {
    DrawPoint(x1, y1, true, style);
    if (2 * error >= -dy) {
      error -= dy;
      x1 += sx;
    }
    if (2 * error <= dx) {
      error += dx;
      y1 += sy;
    }
  }
  DrawPoint(x2, y2, true, style);
}

/// @brief 点字ドットで円を描画します。
/// @param x 円の中心のx座標。
/// @param y 円の中心のy座標。
/// @param radius 円の半径。
void Canvas::DrawPointCircle(int x, int y, int radius) {
  DrawPointCircle(x, y, radius, [](Pixel& /*pixel*/) {});
}

/// @brief 点字ドットで円を描画します。
/// @param x 円の中心のx座標。
/// @param y 円の中心のy座標。
/// @param radius 円の半径。
/// @param color 円の色。
void Canvas::DrawPointCircle(int x, int y, int radius, const Color& color) {
  DrawPointCircle(x, y, radius,
                  [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 点字ドットで円を描画します。
/// @param x 円の中心のx座標。
/// @param y 円の中心のy座標。
/// @param radius 円の半径。
/// @param style 円のスタイル。
void Canvas::DrawPointCircle(int x, int y, int radius, const Stylizer& style) {
  DrawPointEllipse(x, y, radius, radius, style);
}

/// @brief 点字ドットで塗りつぶされた円を描画します。
/// @param x 円の中心のx座標。
/// @param y 円の中心のy座標。
/// @param radius 円の半径。
void Canvas::DrawPointCircleFilled(int x, int y, int radius) {
  DrawPointCircleFilled(x, y, radius, [](Pixel& /*pixel*/) {});
}

/// @brief 点字ドットで塗りつぶされた円を描画します。
/// @param x 円の中心のx座標。
/// @param y 円の中心のy座標。
/// @param radius 円の半径。
/// @param color 円の色。
void Canvas::DrawPointCircleFilled(int x,
                                   int y,
                                   int radius,
                                   const Color& color) {
  DrawPointCircleFilled(x, y, radius,
                        [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 点字ドットで塗りつぶされた円を描画します。
/// @param x 円の中心のx座標。
/// @param y 円の中心のy座標。
/// @param radius 円の半径。
/// @param style 円のスタイル。
void Canvas::DrawPointCircleFilled(int x,
                                   int y,
                                   int radius,
                                   const Stylizer& style) {
  DrawPointEllipseFilled(x, y, radius, radius, style);
}

/// @brief 点字ドットで楕円を描画します。
/// @param x 楕円の中心のx座標。
/// @param y 楕円の中心のy座標。
/// @param r1 x軸に沿った楕円の半径。
/// @param r2 y軸に沿った楕円の半径。
void Canvas::DrawPointEllipse(int x, int y, int r1, int r2) {
  DrawPointEllipse(x, y, r1, r2, [](Pixel& /*pixel*/) {});
}

/// @brief 点字ドットで楕円を描画します。
/// @param x 楕円の中心のx座標。
/// @param y 楕円の中心のy座標。
/// @param r1 x軸に沿った楕円の半径。
/// @param r2 y軸に沿った楕円の半径。
/// @param color 楕円の色。
void Canvas::DrawPointEllipse(int x,
                              int y,
                              int r1,
                              int r2,
                              const Color& color) {
  DrawPointEllipse(x, y, r1, r2,
                   [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 点字ドットで楕円を描画します。
/// @param x1 楕円の中心のx座標。
/// @param y1 楕円の中心のy座標。
/// @param r1 x軸に沿った楕円の半径。
/// @param r2 y軸に沿った楕円の半径。
/// @param s 楕円のスタイル。
void Canvas::DrawPointEllipse(int x1,
                              int y1,
                              int r1,
                              int r2,
                              const Stylizer& s) {
  int x = -r1;
  int y = 0;
  int e2 = r2;
  int dx = (1 + 2 * x) * e2 * e2;
  int dy = x * x;
  int err = dx + dy;

  do {  // NOLINT
    DrawPoint(x1 - x, y1 + y, true, s);
    DrawPoint(x1 + x, y1 + y, true, s);
    DrawPoint(x1 + x, y1 - y, true, s);
    DrawPoint(x1 - x, y1 - y, true, s);
    e2 = 2 * err;
    if (e2 >= dx) {
      x++;
      err += dx += 2 * r2 * r2;
    }
    if (e2 <= dy) {
      y++;
      err += dy += 2 * r1 * r1;
    }
  } while (x <= 0);

  while (y++ < r2) {
    DrawPoint(x1, y1 + y, true, s);
    DrawPoint(x1, y1 - y, true, s);
  }
}

/// @brief 点字ドットで塗りつぶされた楕円を描画します。
/// @param x1 楕円の中心のx座標。
/// @param y1 楕円の中心のy座標。
/// @param r1 x軸に沿った楕円の半径。
/// @param r2 y軸に沿った楕円の半径。
void Canvas::DrawPointEllipseFilled(int x1, int y1, int r1, int r2) {
  DrawPointEllipseFilled(x1, y1, r1, r2, [](Pixel& /*pixel*/) {});
}

/// @brief 点字ドットで塗りつぶされた楕円を描画します。
/// @param x1 楕円の中心のx座標。
/// @param y1 楕円の中心のy座標。
/// @param r1 x軸に沿った楕円の半径。
/// @param r2 y軸に沿った楕円の半径。
/// @param color 楕円の色。
void Canvas::DrawPointEllipseFilled(int x1,
                                    int y1,
                                    int r1,
                                    int r2,
                                    const Color& color) {
  DrawPointEllipseFilled(x1, y1, r1, r2,
                         [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 点字ドットで塗りつぶされた楕円を描画します。
/// @param x1 楕円の中心のx座標。
/// @param y1 楕円の中心のy座標。
/// @param r1 x軸に沿った楕円の半径。
/// @param r2 y軸に沿った楕円の半径。
/// @param s 楕円のスタイル。
void Canvas::DrawPointEllipseFilled(int x1,
                                    int y1,
                                    int r1,
                                    int r2,
                                    const Stylizer& s) {
  int x = -r1;
  int y = 0;
  int e2 = r2;
  int dx = (1 + 2 * x) * e2 * e2;
  int dy = x * x;
  int err = dx + dy;

  do {  // NOLINT
    for (int xx = x1 + x; xx <= x1 - x; ++xx) {
      DrawPoint(xx, y1 + y, true, s);
      DrawPoint(xx, y1 - y, true, s);
    }
    e2 = 2 * err;
    if (e2 >= dx) {
      x++;
      err += dx += 2 * r2 * r2;
    }
    if (e2 <= dy) {
      y++;
      err += dy += 2 * r1 * r1;
    }
  } while (x <= 0);

  while (y++ < r2) {
    for (int yy = y1 - y; yy <= y1 + y; ++yy) {
      DrawPoint(x1, yy, true, s);
    }
  }
}

/// @brief ブロックを描画します。
/// @param x ブロックのx座標。
/// @param y ブロックのy座標。
/// @param value ブロックが塗りつぶされているかどうか。
void Canvas::DrawBlock(int x, int y, bool value) {
  DrawBlock(x, y, value, [](Pixel& /*pixel*/) {});
}

/// @brief ブロックを描画します。
/// @param x ブロックのx座標。
/// @param y ブロックのy座標。
/// @param value ブロックが塗りつぶされているかどうか。
/// @param color ブロックの色。
void Canvas::DrawBlock(int x, int y, bool value, const Color& color) {
  DrawBlock(x, y, value, [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief ブロックを描画します。
/// @param x ブロックのx座標。
/// @param y ブロックのy座標。
/// @param value ブロックが塗りつぶされているかどうか。
/// @param style ブロックのスタイル。
void Canvas::DrawBlock(int x, int y, bool value, const Stylizer& style) {
  Style(x, y, style);
  if (value) {
    DrawBlockOn(x, y);
  } else {
    DrawBlockOff(x, y);
  }
}

/// @brief ブロックを描画します。
/// @param x ブロックのx座標。
/// @param y ブロックのy座標。
void Canvas::DrawBlockOn(int x, int y) {
  if (!IsIn(x, y)) {
    return;
  }
  y /= 2;
  Cell& cell = storage_[XY{x / 2, y / 2}];
  if (cell.type != CellType::kBlock) {
    cell.content.character = " ";
    cell.type = CellType::kBlock;
  }

  const uint8_t bit = (x % 2) * 2 + y % 2;
  uint8_t value = g_map_block_inversed.at(cell.content.character);
  value |= 1U << bit;
  cell.content.character = g_map_block[value];
}

/// @brief ブロックを消去します。
/// @param x ブロックのx座標。
/// @param y ブロックのy座標。
void Canvas::DrawBlockOff(int x, int y) {
  if (!IsIn(x, y)) {
    return;
  }
  Cell& cell = storage_[XY{x / 2, y / 4}];
  if (cell.type != CellType::kBlock) {
    cell.content.character = " ";
    cell.type = CellType::kBlock;
  }
  y /= 2;

  const uint8_t bit = (y % 2) * 2 + x % 2;
  uint8_t value = g_map_block_inversed.at(cell.content.character);
  value &= ~(1U << bit);
  cell.content.character = g_map_block[value];
}

/// @brief ブロックを切り替えます。塗りつぶされている場合は消去され、空の場合は塗りつぶされます。
/// @param x ブロックのx座標。
/// @param y ブロックのy座標。
void Canvas::DrawBlockToggle(int x, int y) {
  if (!IsIn(x, y)) {
    return;
  }
  Cell& cell = storage_[XY{x / 2, y / 4}];
  if (cell.type != CellType::kBlock) {
    cell.content.character = " ";
    cell.type = CellType::kBlock;
  }
  y /= 2;

  const uint8_t bit = (y % 2) * 2 + x % 2;
  uint8_t value = g_map_block_inversed.at(cell.content.character);
  value ^= 1U << bit;
  cell.content.character = g_map_block[value];
}

/// @brief ブロック文字で線を描画します。
/// @param x1 線の最初の点のx座標。
/// @param y1 線の最初の点のy座標。
/// @param x2 線の2番目の点のx座標。
/// @param y2 線の2番目の点のy座標。
void Canvas::DrawBlockLine(int x1, int y1, int x2, int y2) {
  DrawBlockLine(x1, y1, x2, y2, [](Pixel& /*pixel*/) {});
}

/// @brief ブロック文字で線を描画します。
/// @param x1 線の最初の点のx座標。
/// @param y1 線の最初の点のy座標。
/// @param x2 線の2番目の点のx座標。
/// @param y2 線の2番目の点のy座標。
/// @param color 線の色。
void Canvas::DrawBlockLine(int x1, int y1, int x2, int y2, const Color& color) {
  DrawBlockLine(x1, y1, x2, y2,
                [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief ブロック文字で線を描画します。
/// @param x1 線の最初の点のx座標。
/// @param y1 線の最初の点のy座標。
/// @param x2 線の2番目の点のx座標。
/// @param y2 線の2番目の点のy座標。
/// @param style 線のスタイル。
void Canvas::DrawBlockLine(int x1,
                           int y1,
                           int x2,
                           int y2,
                           const Stylizer& style) {
  y1 /= 2;
  y2 /= 2;

  const int dx = std::abs(x2 - x1);
  const int dy = std::abs(y2 - y1);
  const int sx = x1 < x2 ? 1 : -1;
  const int sy = y1 < y2 ? 1 : -1;
  const int length = std::max(dx, dy);

  if (!IsIn(x1, y1) && !IsIn(x2, y2)) {
    return;
  }
  if (dx + dx > width_ * height_) {
    return;
  }

  int error = dx - dy;
  for (int i = 0; i < length; ++i) {
    DrawBlock(x1, y1 * 2, true, style);
    if (2 * error >= -dy) {
      error -= dy;
      x1 += sx;
    }
    if (2 * error <= dx) {
      error += dx;
      y1 += sy;
    }
  }
  DrawBlock(x2, y2 * 2, true, style);
}

/// @brief ブロック文字で円を描画します。
/// @param x 円の中心のx座標。
/// @param y 円の中心のy座標。
/// @param radius 円の半径。
void Canvas::DrawBlockCircle(int x, int y, int radius) {
  DrawBlockCircle(x, y, radius, nostyle);
}

/// @brief ブロック文字で円を描画します。
/// @param x 円の中心のx座標。
/// @param y 円の中心のy座標。
/// @param radius 円の半径。
/// @param color 円の色。
void Canvas::DrawBlockCircle(int x, int y, int radius, const Color& color) {
  DrawBlockCircle(x, y, radius,
                  [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief ブロック文字で円を描画します。
/// @param x 円の中心のx座標。
/// @param y 円の中心のy座標。
/// @param radius 円の半径。
/// @param style 円のスタイル。
void Canvas::DrawBlockCircle(int x, int y, int radius, const Stylizer& style) {
  DrawBlockEllipse(x, y, radius, radius, style);
}

/// @brief ブロック文字で塗りつぶされた円を描画します。
/// @param x 円の中心のx座標。
/// @param y 円の中心のy座標。
/// @param radius 円の半径。
void Canvas::DrawBlockCircleFilled(int x, int y, int radius) {
  DrawBlockCircleFilled(x, y, radius, nostyle);
}

/// @brief ブロック文字で塗りつぶされた円を描画します。
/// @param x 円の中心のx座標。
/// @param y 円の中心のy座標。
/// @param radius 円の半径。
/// @param color 円の色。
void Canvas::DrawBlockCircleFilled(int x,
                                   int y,
                                   int radius,
                                   const Color& color) {
  DrawBlockCircleFilled(x, y, radius,
                        [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief ブロック文字で塗りつぶされた円を描画します。
/// @param x 円の中心のx座標。
/// @param y 円の中心のy座標。
/// @param radius 円の半径。
/// @param s 円のスタイル。
void Canvas::DrawBlockCircleFilled(int x,
                                   int y,
                                   int radius,
                                   const Stylizer& s) {
  DrawBlockEllipseFilled(x, y, radius, radius, s);
}

/// @brief ブロック文字で楕円を描画します。
/// @param x 楕円の中心のx座標。
/// @param y 楕円の中心のy座標。
/// @param r1 x軸に沿った楕円の半径。
/// @param r2 y軸に沿った楕円の半径。
void Canvas::DrawBlockEllipse(int x, int y, int r1, int r2) {
  DrawBlockEllipse(x, y, r1, r2, nostyle);
}

/// @brief ブロック文字で楕円を描画します。
/// @param x 楕円の中心のx座標。
/// @param y 楕円の中心のy座標。
/// @param r1 x軸に沿った楕円の半径。
/// @param r2 y軸に沿った楕円の半径。
/// @param color 楕円の色。
void Canvas::DrawBlockEllipse(int x,
                              int y,
                              int r1,
                              int r2,
                              const Color& color) {
  DrawBlockEllipse(x, y, r1, r2,
                   [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief ブロック文字で楕円を描画します。
/// @param x1 楕円の中心のx座標。
/// @param y1 楕円の中心のy座標。
/// @param r1 x軸に沿った楕円の半径。
/// @param r2 y軸に沿った楕円の半径。
/// @param s 楕円のスタイル。
void Canvas::DrawBlockEllipse(int x1,
                              int y1,
                              int r1,
                              int r2,
                              const Stylizer& s) {
  y1 /= 2;
  r2 /= 2;
  int x = -r1;
  int y = 0;
  int e2 = r2;
  int dx = (1 + 2 * x) * e2 * e2;
  int dy = x * x;
  int err = dx + dy;

  do {  // NOLINT
    DrawBlock(x1 - x, 2 * (y1 + y), true, s);
    DrawBlock(x1 + x, 2 * (y1 + y), true, s);
    DrawBlock(x1 + x, 2 * (y1 - y), true, s);
    DrawBlock(x1 - x, 2 * (y1 - y), true, s);
    e2 = 2 * err;
    if (e2 >= dx) {
      x++;
      err += dx += 2 * r2 * r2;
    }
    if (e2 <= dy) {
      y++;
      err += dy += 2 * r1 * r1;
    }
  } while (x <= 0);

  while (y++ < r2) {
    DrawBlock(x1, 2 * (y1 + y), true, s);
    DrawBlock(x1, 2 * (y1 - y), true, s);
  }
}

/// @brief ブロック文字で塗りつぶされた楕円を描画します。
/// @param x 楕円の中心のx座標。
/// @param y 楕円の中心のy座標。
/// @param r1 x軸に沿った楕円の半径。
/// @param r2 y軸に沿った楕円の半径。
void Canvas::DrawBlockEllipseFilled(int x, int y, int r1, int r2) {
  DrawBlockEllipseFilled(x, y, r1, r2, nostyle);
}

/// @brief ブロック文字で塗りつぶされた楕円を描画します。
/// @param x 楕円の中心のx座標。
/// @param y 楕円の中心のy座標。
/// @param r1 x軸に沿った楕円の半径。
/// @param r2 y軸に沿った楕円の半径。
/// @param color 楕円の色。
void Canvas::DrawBlockEllipseFilled(int x,
                                    int y,
                                    int r1,
                                    int r2,
                                    const Color& color) {
  DrawBlockEllipseFilled(x, y, r1, r2,
                         [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief ブロック文字で塗りつぶされた楕円を描画します。
/// @param x1 楕円の中心のx座標。
/// @param y1 楕円の中心のy座標。
/// @param r1 x軸に沿った楕円の半径。
/// @param r2 y軸に沿った楕円の半径。
/// @param s 楕円のスタイル。
void Canvas::DrawBlockEllipseFilled(int x1,
                                    int y1,
                                    int r1,
                                    int r2,
                                    const Stylizer& s) {
  y1 /= 2;
  r2 /= 2;
  int x = -r1;
  int y = 0;
  int e2 = r2;
  int dx = (1 + 2 * x) * e2 * e2;
  int dy = x * x;
  int err = dx + dy;

  do {  // NOLINT
    for (int xx = x1 + x; xx <= x1 - x; ++xx) {
      DrawBlock(xx, 2 * (y1 + y), true, s);
      DrawBlock(xx, 2 * (y1 - y), true, s);
    }
    e2 = 2 * err;
    if (e2 >= dx) {
      x++;
      err += dx += 2 * r2 * r2;
    }
    if (e2 <= dy) {
      y++;
      err += dy += 2 * r1 * r1;
    }
  } while (x <= 0);

  while (y++ < r2) {
    for (int yy = y1 + y; yy <= y1 - y; ++yy) {
      DrawBlock(x1, 2 * yy, true, s);
    }
  }
}

/// @brief テキストを描画します。
/// @param x テキストのx座標。
/// @param y テキストのy座標。
/// @param value 描画するテキスト。
void Canvas::DrawText(int x, int y, const std::string& value) {
  DrawText(x, y, value, nostyle);
}

/// @brief テキストを描画します。
/// @param x テキストのx座標。
/// @param y テキストのy座標。
/// @param value 描画するテキスト。
/// @param color テキストの色。
void Canvas::DrawText(int x,
                      int y,
                      const std::string& value,
                      const Color& color) {
  DrawText(x, y, value, [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief テキストを描画します。
/// @param x テキストのx座標。
/// @param y テキストのy座標。
/// @param value 描画するテキスト。
/// @param style テキストのスタイル。
void Canvas::DrawText(int x,
                      int y,
                      const std::string& value,
                      const Stylizer& style) {
  for (const auto& it : Utf8ToGlyphs(value)) {
    if (!IsIn(x, y)) {
      x += 2;
      continue;
    }
    Cell& cell = storage_[XY{x / 2, y / 4}];
    cell.type = CellType::kCell;
    cell.content.character = it;
    style(cell.content);
    x += 2;
  }
}

/// @brief 指定された座標に定義済みのピクセルを直接描画します
/// @param x ピクセルのx座標。
/// @param y ピクセルのy座標。
/// @param p 描画するピクセル。
void Canvas::DrawPixel(int x, int y, const Pixel& p) {
  Cell& cell = storage_[XY{x / 2, y / 4}];
  cell.type = CellType::kCell;
  cell.content = p;
}

/// @brief 定義済み画像を、指定された座標を左上隅として描画します
///   負の座標を指定して画像を自由に配置できます。
///   表示される部分のみが描画されます。
/// @param x 画像の左上隅に対応するx座標。
/// @param y 画像の左上隅に対応するy座標。
/// @param image 描画する画像。
void Canvas::DrawImage(int x, int y, const Image& image) {
  x /= 2;
  y /= 4;
  const int dx_begin = std::max(0, -x);
  const int dy_begin = std::max(0, -y);
  const int dx_end = std::min(image.dimx(), width_ - x);
  const int dy_end = std::min(image.dimy(), height_ - y);

  for (int dy = dy_begin; dy < dy_end; ++dy) {
    for (int dx = dx_begin; dx < dx_end; ++dx) {
      Cell& cell = storage_[XY{
          x + dx,
          y + dy,
      }];
      cell.type = CellType::kCell;
      cell.content = image.PixelAt(dx, dy);
    }
  }
}

/// @brief 指定された位置のピクセルを変更します。
/// @param style ピクセルを変更する関数。
void Canvas::Style(int x, int y, const Stylizer& style) {
  if (IsIn(x, y)) {
    style(storage_[XY{x / 2, y / 4}].content);
  }
}

namespace {

class CanvasNodeBase : public Node {
 public:
  CanvasNodeBase() = default;

  void Render(Screen& screen) override {
    const Canvas& c = canvas();
    const int y_max = std::min(c.height() / 4, box_.y_max - box_.y_min + 1);
    const int x_max = std::min(c.width() / 2, box_.x_max - box_.x_min + 1);
    for (int y = 0; y < y_max; ++y) {
      for (int x = 0; x < x_max; ++x) {
        screen.PixelAt(box_.x_min + x, box_.y_min + y) = c.GetPixel(x, y);
      }
    }
  }

  virtual const Canvas& canvas() = 0;
};

}  // namespace

/// @brief CanvasまたはCanvasへの参照から要素を生成します。
// NOLINTNEXTLINE
Element canvas(ConstRef<Canvas> canvas) {
  class Impl : public CanvasNodeBase {
   public:
    explicit Impl(ConstRef<Canvas> canvas) : canvas_(std::move(canvas)) {
      requirement_.min_x = (canvas_->width() + 1) / 2;
      requirement_.min_y = (canvas_->height() + 3) / 4;
    }
    const Canvas& canvas() final { return *canvas_; }
    ConstRef<Canvas> canvas_;
  };
  return std::make_shared<Impl>(canvas);
}

/// @brief 要求されたサイズのキャンバスを描画する要素を生成します。
/// @param width キャンバスの幅。
/// @param height キャンバスの高さ。
/// @param fn キャンバスを描画する関数。
Element canvas(int width, int height, std::function<void(Canvas&)> fn) {
  class Impl : public CanvasNodeBase {
   public:
    Impl(int width, int height, std::function<void(Canvas&)> fn)
        : width_(width), height_(height), fn_(std::move(fn)) {}

    void ComputeRequirement() final {
      requirement_.min_x = (width_ + 1) / 2;
      requirement_.min_y = (height_ + 3) / 4;
    }

    void Render(Screen& screen) final {
      const int width = (box_.x_max - box_.x_min + 1) * 2;
      const int height = (box_.y_max - box_.y_min + 1) * 4;
      canvas_ = Canvas(width, height);
      fn_(canvas_);
      CanvasNodeBase::Render(screen);
    }

    const Canvas& canvas() final { return canvas_; }
    Canvas canvas_;
    int width_;
    int height_;
    std::function<void(Canvas&)> fn_;
  };
  return std::make_shared<Impl>(width, height, std::move(fn));
}

/// @brief キャンバスを描画する要素を生成します。
/// @param fn キャンバスを描画する関数。
Element canvas(std::function<void(Canvas&)> fn) {
  const int default_dim = 12;
  return canvas(default_dim, default_dim, std::move(fn));
}

}  // namespace ftxui
