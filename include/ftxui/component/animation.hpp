// Copyright 2022 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに従います。
#ifndef FTXUI_ANIMATION_HPP
#define FTXUI_ANIMATION_HPP

#include <chrono>      // for milliseconds, duration, steady_clock, time_point
#include <functional>  // for function

namespace ftxui::animation {
/// @brief RequestAnimationFrameは、次のアニメーションサイクルで新しいフレームが描画されるよう要求する関数です。
///
/// @note この関数は、アニメーションやトランジションなど、時間とともに状態や外観を更新する必要があるコンポーネントによって通常呼び出されます。
/// これは、変更が端末によって検出されるイベントに依存せず、時間の経過に依存する場合に役立ちます。
///
/// アニメーションが完了していないコンポーネントは、この関数を呼び出して、後で新しいフレームが描画されるよう要求できます。
///
/// 新しいイベントがなく、完了するアニメーションがない場合、新しいフレームは描画されません。
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

  /// このアニメーションステップが表す期間。
  Duration duration() const { return duration_; }

 private:
  Duration duration_;
};

namespace easing {
using Function = std::function<float(float)>;
// 線形補間（イージングなし）
float Linear(float p);

// 二次イージング; p^2
float QuadraticIn(float p);
float QuadraticOut(float p);
float QuadraticInOut(float p);

// 三次イージング; p^3
float CubicIn(float p);
float CubicOut(float p);
float CubicInOut(float p);

// 四次イージング; p^4
float QuarticIn(float p);
float QuarticOut(float p);
float QuarticInOut(float p);

// 五次イージング; p^5
float QuinticIn(float p);
float QuinticOut(float p);
float QuinticInOut(float p);

// サイン波イージング; sin(p * PI/2)
float SineIn(float p);
float SineOut(float p);
float SineInOut(float p);

// 円形イージング; sqrt(1 - p^2)
float CircularIn(float p);
float CircularOut(float p);
float CircularInOut(float p);

// 指数イージング、底2
float ExponentialIn(float p);
float ExponentialOut(float p);
float ExponentialInOut(float p);

// 指数減衰サイン波イージング
float ElasticIn(float p);
float ElasticOut(float p);
float ElasticInOut(float p);

// オーバーシュート三次イージング;
float BackIn(float p);
float BackOut(float p);
float BackInOut(float p);

// 指数減衰バウンスイージング
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