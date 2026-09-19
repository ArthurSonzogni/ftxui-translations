// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_ANIMATION_HPP
#define FTXUI_ANIMATION_HPP

#include <chrono>      // for milliseconds, duration, steady_clock, time_point
#include <functional>  // for function
#include "ftxui/util/export.hpp"

namespace ftxui::animation {
/// @brief RequestAnimationFrame es una función que solicita que se dibuje un nuevo fotograma en el siguiente ciclo de animación.
///
/// @note Esta función es típicamente llamada por componentes que necesitan
/// actualizar su estado o apariencia con el tiempo, como animaciones o
/// transiciones. Esto es útil cuando el cambio no depende de los
/// eventos vistos por la terminal, sino del paso del tiempo.
///
/// Los componentes que no han completado su animación pueden llamar a esta función para
/// solicitar que se dibuje un nuevo fotograma más tarde.
///
/// Cuando no hay nuevos eventos y no hay animaciones que completar, no se
/// dibuja ningún nuevo fotograma.
///
/// @ingroup component
FTXUI_EXPORT(COMPONENT) void RequestAnimationFrame();

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Duration = std::chrono::duration<float>;

// Parámetro de Component::OnAnimation(param).
class FTXUI_EXPORT(COMPONENT) Params {
 public:
  explicit Params(Duration duration) : duration_(duration) {}

  /// La duración que representa este paso de animación.
  Duration duration() const { return duration_; }

 private:
  Duration duration_;
};

namespace easing {
using Function = std::function<float(float)>;
// Interpolación lineal (sin easing)
FTXUI_EXPORT(COMPONENT) float Linear(float p);

// Easing cuadrático; p^2
FTXUI_EXPORT(COMPONENT) float QuadraticIn(float p);
FTXUI_EXPORT(COMPONENT) float QuadraticOut(float p);
FTXUI_EXPORT(COMPONENT) float QuadraticInOut(float p);

// Easing cúbico; p^3
FTXUI_EXPORT(COMPONENT) float CubicIn(float p);
FTXUI_EXPORT(COMPONENT) float CubicOut(float p);
FTXUI_EXPORT(COMPONENT) float CubicInOut(float p);

// Easing cuártico; p^4
FTXUI_EXPORT(COMPONENT) float QuarticIn(float p);
FTXUI_EXPORT(COMPONENT) float QuarticOut(float p);
FTXUI_EXPORT(COMPONENT) float QuarticInOut(float p);

// Easing quíntico; p^5
FTXUI_EXPORT(COMPONENT) float QuinticIn(float p);
FTXUI_EXPORT(COMPONENT) float QuinticOut(float p);
FTXUI_EXPORT(COMPONENT) float QuinticInOut(float p);

// Easing de onda sinusoidal; sin(p * PI/2)
FTXUI_EXPORT(COMPONENT) float SineIn(float p);
FTXUI_EXPORT(COMPONENT) float SineOut(float p);
FTXUI_EXPORT(COMPONENT) float SineInOut(float p);

// Easing circular; sqrt(1 - p^2)
FTXUI_EXPORT(COMPONENT) float CircularIn(float p);
FTXUI_EXPORT(COMPONENT) float CircularOut(float p);
FTXUI_EXPORT(COMPONENT) float CircularInOut(float p);

// Easing exponencial, base 2
FTXUI_EXPORT(COMPONENT) float ExponentialIn(float p);
FTXUI_EXPORT(COMPONENT) float ExponentialOut(float p);
FTXUI_EXPORT(COMPONENT) float ExponentialInOut(float p);

// Easing de onda sinusoidal amortiguada exponencialmente
FTXUI_EXPORT(COMPONENT) float ElasticIn(float p);
FTXUI_EXPORT(COMPONENT) float ElasticOut(float p);
FTXUI_EXPORT(COMPONENT) float ElasticInOut(float p);

// Easing cúbico con sobrepaso;
FTXUI_EXPORT(COMPONENT) float BackIn(float p);
FTXUI_EXPORT(COMPONENT) float BackOut(float p);
FTXUI_EXPORT(COMPONENT) float BackInOut(float p);

// Easing de rebote con decaimiento exponencial
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
