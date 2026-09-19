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

/// @brief Una cuadrícula rectangular de Cell.
///
/// @note Esta clase se llamaba anteriormente Image.
///
/// @ingroup screen
class FTXUI_EXPORT(SCREEN) Surface {
 public:
  // Constructores:
  Surface() = delete;
  Surface(int dimx, int dimy);

  // Destructor:
  virtual ~Surface() = default;

  // Copia:
  Surface(const Surface&) = default;
  Surface& operator=(const Surface&) = default;

  // Accede a un carácter en la cuadrícula en una posición dada.
  std::string& at(int x, int y);
  const std::string& at(int x, int y) const;

  // Accede a una celda (Cell) en la cuadrícula en una posición dada.
  Cell& CellAt(int x, int y);
  const Cell& CellAt(int x, int y) const;

  // [Obsoleto] alias de CellAt.
  Cell& PixelAt(int x, int y) { return CellAt(x, y); }
  const Cell& PixelAt(int x, int y) const { return CellAt(x, y); }

  // Obtiene las dimensiones de la pantalla.
  int dimx() const { return dimx_; }
  int dimy() const { return dimy_; }

  // Rellena la superficie con espacio y estilo predeterminado
  void Clear();

  // Reserva ABI:
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
