// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_ANIMATION_HPP
#define FTXUI_ANIMATION_HPP

#include <chrono>      // for milliseconds, duration, steady_clock, time_point
#include <functional>  // for function
#include "ftxui/util/export.hpp"

namespace ftxui::animation {
/// @brief RequestAnimationFrame 是一個函式，它請求在下一個動畫週期中繪製新畫面。
///
/// @note 此函式通常由需要隨時間更新其狀態或外觀的元件呼叫，例如動畫或轉場。
/// 當變更不依賴於終端機接收到的事件，而是依賴於時間的推移時，這非常有用。
///
/// 尚未完成動畫的元件可以呼叫此函式，以請求稍後繪製新畫面。
///
/// 當沒有新事件且沒有動畫要完成時，不會繪製新畫面。
///
/// @ingroup component
FTXUI_EXPORT(COMPONENT) void RequestAnimationFrame();

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Duration = std::chrono::duration<float>;

// Component::OnAnimation(param) 的參數。
class FTXUI_EXPORT(COMPONENT) Params {
 public:
  explicit Params(Duration duration) : duration_(duration) {}

  /// 此動畫步驟所代表的持續時間。
  Duration duration() const { return duration_; }

 private:
  Duration duration_;
};

namespace easing {
using Function = std::function<float(float)>;
// 線性插值（無緩動）
FTXUI_EXPORT(COMPONENT) float Linear(float p);

// 二次方緩動；p^2
FTXUI_EXPORT(COMPONENT) float QuadraticIn(float p);
FTXUI_EXPORT(COMPONENT) float QuadraticOut(float p);
FTXUI_EXPORT(COMPONENT) float QuadraticInOut(float p);

// 三次方緩動；p^3
FTXUI_EXPORT(COMPONENT) float CubicIn(float p);
FTXUI_EXPORT(COMPONENT) float CubicOut(float p);
FTXUI_EXPORT(COMPONENT) float CubicInOut(float p);

// 四次方緩動；p^4
FTXUI_EXPORT(COMPONENT) float QuarticIn(float p);
FTXUI_EXPORT(COMPONENT) float QuarticOut(float p);
FTXUI_EXPORT(COMPONENT) float QuarticInOut(float p);

// 五次方緩動；p^5
FTXUI_EXPORT(COMPONENT) float QuinticIn(float p);
FTXUI_EXPORT(COMPONENT) float QuinticOut(float p);
FTXUI_EXPORT(COMPONENT) float QuinticInOut(float p);

// 正弦波緩動；sin(p * PI/2)
FTXUI_EXPORT(COMPONENT) float SineIn(float p);
FTXUI_EXPORT(COMPONENT) float SineOut(float p);
FTXUI_EXPORT(COMPONENT) float SineInOut(float p);

// 圓形緩動；sqrt(1 - p^2)
FTXUI_EXPORT(COMPONENT) float CircularIn(float p);
FTXUI_EXPORT(COMPONENT) float CircularOut(float p);
FTXUI_EXPORT(COMPONENT) float CircularInOut(float p);

// 指數緩動，底數為 2
FTXUI_EXPORT(COMPONENT) float ExponentialIn(float p);
FTXUI_EXPORT(COMPONENT) float ExponentialOut(float p);
FTXUI_EXPORT(COMPONENT) float ExponentialInOut(float p);

// 指數衰減正弦波緩動
FTXUI_EXPORT(COMPONENT) float ElasticIn(float p);
FTXUI_EXPORT(COMPONENT) float ElasticOut(float p);
FTXUI_EXPORT(COMPONENT) float ElasticInOut(float p);

// 過衝三次方緩動；
FTXUI_EXPORT(COMPONENT) float BackIn(float p);
FTXUI_EXPORT(COMPONENT) float BackOut(float p);
FTXUI_EXPORT(COMPONENT) float BackInOut(float p);

// 指數衰減彈跳緩動
FTXUI_EXPORT(COMPONENT) float BounceIn(float p);
FTXUI_EXPORT(COMPONENT) float BounceOut(float p);
FTXUI_EXPORT(COMPONENT) float BounceInOut(float p);
}  // namespace easing

class FTXUI_EXPORT(COMPONENT) Animator {
 public:
  explicit Animator(float* from,
                    float to = 0.f,
                    Duration duration = std::chrono::milliseconds(250),
                    easing::Function easing_function = easing::Linear,
                    Duration delay = std::chrono::milliseconds(0));

  void OnAnimation(Params&);

  float to() const { return to_; }

 private:
  float* value_;
  float from_;
  float to_;
  Duration duration_;
  easing::Function easing_function_;
  Duration current_;
};

}  // namespace ftxui::animation

#endif /* end of include guard: FTXUI_ANIMATION_HPP */
