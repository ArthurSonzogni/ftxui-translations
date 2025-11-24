// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱
// LICENSE 文件。
#ifndef FTXUI_COMPONENT_MOUSE_HPP
#define FTXUI_COMPONENT_MOUSE_HPP
namespace ftxui {

/// @brief 滑鼠事件。它包含滑鼠的座標、按下的按鈕
/// 以及修飾鍵（shift、ctrl、meta）。
/// @ingroup component
struct Mouse {
  enum Button {
    Left = 0,
    Middle = 1,
    Right = 2,
    None = 3,
    WheelUp = 4,
    WheelDown = 5,
    WheelLeft = 6,   /// 僅支援的終端機。
    WheelRight = 7,  /// 僅支援的終端機。
  };

  enum Motion {
    Released = 0,
    Pressed = 1,
    Moved = 2,
  };

  // 按鈕
  Button button = Button::None;

  // 動作
  Motion motion = Motion::Pressed;

  // 修飾鍵：
  bool shift = false;
  bool meta = false;
  bool control = false;

  // 座標：
  int x = 0;
  int y = 0;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_MOUSE_HPP */
