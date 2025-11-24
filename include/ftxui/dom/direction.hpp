// Copyright 2023 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、LICENSE ファイルにあるMITライセンスによって管理されています。
#ifndef FTXUI_DOM_DIRECTION_HPP
#define FTXUI_DOM_DIRECTION_HPP

namespace ftxui {

/// @brief Directionは、東西南北の4つの基本方向を表す列挙型です。
///
/// @ingroup dom
enum class Direction {
  Up = 0,
  Down = 1,
  Left = 2,
  Right = 3,
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_DOM_DIRECTION_HPP */