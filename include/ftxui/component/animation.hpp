// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_ANIMATION_HPP
#define FTXUI_ANIMATION_HPP

#include <chrono>      // for milliseconds, duration, steady_clock, time_point
#include <functional>  // for function

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
void RequestAnimationFrame();

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Duration = std::chrono::duration<float>;

// Parameter of Component::OnAnimation(param).
class Params {
 public:
  explicit Params(Duration duration) : duration_(duration) {}

  /// 此動畫步驟表示的持續時間。
  Duration duration() const { return duration_; }

 private:
  Duration duration_;
};

namespace easing {
using Function = std::function<float(float)>;
// 線性插值（無緩動）
float Linear(float p);

// 二次緩動；p^2
float QuadraticIn(float p);
float QuadraticOut(float p);
float QuadraticInOut(float p);

// 三次緩動；p^3
float CubicIn(float p);
float CubicOut(float p);
float CubicInOut(float p);

// 四次緩動；p^4
float QuarticIn(float p);
float QuarticOut(float p);
float QuarticInOut(float p);

// 五次緩動；p^5
float QuinticIn(float p);
float QuinticOut(float p);
float QuinticInOut(float p);

// 正弦波緩動；sin(p * PI/2)
float SineIn(float p);
float SineOut(float p);
float SineInOut(float p);

// 圓形緩動；sqrt(1 - p^2)
float CircularIn(float p);
float CircularOut(float p);
float CircularInOut(float p);

// 指數緩動，基數 2
float ExponentialIn(float p);
float ExponentialOut(float p);
float ExponentialInOut(float p);

// 指數衰減正弦波緩動
float ElasticIn(float p);
float ElasticOut(float p);
float ElasticInOut(float p);

// 超調三次緩動；
float BackIn(float p);
float BackOut(float p);
float BackInOut(float p);

// 指數衰減彈跳緩動
float BounceIn(float p);
float BounceOut(float p);
float BounceInOut(float p);
}  // namespace easing

class Animator {
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
