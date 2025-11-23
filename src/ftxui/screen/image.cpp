// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <sstream>  // IWYU pragma: keep
#include <string>
#include <vector>

#include "ftxui/screen/image.hpp"
#include "ftxui/screen/pixel.hpp"

namespace ftxui {

namespace {
Pixel& dev_null_pixel() {
  static Pixel pixel;
  return pixel;
}
}  // namespace

Image::Image(int dimx, int dimy)
    : stencil{0, dimx - 1, 0, dimy - 1},
      dimx_(dimx),
      dimy_(dimy),
      pixels_(dimy, std::vector<Pixel>(dimx)) {}

/// @brief Accède à un caractère dans une cellule à une position donnée.
/// @param x La position de la cellule le long de l'axe des x.
/// @param y La position de la cellule le long de l'axe des y.
std::string& Image::at(int x, int y) {
  return PixelAt(x, y).character;
}

/// @brief Accède à un caractère dans une cellule à une position donnée.
/// @param x La position de la cellule le long de l'axe des x.
/// @param y La position de la cellule le long de l'axe des y.
const std::string& Image::at(int x, int y) const {
  return PixelAt(x, y).character;
}

/// @brief Accède à une cellule (Pixel) à une position donnée.
/// @param x La position de la cellule le long de l'axe des x.
/// @param y La position de la cellule le long de l'axe des y.
Pixel& Image::PixelAt(int x, int y) {
  return stencil.Contain(x, y) ? pixels_[y][x] : dev_null_pixel();
}

/// @brief Accède à une cellule (Pixel) à une position donnée.
/// @param x La position de la cellule le long de l'axe des x.
/// @param y La position de la cellule le long de l'axe des y.
const Pixel& Image::PixelAt(int x, int y) const {
  return stencil.Contain(x, y) ? pixels_[y][x] : dev_null_pixel();
}

/// @brief Efface tous les pixels de l'écran.
void Image::Clear() {
  for (auto& line : pixels_) {
    for (auto& cell : line) {
      cell = Pixel();
    }
  }
}

}  // namespace ftxui
