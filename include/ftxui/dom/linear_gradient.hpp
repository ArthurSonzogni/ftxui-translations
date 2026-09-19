// Copyright 2023 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_LINEAR_GRADIENT_HPP
#define FTXUI_DOM_LINEAR_GRADIENT_HPP

#include <optional>
#include <vector>

#include "ftxui/screen/color.hpp"  // for Colors
#include "ftxui/util/export.hpp"   // for FTXUI_EXPORT

namespace ftxui {

/// @brief 表示線性漸變顏色效果設定的類別。
///
/// 範例：
/// ```cpp
/// LinearGradient()
///    .Angle(45)
///    .Stop(Color::Red, 0.0)
///    .Stop(Color::Green, 0.5)
///    .Stop(Color::Blue, 1.0);
/// ```
///
/// 也有簡寫的建構函式：
/// ```cpp
/// LinearGradient(Color::Red, Color::Blue);
/// LinearGradient(45, Color::Red, Color::Blue);
/// ```
///
/// @ingroup dom
struct FTXUI_EXPORT(DOM) LinearGradient {
  float angle = 0.f;

  /// 一個 stop 是漸層中特定位置上的顏色。
  /// 該位置是介於 0.0 與 1.0 之間的值，
  /// 0.0 代表漸層的起點，
  /// 1.0 代表漸層的終點。
  struct Stop {
    Color color = Color::Default;
    std::optional<float> position;
  };
  std::vector<Stop> stops;

  // 簡單的建構子
  LinearGradient();
  LinearGradient(Color begin, Color end);
  LinearGradient(float angle, Color begin, Color end);

  // 使用建構器模式的修飾函式。
  LinearGradient& Angle(float angle);
  LinearGradient& Stop(Color color, float position);
  LinearGradient& Stop(Color color);
};

}  // namespace ftxui

#endif  // FTXUI_DOM_LINEAR_GRADIENT_HPP
