// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
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

/// @brief Accede a un caracter en una celda en una posición dada.
/// @param x La posición de la celda a lo largo del eje x.
/// @param y La posición de la celda a lo largo del eje y.
std::string& Image::at(int x, int y) {
  return PixelAt(x, y).character;
}

/// @brief Accede a un caracter en una celda en una posición dada.
/// @param x La posición de la celda a lo largo del eje x.
/// @param y La posición de la celda a lo largo del eje y.
const std::string& Image::at(int x, int y) const {
  return PixelAt(x, y).character;
}

/// @brief Accede a una celda (Pixel) en una posición dada.
/// @param x La posición de la celda a lo largo del eje x.
/// @param y La posición de la celda a lo largo del eje y.
Pixel& Image::PixelAt(int x, int y) {
  return stencil.Contain(x, y) ? pixels_[y][x] : dev_null_pixel();
}

/// @brief Accede a una celda (Pixel) en una posición dada.
/// @param x La posición de la celda a lo largo del eje x.
/// @param y La posición de la celda a lo largo del eje y.
const Pixel& Image::PixelAt(int x, int y) const {
  return stencil.Contain(x, y) ? pixels_[y][x] : dev_null_pixel();
}

/// @brief Borra todos los píxeles de la pantalla.
void Image::Clear() {
  for (auto& line : pixels_) {
    for (auto& cell : line) {
      cell = Pixel();
    }
  }
}

}  // namespace ftxui