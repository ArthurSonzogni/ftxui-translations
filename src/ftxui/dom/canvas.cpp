// Copyright 2021 Arthur Sonzogni. All rights reserved.
// 本原始碼的使用受 MIT 授權約束，詳情請參閱 LICENSE 檔案。
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

/// @brief 建構函式。
/// @param width 畫布的寬度。一個單元格是 2x4 的盲文點。
/// @param height 畫布的高度。一個單元格是 2x4 的盲文點。
Canvas::Canvas(int width, int height)
    : width_(width),
      height_(height),
      storage_(width_ * height_ / 8 /* NOLINT */) {}

/// @brief 取得單元格的內容。
/// @param x 單元格的 x 座標。
/// @param y 單元格的 y 座標。
Pixel Canvas::GetPixel(int x, int y) const {
  auto it = storage_.find(XY{x, y});
  return (it == storage_.end()) ? Pixel() : it->second.content;
}

/// @brief 繪製一個盲文點。
/// @param x 點的 x 座標。
/// @param y 點的 y 座標。
/// @param value 點是否填滿。
void Canvas::DrawPoint(int x, int y, bool value) {
  DrawPoint(x, y, value, [](Pixel& /*pixel*/) {});
}

/// @brief 繪製一個盲文點。
/// @param x 點的 x 座標。
/// @param y 點的 y 座標。
/// @param value 點是否填滿。
/// @param color 點的顏色。
void Canvas::DrawPoint(int x, int y, bool value, const Color& color) {
  DrawPoint(x, y, value, [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 繪製一個盲文點。
/// @param x 點的 x 座標。
/// @param y 點的 y 座標。
/// @param value 點是否填滿。
/// @param style 單元格的樣式。
void Canvas::DrawPoint(int x, int y, bool value, const Stylizer& style) {
  Style(x, y, style);
  if (value) {
    DrawPointOn(x, y);
  } else {
    DrawPointOff(x, y);
  }
}

/// @brief 繪製一個盲文點。
/// @param x 點的 x 座標。
/// @param y 點的 y 座標。
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

/// @brief 擦除一個盲文點。
/// @param x 點的 x 座標。
/// @param y 點的 y 座標。
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

/// @brief 切換一個盲文點。如果它已填滿，將被擦除；如果為空，則繪製它。
/// @param x 點的 x 座標。
/// @param y 點的 y 座標。
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

/// @brief 繪製一條由盲文點組成的線條。
/// @param x1 第一個點的 x 座標。
/// @param y1 第一個點的 y 座標。
/// @param x2 第二個點的 x 座標。
/// @param y2 第二個點的 y 座標。
void Canvas::DrawPointLine(int x1, int y1, int x2, int y2) {
  DrawPointLine(x1, y1, x2, y2, [](Pixel& /*pixel*/) {});
}

/// @brief 繪製一條由盲文點組成的線條。
/// @param x1 第一個點的 x 座標。
/// @param y1 第一個點的 y 座標。
/// @param x2 第二個點的 x 座標。
/// @param y2 第二個點的 y 座標。
/// @param color 線條的顏色。
void Canvas::DrawPointLine(int x1, int y1, int x2, int y2, const Color& color) {
  DrawPointLine(x1, y1, x2, y2,
                [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 繪製一條由盲文點組成的線條。
/// @param x1 第一個點的 x 座標。
/// @param y1 第一個點的 y 座標。
/// @param x2 第二個點的 x 座標。
/// @param y2 第二個點的 y 座標。
/// @param style 線條的樣式。
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

/// @brief 繪製一個由盲文點組成的圓形。
/// @param x 圓心點的 x 座標。
/// @param y 圓心點的 y 座標。
/// @param radius 圓形的半徑。
void Canvas::DrawPointCircle(int x, int y, int radius) {
  DrawPointCircle(x, y, radius, [](Pixel& /*pixel*/) {});
}

/// @brief 繪製一個由盲文點組成的圓形。
/// @param x 圓心點的 x 座標。
/// @param y 圓心點的 y 座標。
/// @param radius 圓形的半徑。
/// @param color 圓形的顏色。
void Canvas::DrawPointCircle(int x, int y, int radius, const Color& color) {
  DrawPointCircle(x, y, radius,
                  [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 繪製一個由盲文點組成的圓形。
/// @param x 圓心點的 x 座標。
/// @param y 圓心點的 y 座標。
/// @param radius 圓形的半徑。
/// @param style 圓形的樣式。
void Canvas::DrawPointCircle(int x, int y, int radius, const Stylizer& style) {
  DrawPointEllipse(x, y, radius, radius, style);
}

/// @brief 繪製一個由盲文點組成的實心圓形。
/// @param x 圓心點的 x 座標。
/// @param y 圓心點的 y 座標。
/// @param radius 圓形的半徑。 {
  DrawPointCircleFilled(x, y, radius, [](Pixel& /*pixel*/) {});
}

/// @brief 繪製一個由盲文點組成的實心圓形。
/// @param x 圓心點的 x 座標。
/// @param y 圓心點的 y 座標。
/// @param radius 圓形的半徑。
/// @param color 圓形的顏色。
void Canvas::DrawPointCircleFilled(int x,
                                   int y,
                                   int radius,
                                   const Color& color) {
  DrawPointCircleFilled(x, y, radius,
                        [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 繪製一個由盲文點組成的實心圓形。
/// @param x 圓心點的 x 座標。
/// @param y 圓心點的 y 座標。
/// @param radius 圓形的半徑。
/// @param style 圓形的樣式。
void Canvas::DrawPointCircleFilled(int x,
                                   int y,
                                   int radius,
                                   const Stylizer& style) {
  DrawPointEllipseFilled(x, y, radius, radius, style);
}

/// @brief 繪製一個由盲文點組成的橢圓。
/// @param x 橢圓中心的 x 座標。
/// @param y 橢圓中心的 y 座標。
/// @param r1 沿 x 軸的橢圓半徑。
/// @param r2 沿 y 軸的橢圓半徑。
void Canvas::DrawPointEllipse(int x, int y, int r1, int r2) {
  DrawPointEllipse(x, y, r1, r2, [](Pixel& /*pixel*/) {});
}

/// @brief 繪製一個由盲文點組成的橢圓。
/// @param x 橢圓中心的 x 座標。
/// @param y 橢圓中心的 y 座標。
/// @param r1 沿 x 軸的橢圓半徑。
/// @param r2 沿 y 軸的橢圓半徑。
/// @param color 橢圓的顏色。
void Canvas::DrawPointEllipse(int x,
                              int y,
                              int r1,
                              int r2,
                              const Color& color) {
  DrawPointEllipse(x, y, r1, r2,
                   [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 繪製一個由盲文點組成的橢圓。
/// @param x1 橢圓中心的 x 座標。
/// @param y1 橢圓中心的 y 座標。
/// @param r1 沿 x 軸的橢圓半徑。
/// @param r2 沿 y 軸的橢圓半徑。
/// @param s 橢圓的樣式。
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

/// @brief 繪製一個由盲文點組成的實心橢圓。
/// @param x1 橢圓中心的 x 座標。
/// @param y1 橢圓中心的 y 座標。
/// @param r1 沿 x 軸的橢圓半徑。
/// @param r2 沿 y 軸的橢圓半徑。
void Canvas::DrawPointEllipseFilled(int x1, int y1, int r1, int r2) {
  DrawPointEllipseFilled(x1, y1, r1, r2, [](Pixel& /*pixel*/) {});
}

/// @brief 繪製一個由盲文點組成的實心橢圓。
/// @param x1 橢圓中心的 x 座標。
/// @param y1 橢圓中心的 y 座標。
/// @param r1 沿 x 軸的橢圓半徑。
/// @param r2 沿 y 軸的橢圓半徑。
/// @param color 橢圓的顏色。
void Canvas::DrawPointEllipseFilled(int x1,
                                    int y1,
                                    int r1,
                                    int r2,
                                    const Color& color) {
  DrawPointEllipseFilled(x1, y1, r1, r2,
                         [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 繪製一個由盲文點組成的實心橢圓。
/// @param x1 橢圓中心的 x 座標。
/// @param y1 橢圓中心的 y 座標。
/// @param r1 沿 x 軸的橢圓半徑。
/// @param r2 沿 y 軸的橢圓半徑。
/// @param s 橢圓的樣式。
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

/// @brief 繪製一個區塊。
/// @param x 區塊的 x 座標。
/// @param y 區塊的 y 座標。
/// @param value 區塊是否填滿。
void Canvas::DrawBlock(int x, int y, bool value) {
  DrawBlock(x, y, value, [](Pixel& /*pixel*/) {});
}

/// @brief 繪製一個區塊。
/// @param x 區塊的 x 座標。
/// @param y 區塊的 y 座標。
/// @param value 區塊是否填滿。
/// @param color 區塊的顏色。
void Canvas::DrawBlock(int x, int y, bool value, const Color& color) {
  DrawBlock(x, y, value, [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 繪製一個區塊。
/// @param x 區塊的 x 座標。
/// @param y 區塊的 y 座標。
/// @param value 區塊是否填滿。
/// @param style 區塊的樣式。
void Canvas::DrawBlock(int x, int y, bool value, const Stylizer& style) {
  Style(x, y, style);
  if (value) {
    DrawBlockOn(x, y);
  } else {
    DrawBlockOff(x, y);
  }
}

/// @brief 繪製一個區塊。
/// @param x 區塊的 x 座標。
/// @param y 區塊的 y 座標。
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

/// @brief 擦除一個區塊。
/// @param x 區塊的 x 座標。
/// @param y 區塊的 y 座標。
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

/// @brief 切換一個區塊。如果它已填滿，將被擦除；如果為空，將被填滿。
/// @param x 區塊的 x 座標。
/// @param y 區塊的 y 座標。
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

/// @brief 繪製一條由區塊字元組成的線條。
/// @param x1 線條第一個點的 x 座標。
/// @param y1 線條第一個點的 y 座標。
/// @param x2 線條第二個點的 x 座標。
/// @param y2 線條第二個點的 y 座標。
void Canvas::DrawBlockLine(int x1, int y1, int x2, int y2) {
  DrawBlockLine(x1, y1, x2, y2, [](Pixel& /*pixel*/) {});
}

/// @brief 繪製一條由區塊字元組成的線條。
/// @param x1 線條第一個點的 x 座標。
/// @param y1 線條第一個點的 y 座標。
/// @param x2 線條第二個點的 x 座標。
/// @param y2 線條第二個點的 y 座標。
/// @param color 線條的顏色。
void Canvas::DrawBlockLine(int x1, int y1, int x2, int y2, const Color& color) {
  DrawBlockLine(x1, y1, x2, y2,
                [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 繪製一條由區塊字元組成的線條。
/// @param x1 線條第一個點的 x 座標。
/// @param y1 線條第一個點的 y 座標。
/// @param x2 線條第二個點的 x 座標。
/// @param y2 線條第二個點的 y 座標。
/// @param style 線條的樣式。
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

/// @brief 繪製一個由區塊字元組成的圓形。
/// @param x 圓心點的 x 座標。
/// @param y 圓心點的 y 座標。
/// @param radius 圓形的半徑。
void Canvas::DrawBlockCircle(int x, int y, int radius) {
  DrawBlockCircle(x, y, radius, nostyle);
}

/// @brief 繪製一個由區塊字元組成的圓形。
/// @param x 圓心點的 x 座標。
/// @param y 圓心點的 y 座標。
/// @param radius 圓形的半徑。
/// @param color 圓形的顏色。
void Canvas::DrawBlockCircle(int x, int y, int radius, const Color& color) {
  DrawBlockCircle(x, y, radius,
                  [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 繪製一個由區塊字元組成的圓形。
/// @param x 圓心點的 x 座標。
/// @param y 圓心點的 y 座標。
/// @param radius 圓形的半徑。
/// @param style 圓形的樣式。
void Canvas::DrawBlockCircle(int x, int y, int radius, const Stylizer& style) {
  DrawBlockEllipse(x, y, radius, radius, style);
}

/// @brief 繪製一個由區塊字元組成的實心圓形。
/// @param x 圓心點的 x 座標。
/// @param y 圓心點的 y 座標。
/// @param radius 圓形的半徑。
void Canvas::DrawBlockCircleFilled(int x, int y, int radius) {
  DrawBlockCircleFilled(x, y, radius, nostyle);
}

/// @brief 繪製一個由區塊字元組成的實心圓形。
/// @param x 圓心點的 x 座標。
/// @param y 圓心點的 y 座標。
/// @param radius 圓形的半徑。
/// @param color 圓形的顏色。
void Canvas::DrawBlockCircleFilled(int x,
                                   int y,
                                   int radius,
                                   const Color& color) {
  DrawBlockCircleFilled(x, y, radius,
                        [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 繪製一個由區塊字元組成的實心圓形。
/// @param x 圓心點的 x 座標。
/// @param y 圓心點的 y 座標。
/// @param radius 圓形的半徑。
/// @param s 圓形的樣式。
void Canvas::DrawBlockCircleFilled(int x,
                                   int y,
                                   int radius,
                                   const Stylizer& s) {
  DrawBlockEllipseFilled(x, y, radius, radius, s);
}

/// @brief 繪製一個由區塊字元組成的橢圓。
/// @param x 橢圓中心的 x 座標。
/// @param y 橢圓中心的 y 座標。
/// @param r1 沿 x 軸的橢圓半徑。
/// @param r2 沿 y 軸的橢圓半徑。
void Canvas::DrawBlockEllipse(int x, int y, int r1, int r2) {
  DrawBlockEllipse(x, y, r1, r2, nostyle);
}

/// @brief 繪製一個由區塊字元組成的橢圓。
/// @param x 橢圓中心的 x 座標。
/// @param y 橢圓中心的 y 座標。
/// @param r1 沿 x 軸的橢圓半徑。
/// @param r2 沿 y 軸的橢圓半徑。
/// @param color 橢圓的顏色。
void Canvas::DrawBlockEllipse(int x,
                              int y,
                              int r1,
                              int r2,
                              const Color& color) {
  DrawBlockEllipse(x, y, r1, r2,
                   [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 繪製一個由區塊字元組成的橢圓。
/// @param x1 橢圓中心的 x 座標。
/// @param y1 橢圓中心的 y 座標。
/// @param r1 沿 x 軸的橢圓半徑。
/// @param r2 沿 y 軸的橢圓半徑。
/// @param s 橢圓的樣式。
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

/// @brief 繪製一個由區塊字元組成的實心橢圓。
/// @param x 橢圓中心的 x 座標。
/// @param y 橢圓中心的 y 座標。
/// @param r1 沿 x 軸的橢圓半徑。
/// @param r2 沿 y 軸的橢圓半徑。
void Canvas::DrawBlockEllipseFilled(int x, int y, int r1, int r2) {
  DrawBlockEllipseFilled(x, y, r1, r2, nostyle);
}

/// @brief 繪製一個由區塊字元組成的實心橢圓。
/// @param x 橢圓中心的 x 座標。
/// @param y 橢圓中心的 y 座標。
/// @param r1 沿 x 軸的橢圓半徑。
/// @param r2 沿 y 軸的橢圓半徑。
/// @param color 橢圓的顏色。
void Canvas::DrawBlockEllipseFilled(int x,
                                    int y,
                                    int r1,
                                    int r2,
                                    const Color& color) {
  DrawBlockEllipseFilled(x, y, r1, r2,
                         [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 繪製一個由區塊字元組成的實心橢圓。
/// @param x1 橢圓中心的 x 座標。
/// @param y1 橢圓中心的 y 座標。
/// @param r1 沿 x 軸的橢圓半徑。
/// @param r2 沿 y 軸的橢圓半徑。
/// @param s 橢圓的樣式。
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

/// @brief 繪製一段文字。
/// @param x 文字的 x 座標。
/// @param y 文字的 y 座標。
/// @param value 要繪製的文字。
void Canvas::DrawText(int x, int y, const std::string& value) {
  DrawText(x, y, value, nostyle);
}

/// @brief 繪製一段文字。
/// @param x 文字的 x 座標。
/// @param y 文字的 y 座標。
/// @param value 要繪製的文字。
/// @param color 文字的顏色。
void Canvas::DrawText(int x,
                      int y,
                      const std::string& value,
                      const Color& color) {
  DrawText(x, y, value, [color](Pixel& p) { p.foreground_color = color; });
}

/// @brief 繪製一段文字。
/// @param x 文字的 x 座標。
/// @param y 文字的 y 座標。
/// @param value 要繪製的文字。
/// @param style 文字的樣式。
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

/// @brief 直接在給定座標繪製一個預定義的像素。
/// @param x 像素的 x 座標。
/// @param y 像素的 y 座標。
/// @param p 要繪製的像素。
void Canvas::DrawPixel(int x, int y, const Pixel& p) {
  Cell& cell = storage_[XY{x / 2, y / 4}];
  cell.type = CellType::kCell;
  cell.content = p;
}

/// @brief 繪製一個預定義的圖像，左上角位於給定座標。
///   您可以提供負座標來隨意對齊圖像 - 只會繪製「可見」部分。
/// @param x 圖像左上角對應的 x 座標。
/// @param y 圖像左上角對應的 y 座標。
/// @param image 要繪製的圖像。
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

/// @brief 修改給定位置的像素。
/// @param style 修改像素的函式。
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

/// @brief 從 Canvas 或對 Canvas 的參考中產生一個元素。
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

/// @brief 從指定大小的畫布中產生一個繪製畫布的元素。
/// @param width 畫布的寬度。
/// @param height 畫布的高度。
/// @param fn 繪製畫布的函式。
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

/// @brief 從繪製畫布的函式中產生一個元素。
/// @param fn 繪製畫布的函式。
Element canvas(std::function<void(Canvas&)> fn) {
  const int default_dim = 12;
  return canvas(default_dim, default_dim, std::move(fn));
}

}  // namespace ftxui
