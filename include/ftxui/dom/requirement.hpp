// Copyright 2020 Arthur Sonzogni. All rights reserved. (日本語訳: Arthur Sonzogni. 全著作権所有。)
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file. (日本語訳: このソースコードの使用は、LICENSEファイルにあるMITライセンスに従います。)
#ifndef FTXUI_DOM_REQUIREMENT_HPP
#define FTXUI_DOM_REQUIREMENT_HPP

#include "ftxui/screen/box.hpp"
#include "ftxui/screen/screen.hpp"

namespace ftxui {
class Node;

/// @brief Requirementは、ターミナルユーザーインターフェースにおけるNodeのレイアウト要件を定義する構造体です。
///
/// 要素を完全に描画するために必要な最小サイズを指定します。
/// @ingroup dom
struct Requirement {
  // 要素を完全に描画するために必要なサイズ。
  int min_x = 0;
  int min_y = 0;

  // コンポーネントに与えられる柔軟性。
  int flex_grow_x = 0;
  int flex_grow_y = 0;
  int flex_shrink_x = 0;
  int flex_shrink_y = 0;

  // Focus management to support the frame/focus/select element.
  struct Focused {
    bool enabled = false;
    Box box;
    Node* node = nullptr;
    Screen::Cursor::Shape cursor_shape = Screen::Cursor::Shape::Hidden;

    // コンポーネントとのインタラクション用内部設定。
    bool component_active = false;

    // この要件が他の要件よりも優先されるべきかどうかを返します。
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
