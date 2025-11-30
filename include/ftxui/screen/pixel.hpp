// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_PIXEL_HPP
#define FTXUI_SCREEN_PIXEL_HPP

#include <cstdint>                 // for uint8_t
#include <string>                  // for string, basic_string, allocator
#include "ftxui/screen/color.hpp"  // for Color, Color::Default

namespace ftxui {

/// @brief Un carácter Unicode y su estilo asociado.
/// @ingroup screen
struct Pixel {
  Pixel()
      : blink(false),
        bold(false),
        dim(false),
        italic(false),
        inverted(false),
        underlined(false),
        underlined_double(false),
        strikethrough(false),
        automerge(false) {}

  // Un campo de bits que representa el estilo:
  bool blink : 1;
  bool bold : 1;
  bool dim : 1;
  bool italic : 1;
  bool inverted : 1;
  bool underlined : 1;
  bool underlined_double : 1;
  bool strikethrough : 1;
  bool automerge : 1;

  // El hipervínculo asociado al píxel.
  // 0 es el valor predeterminado, lo que significa que no hay hipervínculo.
  // Es un índice para acceder a los metadatos de la pantalla
  uint8_t hyperlink = 0;

  // Los grafemas almacenados en el píxel. Para admitir la combinación de caracteres,
  // como: a?, esto puede contener potencialmente múltiples puntos de código.
  std::string character = "";

  // Colores:
  Color background_color = Color::Default;
  Color foreground_color = Color::Default;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_PIXEL_HPP