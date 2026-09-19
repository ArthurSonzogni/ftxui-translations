// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_REQUIREMENT_HPP
#define FTXUI_DOM_REQUIREMENT_HPP

#include "ftxui/screen/box.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/util/export.hpp"

namespace ftxui {
class Node;

/// @brief Requirement 是一個結構，定義了終端使用者介面中節點的佈局要求。
///
/// 它指定了完全繪製元素所需的最小尺寸。
/// @ingroup dom
struct FTXUI_EXPORT(DOM) Requirement {
  // The required size to fully draw the element.
  int min_x = 0;
  int min_y = 0;

  // How much flexibility is given to the component.
  int flex_grow_x = 0;
  int flex_grow_y = 0;
  int flex_shrink_x = 0;
  int flex_shrink_y = 0;

  // 焦點管理，支援frame/focus/select元素。
  struct Focused {
    bool enabled = false;
    Box box;
    Node* node = nullptr;
    Screen::Cursor::Shape cursor_shape = Screen::Cursor::Shape::Hidden;

    // Internal for interactions with components.
    bool component_active = false;
    bool component_focused = false;

    // Return whether this requirement should be preferred over the other.
    bool Prefer(const Focused& other) const {
      if (!other.enabled) {
        return false;
      }
      if (!enabled) {
        return true;
      }
      if (other.component_focused != component_focused) {
        return other.component_focused;
      }

      return other.component_active && !component_active;
    }
  };
  Focused focused;
};

}  // namespace ftxui

#endif  // FTXUI_DOM_REQUIREMENT_HPP
