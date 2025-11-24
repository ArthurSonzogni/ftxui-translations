// Copyright 2020 Arthur Sonzogni. 版權所有。
// 本原始碼受 MIT 授權條款約束，詳情請參閱
// LICENSE 檔案。
#ifndef FTXUI_SCREEN_COLOR_INFO_HPP
#define FTXUI_SCREEN_COLOR_INFO_HPP

#include <cstdint>
#include <ftxui/screen/color.hpp>

namespace ftxui {

/// @brief ColorInfo 是一個結構，包含有關終端機
/// 顏色調色盤的資訊。
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