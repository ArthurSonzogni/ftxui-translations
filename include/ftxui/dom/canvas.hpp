// Copyright 2021 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#ifndef FTXUI_DOM_CANVAS_HPP
#define FTXUI_DOM_CANVAS_HPP

#include <cstddef>        // for size_t
#include <functional>     // for function
#include <string>         // for string
#include <unordered_map>  // for unordered_map

#include "ftxui/screen/color.hpp"  // for Color
#include "ftxui/screen/image.hpp"  // for Pixel, Image

#ifdef DrawText
// Solution de contournement pour WinUsr.h (via Windows.h) définissant des macros qui causent des problèmes.
// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-drawtext
#undef DrawText
#endif

namespace ftxui {

/// @brief Canvas est un tampon dessinable associé aux opérations de dessin.
///
/// Canvas est une zone de dessin qui peut être utilisée pour créer des graphiques complexes. Il
/// prend en charge le dessin de points, de lignes, de cercles, d'ellipses, de texte et d'images à l'aide de
/// caractères braille, de blocs ou de caractères normaux.
///
/// Note: Un terminal contient des cellules. Une cellule est une unité de:
/// - caractères braille 2x4 (1x1 pixel)
/// - caractères de bloc 2x2 (2x2 pixels)
/// - caractères normaux 2x4 (2x4 pixels)
///
/// Vous devez multiplier la coordonnée x par 2 et la coordonnée y par 4 pour
/// obtenir la position correcte dans le terminal.
///
/// @ingroup dom
struct Canvas {
 public:
  Canvas() = default;
  Canvas(int width, int height);

  // Getters:
  int width() const { return width_; }
  int height() const { return height_; }
  Pixel GetPixel(int x, int y) const;

  using Stylizer = std::function<void(Pixel&)>;

  // Dessine en utilisant des caractères braille --------------------------------------------
  void DrawPointOn(int x, int y);
  void DrawPointOff(int x, int y);
  void DrawPointToggle(int x, int y);
  void DrawPoint(int x, int y, bool value);
  void DrawPoint(int x, int y, bool value, const Stylizer& s);
  void DrawPoint(int x, int y, bool value, const Color& color);
  void DrawPointLine(int x1, int y1, int x2, int y2);
  void DrawPointLine(int x1, int y1, int x2, int y2, const Stylizer& s);
  void DrawPointLine(int x1, int y1, int x2, int y2, const Color& color);
  void DrawPointCircle(int x, int y, int radius);
  void DrawPointCircle(int x, int y, int radius, const Stylizer& s);
  void DrawPointCircle(int x, int y, int radius, const Color& color);
  void DrawPointCircleFilled(int x, int y, int radius);
  void DrawPointCircleFilled(int x, int y, int radius, const Stylizer& s);
  void DrawPointCircleFilled(int x, int y, int radius, const Color& color);
  void DrawPointEllipse(int x, int y, int r1, int r2);
  void DrawPointEllipse(int x, int y, int r1, int r2, const Color& color);
  void DrawPointEllipse(int x, int y, int r1, int r2, const Stylizer& s);
  void DrawPointEllipseFilled(int x, int y, int r1, int r2);
  void DrawPointEllipseFilled(int x, int y, int r1, int r2, const Color& color);
  void DrawPointEllipseFilled(int x, int y, int r1, int r2, const Stylizer& s);

  // Dessine en utilisant des caractères de boîte -------------------------------------------------
  // Les blocs sont de taille 1x2. y est considéré comme un multiple de 2.
  void DrawBlockOn(int x, int y);
  void DrawBlockOff(int x, int y);
  void DrawBlockToggle(int x, int y);
  void DrawBlock(int x, int y, bool value);
  void DrawBlock(int x, int y, bool value, const Stylizer& s);
  void DrawBlock(int x, int y, bool value, const Color& color);
  void DrawBlockLine(int x1, int y1, int x2, int y2);
  void DrawBlockLine(int x1, int y1, int x2, int y2, const Stylizer& s);
  void DrawBlockLine(int x1, int y1, int x2, int y2, const Color& color);
  void DrawBlockCircle(int x1, int y1, int radius);
  void DrawBlockCircle(int x1, int y1, int radius, const Stylizer& s);
  void DrawBlockCircle(int x1, int y1, int radius, const Color& color);
  void DrawBlockCircleFilled(int x1, int y1, int radius);
  void DrawBlockCircleFilled(int x1, int y1, int radius, const Stylizer& s);
  void DrawBlockCircleFilled(int x1, int y1, int radius, const Color& color);
  void DrawBlockEllipse(int x1, int y1, int r1, int r2);
  void DrawBlockEllipse(int x1, int y1, int r1, int r2, const Stylizer& s);
  void DrawBlockEllipse(int x1, int y1, int r1, int r2, const Color& color);
  void DrawBlockEllipseFilled(int x1, int y1, int r1, int r2);
  void DrawBlockEllipseFilled(int x1,
                              int y1,
                              int r1,
                              int r2,
                              const Stylizer& s);
  void DrawBlockEllipseFilled(int x1,
                              int y1,
                              int r1,
                              int r2,
                              const Color& color);

  // Dessine en utilisant des caractères normaux ----------------------------------------------
  // Dessine en utilisant un caractère de taille 2x4 à la position (x,y)
  // x est considéré comme un multiple de 2.
  // y est considéré comme un multiple de 4.
  void DrawText(int x, int y, const std::string& value);
  void DrawText(int x, int y, const std::string& value, const Color& color);
  void DrawText(int x, int y, const std::string& value, const Stylizer& style);

  // Dessine directement des pixels ou des images --------------------------------------
  // x est considéré comme un multiple de 2.
  // y est considéré comme un multiple de 4.
  void DrawPixel(int x, int y, const Pixel&);
  void DrawImage(int x, int y, const Image&);

  // Décorateur:
  // x est considéré comme un multiple de 2.
  // y est considéré comme un multiple de 4.
  void Style(int x, int y, const Stylizer& style);

 private:
  bool IsIn(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
  }

  enum CellType {
    kCell,     // Units of size 2x4
    kBlock,    // Units of size 2x2
    kBraille,  // Units of size 1x1
  };

  struct Cell {
    CellType type = kCell;
    Pixel content;
  };

  struct XY {
    int x;
    int y;
    bool operator==(const XY& other) const {
      return x == other.x && y == other.y;
    }
  };

  struct XYHash {
    size_t operator()(const XY& xy) const {
      constexpr size_t shift = 1024;
      return size_t(xy.x) * shift + size_t(xy.y);
    }
  };

  int width_ = 0;
  int height_ = 0;
  std::unordered_map<XY, Cell, XYHash> storage_;
};

}  // namespace ftxui

#endif  // FTXUI_DOM_CANVAS_HPP