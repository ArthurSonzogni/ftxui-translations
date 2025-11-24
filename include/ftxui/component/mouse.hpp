// Copyright 2020 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、MITライセンスによって管理されています。
// ライセンスファイルに記載されています。
#ifndef FTXUI_COMPONENT_MOUSE_HPP
#define FTXUI_COMPONENT_MOUSE_HPP
namespace ftxui {

/// @brief マウスイベント。マウスの座標、押されたボタン、
/// および修飾子（shift, ctrl, meta）が含まれます。
/// @ingroup component
struct Mouse {
  enum Button {
    Left = 0,
    Middle = 1,
    Right = 2,
    None = 3,
    WheelUp = 4,
    WheelDown = 5,
    WheelLeft = 6,   /// 対応しているターミナルのみ。
    WheelRight = 7,  /// 対応しているターミナルのみ。
  };

  enum Motion {
    Released = 0,
    Pressed = 1,
    Moved = 2,
  };

  // ボタン
  Button button = Button::None;

  // 動作
  Motion motion = Motion::Pressed;

  // 修飾子:
  bool shift = false;
  bool meta = false;
  bool control = false;

  // 座標:
  int x = 0;
  int y = 0;
};

}  // namespace ftxui

#endif /* インクルードガードの終わり: FTXUI_COMPONENT_MOUSE_HPP */