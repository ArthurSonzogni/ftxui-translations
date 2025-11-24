// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードは、LICENSEファイルにあるMITライセンスに基づいて管理されています。
#include "ftxui/dom/flexbox_config.hpp"

namespace ftxui {

/// @brief フレックスボックスの方向を設定します。
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::Direction d) {
  this->direction = d;
  return *this;
}

/// @brief フレックスボックスの折り返しを設定します。
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::Wrap w) {
  this->wrap = w;
  return *this;
}

/// @brief フレックスボックスの justify content を設定します。
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::JustifyContent j) {
  this->justify_content = j;
  return *this;
}

/// @brief フレックスボックスの align items を設定します。
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::AlignItems a) {
  this->align_items = a;
  return *this;
}

/// @brief フレックスボックスの align content を設定します。
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::AlignContent a) {
  this->align_content = a;
  return *this;
}

/// @brief フレックスボックスのギャップを設定します。
FlexboxConfig& FlexboxConfig::SetGap(int x, int y) {
  this->gap_x = x;
  this->gap_y = y;
  return *this;
}

}  // namespace ftxui
