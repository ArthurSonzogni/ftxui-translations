// Copyright 2022 Arthur Sonzogni. All rights reserved.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef FTXUI_ANIMATION_HPP
#define FTXUI_ANIMATION_HPP

#include <chrono>      // for milliseconds, duration, steady_clock, time_point
#include <functional>  // for function

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
void RequestAnimationFrame();

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Duration = std::chrono::duration<float>;

// Parámetro de Component::OnAnimation(param).
class Params {
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
float Linear(float p);

// Easing cuadrático; p^2
float QuadraticIn(float p);
float QuadraticOut(float p);
float QuadraticInOut(float p);

// Easing cúbico; p^3
float CubicIn(float p);
float CubicOut(float p);
float CubicInOut(float p);

// Easing cuártico; p^4
float QuarticIn(float p);
float QuarticOut(float p);
float QuarticInOut(float p);

// Easing quíntico; p^5
float QuinticIn(float p);
float QuinticOut(float p);
float QuinticInOut(float p);

// Easing de onda sinusoidal; sin(p * PI/2)
float SineIn(float p);
float SineOut(float p);
float SineInOut(float p);

// Easing circular; sqrt(1 - p^2)
float CircularIn(float p);
float CircularOut(float p);
float CircularInOut(float p);

// Easing exponencial, base 2
float ExponentialIn(float p);
float ExponentialOut(float p);
float ExponentialInOut(float p);

// Easing de onda sinusoidal amortiguada exponencialmente
float ElasticIn(float p);
float ElasticOut(float p);
float ElasticInOut(float p);

// Easing cúbico con sobrepaso;
float BackIn(float p);
float BackOut(float p);
float BackInOut(float p);

// Easing de rebote con decaimiento exponencial
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