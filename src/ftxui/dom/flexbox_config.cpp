// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 本源代碼受 MIT 許可證約束，該許可證可在
// LICENSE 文件中找到。
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