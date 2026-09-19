// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_COMPONENT_MOUSE_HPP
#define FTXUI_COMPONENT_MOUSE_HPP

#include <cstdint>

#include "ftxui/util/export.hpp"

namespace ftxui {

/// @brief マウスイベント。マウスの座標、押されたボタン、
/// および修飾子（shift, ctrl, meta）が含まれます。
/// @ingroup component
struct FTXUI_EXPORT(COMPONENT) Mouse {
  enum Button : uint8_t {
    Left = 0,
    Middle = 1,
    Right = 2,
    None = 3,
    WheelUp = 4,
    WheelDown = 5,
    WheelLeft = 6,   /// サポートされているターミナルのみ。
    WheelRight = 7,  /// サポートされているターミナルのみ。
  };

  enum Motion : uint8_t {
    Released = 0,
    Pressed = 1,
    Moved = 2,
  };

  // ボタン
  Button button = Button::None;

  // モーション
  Motion motion = Motion::Pressed;

  // 修飾キー:
  bool shift = false;
  bool meta = false;
  bool control = false;

  // 座標:
  int x = 0;
  int y = 0;
};

}  // namespace ftxui

#endif /* インクルードガードの終わり: FTXUI_COMPONENT_MOUSE_HPP */
