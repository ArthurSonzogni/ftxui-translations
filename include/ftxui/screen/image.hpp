// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_IMAGE_HPP
#define FTXUI_SCREEN_IMAGE_HPP

#include <string>  // for string, basic_string, allocator
#include <vector>  // for vector

#include "ftxui/screen/box.hpp"    // for Box
#include "ftxui/screen/pixel.hpp"  // for Pixel

namespace ftxui {

/// @brief Une grille rectangulaire de pixels.
/// @ingroup screen
class Image {
 public:
  // Constructors:
  Image() = delete;
  Image(int dimx, int dimy);

  // Destructor:
  virtual ~Image() = default;

  // Accède à un caractère de la grille à une position donnée.
  std::string& at(int x, int y);
  const std::string& at(int x, int y) const;

  // Accède à une cellule (Pixel) de la grille à une position donnée.
  Pixel& PixelAt(int x, int y);
  const Pixel& PixelAt(int x, int y) const;

  // Obtenir les dimensions de l'écran.
  int dimx() const { return dimx_; }
  int dimy() const { return dimy_; }

  // Remplit l'image avec des espaces et un style par défaut.
  void Clear();

  Box stencil;

 protected:
  int dimx_;
  int dimy_;
  std::vector<std::vector<Pixel>> pixels_;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_IMAGE_HPP
