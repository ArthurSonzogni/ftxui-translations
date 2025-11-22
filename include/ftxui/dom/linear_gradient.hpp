// 版权所有 2023 Arthur Sonzogni. 保留所有权利。
// 此源代码受 MIT 许可协议管理，详情请查阅
// LICENSE 文件。
#ifndef FTXUI_DOM_LINEAR_GRADIENT_HPP
#define FTXUI_DOM_LINEAR_GRADIENT_HPP

#include <optional>
#include <vector>

#include "ftxui/screen/color.hpp"  // for Colors

namespace ftxui {

/// @brief 一个表示线性渐变颜色效果设置的类。
///
/// 示例：
/// ```cpp
/// LinearGradient()
///    .Angle(45)
///    .Stop(Color::Red, 0.0)
///    .Stop(Color::Green, 0.5)
///    .Stop(Color::Blue, 1.0);
/// ```
///
/// 也有简写构造函数：
/// ```cpp
/// LinearGradient(Color::Red, Color::Blue);
/// LinearGradient(45, Color::Red, Color::Blue);
/// ```
///
/// @ingroup dom
struct LinearGradient {
  float angle = 0.f;

  /// 渐变中的一个“停止点”是在渐变中特定位置的颜色。
  /// 位置是一个介于 0.0 和 1.0 之间的值，
  /// 其中 0.0 是渐变的开始，
  /// 1.0 是渐变的结束。
  struct Stop {
    Color color = Color::Default;
    std::optional<float> position;
  };
  std::vector<Stop> stops;

  // 简单构造函数

  // 使用构建器模式的修饰符。
  LinearGradient& Angle(float angle);
  LinearGradient& Stop(Color color, float position);
  LinearGradient& Stop(Color color);
};

}  // namespace ftxui

#endif  // FTXUI_DOM_LINEAR_GRADIENT_HPP
