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

/// @brief 線形グラデーションカラー効果の設定を表すクラスです。
///
/// 例:
/// ```cpp
/// LinearGradient()
///    .Angle(45)
///    .Stop(Color::Red, 0.0)
///    .Stop(Color::Green, 0.5)
///    .Stop(Color::Blue, 1.0);
/// ```
///
/// 短縮コンストラクタもあります:
/// ```cpp
/// LinearGradient(Color::Red, Color::Blue);
/// LinearGradient(45, Color::Red, Color::Blue);
/// ```
///
/// @ingroup dom
struct FTXUI_EXPORT(DOM) LinearGradient {
  float angle = 0.f;

  /// ストップとは、グラデーション内の特定の位置における色です。
  /// 位置は0.0から1.0の間の値で、
  /// 0.0はグラデーションの開始、
  /// 1.0はグラデーションの終了です。
  struct Stop {
    Color color = Color::Default;
    std::optional<float> position;
  };
  std::vector<Stop> stops;

  // シンプルなコンストラクタ
  LinearGradient();
  LinearGradient(Color begin, Color end);
  LinearGradient(float angle, Color begin, Color end);

  // ビルダーパターンを使用した修飾子。
  LinearGradient& Angle(float angle);
  LinearGradient& Stop(Color color, float position);
  LinearGradient& Stop(Color color);
};

}  // namespace ftxui

#endif  // FTXUI_DOM_LINEAR_GRADIENT_HPP
