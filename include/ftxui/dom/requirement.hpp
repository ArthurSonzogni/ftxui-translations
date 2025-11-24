// Copyright 2020 Arthur Sonzogni. All rights reserved.
// 本原始碼受 MIT 授權條款保護，詳情請參閱 LICENSE 檔案。
#ifndef FTXUI_DOM_REQUIREMENT_HPP
#define FTXUI_DOM_REQUIREMENT_HPP

#include "ftxui/screen/box.hpp"
#include "ftxui/screen/screen.hpp"

namespace ftxui {
class Node;

/// @brief Requirement 是一個結構，定義了終端使用者介面中節點的佈局要求。
///
/// 它指定了完全繪製元素所需的最小尺寸。
/// @ingroup dom
struct Requirement {
  // 完全繪製元素所需的尺寸。
  int min_x = 0;
  int min_y = 0;

  // 元件的彈性程度。
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

    // 內部用於與元件互動。
    bool component_active = false;

    // 返回此需求是否應優先於另一個。
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
