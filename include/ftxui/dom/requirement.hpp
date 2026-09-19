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

/// @brief Requirementは、ターミナルユーザーインターフェースにおけるNodeのレイアウト要件を定義する構造体です。
///
/// 要素を完全に描画するために必要な最小サイズを指定します。
/// @ingroup dom
struct FTXUI_EXPORT(DOM) Requirement {
  // 要素を完全に描画するために必要なサイズ。
  int min_x = 0;
  int min_y = 0;

  // コンポーネントにどれだけの柔軟性が与えられるか。
  int flex_grow_x = 0;
  int flex_grow_y = 0;
  int flex_shrink_x = 0;
  int flex_shrink_y = 0;

  // フレーム/フォーカス/選択要素をサポートするためのフォーカス管理。
  struct Focused {
    bool enabled = false;
    Box box;
    Node* node = nullptr;
    Screen::Cursor::Shape cursor_shape = Screen::Cursor::Shape::Hidden;

    // コンポーネントとのやり取りのための内部用。
    bool component_active = false;
    bool component_focused = false;

    // この要件が他のものより優先されるべきかどうかを返します。
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
