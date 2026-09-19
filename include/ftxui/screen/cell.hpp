// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_CELL_HPP
#define FTXUI_SCREEN_CELL_HPP

#include <cstdint>                 // for uint8_t
#include <string>                  // for string, basic_string, allocator
#include "ftxui/screen/color.hpp"  // for Color, Color::Default
#include "ftxui/util/export.hpp"

namespace ftxui {

/// @brief Un carácter Unicode y su estilo asociado.
///
/// @note Esta estructura se llamaba anteriormente Pixel.
///
/// @ingroup screen
struct FTXUI_EXPORT(SCREEN) Cell {
  Cell()
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

  // El hipervínculo asociado a la celda.
  // 0 es el valor predeterminado, lo que significa que no hay hipervínculo.
  // Es un índice para acceder a los metadatos de Screen
  uint8_t hyperlink = 0;

  // Los grafemas almacenados en la celda. Para admitir caracteres combinados,
  // como: a?, esto puede potencialmente contener múltiples puntos de código.
  std::string character = "";

  // Colores:
  Color background_color = Color::Default;
  Color foreground_color = Color::Default;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_CELL_HPP
