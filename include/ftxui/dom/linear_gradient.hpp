// Copyright 2023 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスに従います。
#ifndef FTXUI_DOM_LINEAR_GRADIENT_HPP
#define FTXUI_DOM_LINEAR_GRADIENT_HPP

#include <optional>
#include <vector>

#include "ftxui/screen/color.hpp"  // for Colors

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
struct LinearGradient {
  float angle = 0.f;

  /// ストップは、グラデーション内の特定の位置にある色です。
  /// 位置は0.0から1.0までの値で、
  /// 0.0がグラデーションの開始、
  /// 1.0がグラデーションの終了です。
  struct Stop {
    Color color = Color::Default;
    std::optional<float> position;
  };
  std::vector<Stop> stops;

  // シンプルなコンストラクタ
  LinearGradient();
  LinearGradient(Color begin, Color end);
  LinearGradient(float angle, Color begin, Color end);

  // ビルダーパターンを使用したモディファイア。
  LinearGradient& Angle(float angle);
  LinearGradient& Stop(Color color, float position);
  LinearGradient& Stop(Color color);
};

}  // namespace ftxui

#endif  // FTXUI_DOM_LINEAR_GRADIENT_HPP