// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_SURFACE_HPP
#define FTXUI_SCREEN_SURFACE_HPP

#include <string>  // for string, basic_string, allocator
#include <vector>  // for vector

#include "ftxui/screen/box.hpp"   // for Box
#include "ftxui/screen/cell.hpp"  // for Cell
#include "ftxui/util/export.hpp"  // for FTXUI_EXPORT

namespace ftxui {

/// @brief Une grille rectangulaire de Cell.
///
/// @note Cette classe s'appelait précédemment Image.
///
/// @ingroup screen
class FTXUI_EXPORT(SCREEN) Surface {
 public:
  // Constructeurs :
  Surface() = delete;
  Surface(int dimx, int dimy);

  // Destructeur :
  virtual ~Surface() = default;

  // Copie :
  Surface(const Surface&) = default;
  Surface& operator=(const Surface&) = default;

  // Accède à un caractère de la grille à une position donnée.
  std::string& at(int x, int y);
  const std::string& at(int x, int y) const;

  // Accède à une cellule (Cell) de la grille à une position donnée.
  Cell& CellAt(int x, int y);
  const Cell& CellAt(int x, int y) const;

  // [Obsolète] alias de CellAt.
  Cell& PixelAt(int x, int y) { return CellAt(x, y); }
  const Cell& PixelAt(int x, int y) const { return CellAt(x, y); }

  // Récupère les dimensions de l'écran.
  int dimx() const { return dimx_; }
  int dimy() const { return dimy_; }

  // Remplit la surface avec des espaces et le style par défaut.
  void Clear();

  // Réserve ABI :
  virtual void Reserved1();
  virtual void Reserved2();
  virtual void Reserved3();
  virtual void Reserved4();
  virtual void Reserved5();
  virtual void Reserved6();
  virtual void Reserved7();
  virtual void Reserved8();

  Box stencil;

 protected:
  Cell& FastCellAt(int x, int y);
  const Cell& FastCellAt(int x, int y) const;

  int dimx_;
  int dimy_;
  std::vector<Cell> cells_;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_SURFACE_HPP
