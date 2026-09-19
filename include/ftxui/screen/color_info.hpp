// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_COLOR_INFO_HPP
#define FTXUI_SCREEN_COLOR_INFO_HPP

#include <cstdint>
#include <vector>

#include <ftxui/screen/color.hpp>
#include <ftxui/util/export.hpp>

namespace ftxui {

/// @brief ColorInfo est une structure qui contient des informations sur la palette de couleurs du terminal.
///
/// @ingroup screen
struct FTXUI_EXPORT(SCREEN) ColorInfo {
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

FTXUI_EXPORT(SCREEN) ColorInfo GetColorInfo(Color::Palette256 index);
FTXUI_EXPORT(SCREEN) ColorInfo GetColorInfo(Color::Palette16 index);

/// @brief Obtenir les informations de couleur pour la palette256, triées en 2D.
/// @return Un vecteur 2D de ColorInfo.
/// @ingroup screen
FTXUI_EXPORT(SCREEN) std::vector<std::vector<ColorInfo>> ColorInfoSorted2D();

}  // namespace ftxui

#endif  // FTXUI_SCREEN_COLOR_INFO_HPP
