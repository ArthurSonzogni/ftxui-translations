// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/dom/canvas.hpp"

// Sous Windows, DrawText est une macro définie dans windows.h. Cela entre en conflit avec notre
// méthode Canvas::DrawText lors de la compilation en une seule unité de traduction.
#ifdef DrawText
#undef DrawText
#endif

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
#include "ftxui/screen/cell.hpp"      // for Cell
#include "ftxui/screen/screen.hpp"    // for Cell, Screen
#include "ftxui/screen/string.hpp"    // for Utf8ToGlyphs
#include "ftxui/screen/surface.hpp"   // for Surface
#include "ftxui/util/ref.hpp"         // for ConstRef

namespace ftxui {

namespace {

// Motif UTF8 de base :
// 11100010 10100000 10000000 // vide

// Motif pour les points individuels :
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

constexpr auto nostyle = [](Cell& /*pixel*/) {};

}  // namespace

/// @brief Constructeur.
/// @param width la largeur du canevas. Une cellule est un point braille 2x4.
/// @param height la hauteur du canevas. Une cellule est un point braille 2x4.
Canvas::Canvas(int width, int height)
    : width_(std::max(0, width)),
      height_(std::max(0, height)),
      storage_(static_cast<size_t>(width_) * static_cast<size_t>(height_) /
               8 /* NOLINT */) {}

/// @brief Récupère le contenu d'une cellule.
/// @param x la coordonnée x de la cellule.
/// @param y la coordonnée y de la cellule.
Cell Canvas::GetCell(int x, int y) const {
  auto it = storage_.find(XY{x, y});
  return (it == storage_.end()) ? Cell() : it->second.content;
}

/// @brief Dessine un point braille.
/// @param x la coordonnée x du point.
/// @param y la coordonnée y du point.
/// @param value indique si le point est rempli ou non.
void Canvas::DrawPoint(int x, int y, bool value) {
  DrawPoint(x, y, value, [](Cell& /*pixel*/) {});
}

/// @brief Dessine un point braille.
/// @param x la coordonnée x du point.
/// @param y la coordonnée y du point.
/// @param value indique si le point est rempli ou non.
/// @param color la couleur du point.
void Canvas::DrawPoint(int x, int y, bool value, const Color& color) {
  DrawPoint(x, y, value, [color](Cell& p) { p.foreground_color = color; });
}

/// @brief Dessine un point braille.
/// @param x la coordonnée x du point.
/// @param y la coordonnée y du point.
/// @param value indique si le point est rempli ou non.
/// @param style le style de la cellule.
void Canvas::DrawPoint(int x, int y, bool value, const Stylizer& style) {
  Style(x, y, style);
  if (value) {
    DrawPointOn(x, y);
  } else {
    DrawPointOff(x, y);
  }
}

/// @brief Dessine un point braille.
/// @param x la coordonnée x du point.
/// @param y la coordonnée y du point.
void Canvas::DrawPointOn(int x, int y) {
  if (!IsIn(x, y)) {
    return;
  }
  CanvasCell& cell = storage_[XY{x / 2, y / 4}];
  if (cell.type != CellType::kBraille) {
    cell.content.character = "⠀";  // 3 octets.
    cell.type = CellType::kBraille;
  }

  cell.content.character[1] |= g_map_braille[x % 2][y % 4][0];  // NOLINT
  cell.content.character[2] |= g_map_braille[x % 2][y % 4][1];  // NOLINT
}

/// @brief Efface un point braille.
/// @param x la coordonnée x du point.
/// @param y la coordonnée y du point.
void Canvas::DrawPointOff(int x, int y) {
  if (!IsIn(x, y)) {
    return;
  }
  CanvasCell& cell = storage_[XY{x / 2, y / 4}];
  if (cell.type != CellType::kBraille) {
    cell.content.character = "⠀";  // 3 oct
    cell.type = CellType::kBraille;
  }

  cell.content.character[1] &= ~(g_map_braille[x % 2][y % 4][0]);  // NOLINT
  cell.content.character[2] &= ~(g_map_braille[x % 2][y % 4][1]);  // NOLINT
}

/// @brief Inverse un point braille. Un point rempli sera effacé, et un point vide
/// sera dessiné.
/// @param x la coordonnée x du point.
/// @param y la coordonnée y du point.
void Canvas::DrawPointToggle(int x, int y) {
  if (!IsIn(x, y)) {
    return;
  }
  CanvasCell& cell = storage_[XY{x / 2, y / 4}];
  if (cell.type != CellType::kBraille) {
    cell.content.character = "⠀";  // 3 oct
    cell.type = CellType::kBraille;
  }

  cell.content.character[1] ^= g_map_braille[x % 2][y % 4][0];  // NOLINT
  cell.content.character[2] ^= g_map_braille[x % 2][y % 4][1];  // NOLINT
}

/// @brief Dessine une ligne de points braille.
/// @param x1 la coordonnée x du premier point.
/// @param y1 la coordonnée y du premier point.
/// @param x2 la coordonnée x du second point.
/// @param y2 la coordonnée y du second point.
void Canvas::DrawPointLine(int x1, int y1, int x2, int y2) {
  DrawPointLine(x1, y1, x2, y2, [](Cell& /*pixel*/) {});
}

/// @brief Dessine une ligne de points braille.
/// @param x1 la coordonnée x du premier point.
/// @param y1 la coordonnée y du premier point.
/// @param x2 la coordonnée x du second point.
/// @param y2 la coordonnée y du second point.
/// @param color la couleur de la ligne.
void Canvas::DrawPointLine(int x1, int y1, int x2, int y2, const Color& color) {
  DrawPointLine(x1, y1, x2, y2,
                [color](Cell& p) { p.foreground_color = color; });
}

/// @brief Dessine une ligne de points braille.
/// @param x1 la coordonnée x du premier point.
/// @param y1 la coordonnée y du premier point.
/// @param x2 la coordonnée x du second point.
/// @param y2 la coordonnée y du second point.
/// @param style le style de la ligne.
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

/// @brief Dessine un cercle de points braille.
/// @param x la coordonnée x du centre du cercle.
/// @param y la coordonnée y du centre du cercle.
/// @param radius le rayon du cercle.
void Canvas::DrawPointCircle(int x, int y, int radius) {
  DrawPointCircle(x, y, radius, [](Cell& /*pixel*/) {});
}

/// @brief Dessine un cercle de points braille.
/// @param x la coordonnée x du centre du cercle.
/// @param y la coordonnée y du centre du cercle.
/// @param radius le rayon du cercle.
/// @param color la couleur du cercle.
void Canvas::DrawPointCircle(int x, int y, int radius, const Color& color) {
  DrawPointCircle(x, y, radius,
                  [color](Cell& p) { p.foreground_color = color; });
}

/// @brief Dessine un cercle de points braille.
/// @param x la coordonnée x du centre du cercle.
/// @param y la coordonnée y du centre du cercle.
/// @param radius le rayon du cercle.
/// @param style le style du cercle.
void Canvas::DrawPointCircle(int x, int y, int radius, const Stylizer& style) {
  DrawPointEllipse(x, y, radius, radius, style);
}

/// @brief Dessine un cercle rempli de points braille.
/// @param x la coordonnée x du centre du cercle.
/// @param y la coordonnée y du centre du cercle.
/// @param radius le rayon du cercle.
void Canvas::DrawPointCircleFilled(int x, int y, int radius) {
  DrawPointCircleFilled(x, y, radius, [](Cell& /*pixel*/) {});
}

/// @brief Dessine un cercle rempli de points braille.
/// @param x la coordonnée x du centre du cercle.
/// @param y la coordonnée y du centre du cercle.
/// @param radius le rayon du cercle.
/// @param color la couleur du cercle.
void Canvas::DrawPointCircleFilled(int x,
                                   int y,
                                   int radius,
                                   const Color& color) {
  DrawPointCircleFilled(x, y, radius,
                        [color](Cell& p) { p.foreground_color = color; });
}

/// @brief Dessine un cercle rempli de points braille.
/// @param x la coordonnée x du centre du cercle.
/// @param y la coordonnée y du centre du cercle.
/// @param radius le rayon du cercle.
/// @param style le style du cercle.
void Canvas::DrawPointCircleFilled(int x,
                                   int y,
                                   int radius,
                                   const Stylizer& style) {
  DrawPointEllipseFilled(x, y, radius, radius, style);
}

/// @brief Dessine une ellipse de points braille.
/// @param x la coordonnée x du centre de l'ellipse.
/// @param y la coordonnée y du centre de l'ellipse.
/// @param r1 le rayon de l'ellipse le long de l'axe des x.
/// @param r2 le rayon de l'ellipse le long de l'axe des y.
void Canvas::DrawPointEllipse(int x, int y, int r1, int r2) {
  DrawPointEllipse(x, y, r1, r2, [](Cell& /*pixel*/) {});
}

/// @brief Dessine une ellipse de points braille.
/// @param x la coordonnée x du centre de l'ellipse.
/// @param y la coordonnée y du centre de l'ellipse.
/// @param r1 le rayon de l'ellipse le long de l'axe des x.
/// @param r2 le rayon de l'ellipse le long de l'axe des y.
/// @param color la couleur de l'ellipse.
void Canvas::DrawPointEllipse(int x,
                              int y,
                              int r1,
                              int r2,
                              const Color& color) {
  DrawPointEllipse(x, y, r1, r2,
                   [color](Cell& p) { p.foreground_color = color; });
}

/// @brief Dessine une ellipse de points braille.
/// @param x1 la coordonnée x du centre de l'ellipse.
/// @param y1 la coordonnée y du centre de l'ellipse.
/// @param r1 le rayon de l'ellipse le long de l'axe des x.
/// @param r2 le rayon de l'ellipse le long de l'axe des y.
/// @param s le style de l'ellipse.
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

/// @brief Dessine une ellipse remplie de points braille.
/// @param x1 la coordonnée x du centre de l'ellipse.
/// @param y1 la coordonnée y du centre de l'ellipse.
/// @param r1 le rayon de l'ellipse le long de l'axe des x.
/// @param r2 le rayon de l'ellipse le long de l'axe des y.
void Canvas::DrawPointEllipseFilled(int x1, int y1, int r1, int r2) {
  DrawPointEllipseFilled(x1, y1, r1, r2, [](Cell& /*pixel*/) {});
}

/// @brief Dessine une ellipse remplie de points braille.
/// @param x1 la coordonnée x du centre de l'ellipse.
/// @param y1 la coordonnée y du centre de l'ellipse.
/// @param r1 le rayon de l'ellipse le long de l'axe des x.
/// @param r2 le rayon de l'ellipse le long de l'axe des y.
/// @param color la couleur de l'ellipse.
void Canvas::DrawPointEllipseFilled(int x1,
                                    int y1,
                                    int r1,
                                    int r2,
                                    const Color& color) {
  DrawPointEllipseFilled(x1, y1, r1, r2,
                         [color](Cell& p) { p.foreground_color = color; });
}

/// @brief Dessine une ellipse remplie de points braille.
/// @param x1 la coordonnée x du centre de l'ellipse.
/// @param y1 la coordonnée y du centre de l'ellipse.
/// @param r1 le rayon de l'ellipse le long de l'axe des x.
/// @param r2 le rayon de l'ellipse le long de l'axe des y.
/// @param s le style de l'ellipse.
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

/// @brief Dessine un bloc.
/// @param x la coordonnée x du bloc.
/// @param y la coordonnée y du bloc.
/// @param value indique si le bloc est rempli ou non.
void Canvas::DrawBlock(int x, int y, bool value) {
  DrawBlock(x, y, value, [](Cell& /*pixel*/) {});
}

/// @brief Dessine un bloc.
/// @param x la coordonnée x du bloc.
/// @param y la coordonnée y du bloc.
/// @param value indique si le bloc est rempli ou non.
/// @param color la couleur du bloc.
void Canvas::DrawBlock(int x, int y, bool value, const Color& color) {
  DrawBlock(x, y, value, [color](Cell& p) { p.foreground_color = color; });
}

/// @brief Dessine un bloc.
/// @param x la coordonnée x du bloc.
/// @param y la coordonnée y du bloc.
/// @param value indique si le bloc est rempli ou non.
/// @param style le style du bloc.
void Canvas::DrawBlock(int x, int y, bool value, const Stylizer& style) {
  Style(x, y, style);
  if (value) {
    DrawBlockOn(x, y);
  } else {
    DrawBlockOff(x, y);
  }
}

/// @brief Dessine un bloc.
/// @param x la coordonnée x du bloc.
/// @param y la coordonnée y du bloc.
void Canvas::DrawBlockOn(int x, int y) {
  if (!IsIn(x, y)) {
    return;
  }
  y /= 2;
  CanvasCell& cell = storage_[XY{x / 2, y / 2}];
  if (cell.type != CellType::kBlock) {
    cell.content.character = " ";
    cell.type = CellType::kBlock;
  }

  const uint8_t bit = (x % 2) * 2 + y % 2;
  uint8_t value = g_map_block_inversed.at(cell.content.character);
  value |= 1U << bit;
  cell.content.character = g_map_block[value];
}

/// @brief Efface un bloc.
/// @param x la coordonnée x du bloc.
/// @param y la coordonnée y du bloc.
void Canvas::DrawBlockOff(int x, int y) {
  if (!IsIn(x, y)) {
    return;
  }
  CanvasCell& cell = storage_[XY{x / 2, y / 4}];
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

/// @brief Inverse un bloc. S'il est rempli, il sera effacé. S'il est vide,
/// il sera rempli.
/// @param x la coordonnée x du bloc.
/// @param y la coordonnée y du bloc.
void Canvas::DrawBlockToggle(int x, int y) {
  if (!IsIn(x, y)) {
    return;
  }
  CanvasCell& cell = storage_[XY{x / 2, y / 4}];
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

/// @brief Dessine une ligne de caractères de bloc.
/// @param x1 la coordonnée x du premier point de la ligne.
/// @param y1 la coordonnée y du premier point de la ligne.
/// @param x2 la coordonnée x du second point de la ligne.
/// @param y2 la coordonnée y du second point de la ligne.
void Canvas::DrawBlockLine(int x1, int y1, int x2, int y2) {
  DrawBlockLine(x1, y1, x2, y2, [](Cell& /*pixel*/) {});
}

/// @brief Dessine une ligne de caractères de bloc.
/// @param x1 la coordonnée x du premier point de la ligne.
/// @param y1 la coordonnée y du premier point de la ligne.
/// @param x2 la coordonnée x du second point de la ligne.
/// @param y2 la coordonnée y du second point de la ligne.
/// @param color la couleur de la ligne.
void Canvas::DrawBlockLine(int x1, int y1, int x2, int y2, const Color& color) {
  DrawBlockLine(x1, y1, x2, y2,
                [color](Cell& p) { p.foreground_color = color; });
}

/// @brief Dessine une ligne de caractères de bloc.
/// @param x1 la coordonnée x du premier point de la ligne.
/// @param y1 la coordonnée y du premier point de la ligne.
/// @param x2 la coordonnée x du second point de la ligne.
/// @param y2 la coordonnée y du second point de la ligne.
/// @param style le style de la ligne.
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

/// @brief Dessine un cercle de caractères de bloc.
/// @param x la coordonnée x du centre du cercle.
/// @param y la coordonnée y du centre du cercle.
/// @param radius le rayon du cercle.
void Canvas::DrawBlockCircle(int x, int y, int radius) {
  DrawBlockCircle(x, y, radius, nostyle);
}

/// @brief Dessine un cercle de caractères de bloc.
/// @param x la coordonnée x du centre du cercle.
/// @param y la coordonnée y du centre du cercle.
/// @param radius le rayon du cercle.
/// @param color la couleur du cercle.
void Canvas::DrawBlockCircle(int x, int y, int radius, const Color& color) {
  DrawBlockCircle(x, y, radius,
                  [color](Cell& p) { p.foreground_color = color; });
}

/// @brief Dessine un cercle de caractères de bloc.
/// @param x la coordonnée x du centre du cercle.
/// @param y la coordonnée y du centre du cercle.
/// @param radius le rayon du cercle.
/// @param style le style du cercle.
void Canvas::DrawBlockCircle(int x, int y, int radius, const Stylizer& style) {
  DrawBlockEllipse(x, y, radius, radius, style);
}

/// @brief Dessine un cercle rempli de caractères de bloc.
/// @param x la coordonnée x du centre du cercle.
/// @param y la coordonnée y du centre du cercle.
/// @param radius le rayon du cercle.
void Canvas::DrawBlockCircleFilled(int x, int y, int radius) {
  DrawBlockCircleFilled(x, y, radius, nostyle);
}

/// @brief Dessine un cercle rempli de caractères de bloc.
/// @param x la coordonnée x du centre du cercle.
/// @param y la coordonnée y du centre du cercle.
/// @param radius le rayon du cercle.
/// @param color la couleur du cercle.
void Canvas::DrawBlockCircleFilled(int x,
                                   int y,
                                   int radius,
                                   const Color& color) {
  DrawBlockCircleFilled(x, y, radius,
                        [color](Cell& p) { p.foreground_color = color; });
}

/// @brief Dessine un cercle rempli de caractères de bloc.
/// @param x la coordonnée x du centre du cercle.
/// @param y la coordonnée y du centre du cercle.
/// @param radius le rayon du cercle.
/// @param s le style du cercle.
void Canvas::DrawBlockCircleFilled(int x,
                                   int y,
                                   int radius,
                                   const Stylizer& s) {
  DrawBlockEllipseFilled(x, y, radius, radius, s);
}

/// @brief Dessine une ellipse de caractères de bloc.
/// @param x la coordonnée x du centre de l'ellipse.
/// @param y la coordonnée y du centre de l'ellipse.
/// @param r1 le rayon de l'ellipse le long de l'axe des x.
/// @param r2 le rayon de l'ellipse le long de l'axe des y.
void Canvas::DrawBlockEllipse(int x, int y, int r1, int r2) {
  DrawBlockEllipse(x, y, r1, r2, nostyle);
}

/// @brief Dessine une ellipse de caractères de bloc.
/// @param x la coordonnée x du centre de l'ellipse.
/// @param y la coordonnée y du centre de l'ellipse.
/// @param r1 le rayon de l'ellipse le long de l'axe des x.
/// @param r2 le rayon de l'ellipse le long de l'axe des y.
/// @param color la couleur de l'ellipse.
void Canvas::DrawBlockEllipse(int x,
                              int y,
                              int r1,
                              int r2,
                              const Color& color) {
  DrawBlockEllipse(x, y, r1, r2,
                   [color](Cell& p) { p.foreground_color = color; });
}

/// @brief Dessine une ellipse de caractères de bloc.
/// @param x1 la coordonnée x du centre de l'ellipse.
/// @param y1 la coordonnée y du centre de l'ellipse.
/// @param r1 le rayon de l'ellipse le long de l'axe des x.
/// @param r2 le rayon de l'ellipse le long de l'axe des y.
/// @param s le style de l'ellipse.
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

/// @brief Dessine une ellipse remplie de caractères de bloc.
/// @param x la coordonnée x du centre de l'ellipse.
/// @param y la coordonnée y du centre de l'ellipse.
/// @param r1 le rayon de l'ellipse le long de l'axe des x.
/// @param r2 le rayon de l'ellipse le long de l'axe des y.
void Canvas::DrawBlockEllipseFilled(int x, int y, int r1, int r2) {
  DrawBlockEllipseFilled(x, y, r1, r2, nostyle);
}

/// @brief Dessine une ellipse remplie de caractères de bloc.
/// @param x la coordonnée x du centre de l'ellipse.
/// @param y la coordonnée y du centre de l'ellipse.
/// @param r1 le rayon de l'ellipse le long de l'axe des x.
/// @param r2 le rayon de l'ellipse le long de l'axe des y.
/// @param color la couleur de l'ellipse.
void Canvas::DrawBlockEllipseFilled(int x,
                                    int y,
                                    int r1,
                                    int r2,
                                    const Color& color) {
  DrawBlockEllipseFilled(x, y, r1, r2,
                         [color](Cell& p) { p.foreground_color = color; });
}

/// @brief Dessine une ellipse remplie de caractères de bloc.
/// @param x1 la coordonnée x du centre de l'ellipse.
/// @param y1 la coordonnée y du centre de l'ellipse.
/// @param r1 le rayon de l'ellipse le long de l'axe des x.
/// @param r2 le rayon de l'ellipse le long de l'axe des y.
/// @param s le style de l'ellipse.
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

/// @brief Dessine un morceau de texte.
/// @param x la coordonnée x du texte.
/// @param y la coordonnée y du texte.
/// @param value le texte à dessiner.
void Canvas::DrawText(int x, int y, std::string_view value) {
  DrawText(x, y, value, nostyle);
}

/// @brief Dessine un morceau de texte.
/// @param x la coordonnée x du texte.
/// @param y la coordonnée y du texte.
/// @param value le texte à dessiner.
/// @param color la couleur du texte.
void Canvas::DrawText(int x,
                      int y,
                      std::string_view value,
                      const Color& color) {
  DrawText(x, y, value, [color](Cell& p) { p.foreground_color = color; });
}

/// @brief Dessine un morceau de texte.
/// @param x la coordonnée x du texte.
/// @param y la coordonnée y du texte.
/// @param value le texte à dessiner.
/// @param style le style du texte.
void Canvas::DrawText(int x,
                      int y,
                      std::string_view value,
                      const Stylizer& style) {
  for (const auto& it : Utf8ToGlyphs(value)) {
    if (!IsIn(x, y)) {
      x += 2;
      continue;
    }
    CanvasCell& cell = storage_[XY{x / 2, y / 4}];
    cell.type = CellType::kCell;
    cell.content.character = it;
    style(cell.content);
    x += 2;
  }
}

/// @brief Dessine directement un pixel prédéfini à la coordonnée donnée
/// @param x la coordonnée x du pixel.
/// @param y la coordonnée y du pixel.
/// @param p le pixel à dessiner.
void Canvas::DrawCell(int x, int y, const Cell& p) {
  CanvasCell& cell = storage_[XY{x / 2, y / 4}];
  cell.type = CellType::kCell;
  cell.content = p;
}

/// @brief Dessine une image prédéfinie, avec le coin supérieur gauche à la coordonnée donnée
///   Vous pouvez fournir des coordonnées négatives pour aligner l'image comme vous le souhaitez -
///   seule la partie 'visible' sera dessinée
/// @param x la coordonnée x correspondant au coin supérieur gauche de l'image.
/// @param y la coordonnée y correspondant au coin supérieur gauche de l'image.
/// @param image l'image à dessiner.
void Canvas::DrawSurface(int x, int y, const Surface& image) {
  x /= 2;
  y /= 4;
  const int dx_begin = std::max(0, -x);
  const int dy_begin = std::max(0, -y);
  const int dx_end = std::min(image.dimx(), width_ - x);
  const int dy_end = std::min(image.dimy(), height_ - y);

  for (int dy = dy_begin; dy < dy_end; ++dy) {
    for (int dx = dx_begin; dx < dx_end; ++dx) {
      CanvasCell& cell = storage_[XY{
          x + dx,
          y + dy,
      }];
      cell.type = CellType::kCell;
      cell.content = image.CellAt(dx, dy);
    }
  }
}

/// @brief Modifie un pixel à un emplacement donné.
/// @param x La coordonnée x du pixel.
/// @param y La coordonnée y du pixel.
/// @param style une fonction qui modifie le pixel.
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
        screen.CellAt(box_.x_min + x, box_.y_min + y) = c.GetCell(x, y);
      }
    }
  }

  virtual const Canvas& canvas() = 0;
};

}  // namespace

/// @brief Produit un élément à partir d'un Canevas, ou une référence à un Canevas.
// NOLINTNEXTLINE
Element canvas(ConstRef<Canvas> canvas) {
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
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
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
}

/// @brief Produit un élément dessinant un canevas de la taille demandée.
/// @param width la largeur du canevas.
/// @param height la hauteur du canevas.
/// @param fn une fonction dessinant le canevas.
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

/// @brief Produit un élément dessinant un canevas.
/// @param fn une fonction dessinant le canevas.
Element canvas(std::function<void(Canvas&)> fn) {
  const int default_dim = 12;
  return canvas(default_dim, default_dim, std::move(fn));
}

}  // namespace ftxui
