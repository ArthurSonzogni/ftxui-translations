// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_REQUIREMENT_HPP
#define FTXUI_DOM_REQUIREMENT_HPP

#include "ftxui/screen/box.hpp"
#include "ftxui/screen/screen.hpp"

namespace ftxui {
class Node;

/// @brief Requirement 是一个结构体，用于定义终端用户界面中 Node 的布局要求。
///
/// 它指定了完整绘制元素所需的最小尺寸，
/// @ingroup dom
struct Requirement {
  // 完整绘制元素所需的尺寸。
  int min_x = 0;
  int min_y = 0;

  // 赋予组件的灵活性。
  int flex_grow_x = 0;
  int flex_grow_y = 0;
  int flex_shrink_x = 0;
  int flex_shrink_y = 0;

  // 焦点管理以支持 frame/focus/select 元素。
  struct Focused {
    bool enabled = false;
    Box box;
    Node* node = nullptr;
    Screen::Cursor::Shape cursor_shape = Screen::Cursor::Shape::Hidden;

    // 用于与组件交互的内部变量。
    bool component_active = false;

    // 返回此要求是否应优先于另一个要求。
    bool Prefer(const Focused& other) const {
      if (!other.enabled) {
        return false;
      }
      if (!enabled) {
        return true;
      }

      return other.component_active && !component_active;
    }
  };
  Focused focused;
};

}  // namespace ftxui

#endif  // FTXUI_DOM_REQUIREMENT_HPP
