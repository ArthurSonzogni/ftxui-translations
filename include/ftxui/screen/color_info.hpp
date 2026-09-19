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

/// @brief ColorInfo 是一個結構，包含有關終端機
/// 顏色調色盤的資訊。
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

/// @brief 取得 palette256 的顏色資訊，以 2D 方式排序。
/// @return 一個 2D 的 ColorInfo 向量。
/// @ingroup screen
FTXUI_EXPORT(SCREEN) std::vector<std::vector<ColorInfo>> ColorInfoSorted2D();

}  // namespace ftxui

#endif  // FTXUI_SCREEN_COLOR_INFO_HPP
