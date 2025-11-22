// 版权所有 2022 Arthur Sonzogni. 保留所有权利。
// 本源代码的使用受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
#ifndef FTXUI_ANIMATION_HPP
#define FTXUI_ANIMATION_HPP

#include <chrono>      // for milliseconds, duration, steady_clock, time_point
#include <functional>  // for function

namespace ftxui::animation {
/// @brief RequestAnimationFrame 是一个请求在下一个动画周期中绘制新帧的函数。
///
/// @note 此函数通常由需要随时间更新其状态或外观的组件调用，例如动画或过渡。当变化不依赖于终端接收到的事件，而是依赖于时间的推移时，这很有用。
///
/// 尚未完成动画的组件可以调用此函数来请求稍后绘制新帧。
///
/// 当没有新事件且没有动画需要完成时，不会绘制新帧。
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

  /// 此动画步骤表示的持续时间。
  Duration duration() const { return duration_; }

 private:
  Duration duration_;
};

namespace easing {
using Function = std::function<float(float)>;
// 线性插值（无缓动）
float Linear(float p);

// 二次缓动；p^2
float QuadraticIn(float p);
float QuadraticOut(float p);
float QuadraticInOut(float p);

// 三次缓动；p^3
float CubicIn(float p);
float CubicOut(float p);
float CubicInOut(float p);

// 四次缓动；p^4
float QuarticIn(float p);
float QuarticOut(float p);
float QuarticInOut(float p);

// 五次缓动；p^5
float QuinticIn(float p);
float QuinticOut(float p);
float QuinticInOut(float p);

// 正弦波缓动；sin(p * PI/2)
float SineIn(float p);
float SineOut(float p);
float SineInOut(float p);

// 圆形缓动；sqrt(1 - p^2)
float CircularIn(float p);
float CircularOut(float p);
float CircularInOut(float p);

// 指数缓动，基数 2
float ExponentialIn(float p);
float ExponentialOut(float p);
float ExponentialInOut(float p);

// 指数衰减正弦波缓动
float ElasticIn(float p);
float ElasticOut(float p);
float ElasticInOut(float p);

// 超调三次缓动；
float BackIn(float p);
float BackOut(float p);
float BackInOut(float p);

// 指数衰减反弹缓动
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
