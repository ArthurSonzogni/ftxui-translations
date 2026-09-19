// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>
#include <sstream>  // IWYU pragma: keep
#include <string>
#include <vector>

#include "ftxui/screen/cell.hpp"
#include "ftxui/screen/surface.hpp"

namespace ftxui {

namespace {
Cell& dev_null_cell() {
  static Cell cell;
  return cell;
}
}  // namespace

Surface::Surface(int dimx, int dimy)
    : stencil{0, std::max(0, dimx) - 1, 0, std::max(0, dimy) - 1},
      dimx_(std::max(0, dimx)),
      dimy_(std::max(0, dimy)),
      cells_(static_cast<size_t>(dimx_) * static_cast<size_t>(dimy_)) {}

/// @brief Accède à un caractère dans une cellule à une position donnée.
/// @param x La position de la cellule sur l'axe des x.
/// @param y La position de la cellule sur l'axe des y.
std::string& Surface::at(int x, int y) {
  return CellAt(x, y).character;
}

/// @brief Accède à un caractère dans une cellule à une position donnée.
/// @param x La position de la cellule sur l'axe des x.
/// @param y La position de la cellule sur l'axe des y.
const std::string& Surface::at(int x, int y) const {
  return CellAt(x, y).character;
}

/// @brief Accède à une cellule (Cell) à une position donnée.
/// @param x La position de la cellule sur l'axe des x.
/// @param y La position de la cellule sur l'axe des y.
Cell& Surface::CellAt(int x, int y) {
  return stencil.Contain(x, y) ? FastCellAt(x, y) : dev_null_cell();
}

/// @brief Accède à une cellule (Cell) à une position donnée.
/// @param x La position de la cellule sur l'axe des x.
/// @param y La position de la cellule sur l'axe des y.
const Cell& Surface::CellAt(int x, int y) const {
  return stencil.Contain(x, y) ? FastCellAt(x, y) : dev_null_cell();
}

/// @brief Accède à une cellule (Cell) à une position donnée, sans
/// vérification du stencil.
/// @param x La position de la cellule sur l'axe des x.
/// @param y La position de la cellule sur l'axe des y.
Cell& Surface::FastCellAt(int x, int y) {
  return cells_[static_cast<size_t>(y) * static_cast<size_t>(dimx_) +
                static_cast<size_t>(x)];
}

/// @brief Accède à une cellule (Cell) à une position donnée, sans
/// vérification du stencil.
/// @param x La position de la cellule sur l'axe des x.
/// @param y La position de la cellule sur l'axe des y.
const Cell& Surface::FastCellAt(int x, int y) const {
  return cells_[static_cast<size_t>(y) * static_cast<size_t>(dimx_) +
                static_cast<size_t>(x)];
}

/// @brief Efface toutes les cellules de la surface.
void Surface::Clear() {
  std::fill(cells_.begin(), cells_.end(), Cell());
}

void Surface::Reserved1() {}
void Surface::Reserved2() {}
void Surface::Reserved3() {}
void Surface::Reserved4() {}
void Surface::Reserved5() {}
void Surface::Reserved6() {}
void Surface::Reserved7() {}
void Surface::Reserved8() {}

}  // namespace ftxui
