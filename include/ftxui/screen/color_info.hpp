// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#ifndef FTXUI_SCREEN_COLOR_INFO_HPP
#define FTXUI_SCREEN_COLOR_INFO_HPP

#include <cstdint>
#include <ftxui/screen/color.hpp>

namespace ftxui {

/// @brief ColorInfo est une structure qui contient des informations sur la palette de couleurs du terminal.
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
