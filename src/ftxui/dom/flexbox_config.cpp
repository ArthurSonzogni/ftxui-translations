// 版权所有 2020 Arthur Sonzogni. 保留所有权利。
// 本源代码的使用受 MIT 许可证的约束，该许可证可在
// LICENSE 文件中找到。
#include "ftxui/dom/flexbox_config.hpp"

namespace ftxui {

/// @brief 设置 flexbox 方向。
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::Direction d) {
  this->direction = d;
  return *this;
}

/// @brief 设置 flexbox 换行。
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::Wrap w) {
  this->wrap = w;
  return *this;
}

/// @brief 设置 flexbox justify content。
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::JustifyContent j) {
  this->justify_content = j;
  return *this;
}

/// @brief 设置 flexbox align items。
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::AlignItems a) {
  this->align_items = a;
  return *this;
}

/// @brief 设置 flexbox align content。
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::AlignContent a) {
  this->align_content = a;
  return *this;
}

/// @brief 设置 flexbox flex direction。
FlexboxConfig& FlexboxConfig::SetGap(int x, int y) {
  this->gap_x = x;
  this->gap_y = y;
  return *this;
}

}  // namespace ftxui
