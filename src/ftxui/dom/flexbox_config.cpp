// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/dom/flexbox_config.hpp"

namespace ftxui {

/// @brief 設定 flexbox 方向。
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::Direction d) {
  this->direction = d;
  return *this;
}

/// @brief 設定 flexbox 換行。
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::Wrap w) {
  this->wrap = w;
  return *this;
}

/// @brief 設定 flexbox 內容對齊。
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::JustifyContent j) {
  this->justify_content = j;
  return *this;
}

/// @brief 設定 flexbox 項目對齊。
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::AlignItems a) {
  this->align_items = a;
  return *this;
}

/// @brief 設定 flexbox 內容對齊。
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::AlignContent a) {
  this->align_content = a;
  return *this;
}

/// @brief 設定 flexbox 彈性方向。
FlexboxConfig& FlexboxConfig::SetGap(int x, int y) {
  this->gap_x = x;
  this->gap_y = y;
  return *this;
}

}  // namespace ftxui
