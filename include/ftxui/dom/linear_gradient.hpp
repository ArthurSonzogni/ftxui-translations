// 版權所有 2023 Arthur Sonzogni。保留所有權利。
// 本原始碼的使用受 MIT 許可證的約束，該許可證可在 LICENSE 文件中找到。
#ifndef FTXUI_DOM_LINEAR_GRADIENT_HPP
#define FTXUI_DOM_LINEAR_GRADIENT_HPP

#include <optional>
#include <vector>

#include "ftxui/screen/color.hpp"  // for Colors

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
struct LinearGradient {
  float angle = 0.f;

  /// 漸變停止點是漸變中特定位置的顏色。
  /// 位置是介於 0.0 和 1.0 之間的值，
  /// 其中 0.0 是漸變的開始，
  /// 1.0 是漸變的結束。
  struct Stop {
    Color color = Color::Default;
    std::optional<float> position;
  };
  std::vector<Stop> stops;

  // 簡單建構函式
  LinearGradient();
  LinearGradient(Color begin, Color end);
  LinearGradient(float angle, Color begin, Color end);

  // 使用建構器模式的修改器。
  LinearGradient& Angle(float angle);
  LinearGradient& Stop(Color color, float position);
  LinearGradient& Stop(Color color);
};

}  // namespace ftxui

#endif  // FTXUI_DOM_LINEAR_GRADIENT_HPP
