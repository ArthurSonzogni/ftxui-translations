// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef FTXUI_SCREEN_COLOR_INFO_HPP
#define FTXUI_SCREEN_COLOR_INFO_HPP

#include <cstdint>
#include <ftxui/screen/color.hpp>

namespace ftxui {

/// @brief ColorInfo es una estructura que contiene información sobre la paleta
/// de colores del terminal.
///
/// @ingroup screen
struct ColorInfo {
  const char* name;
  uint8_t index_256;
  uint8_t index_16;
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t hue;
  uint8_t saturation;
  uint8_t value;
};

ColorInfo GetColorInfo(Color::Palette256 index);
ColorInfo GetColorInfo(Color::Palette16 index);

}  // namespace ftxui

#endif  // FTXUI_SCREEN_COLOR_INFO_HPP