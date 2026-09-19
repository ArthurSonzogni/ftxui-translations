// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_ANIMATION_HPP
#define FTXUI_ANIMATION_HPP

#include <chrono>      // for milliseconds, duration, steady_clock, time_point
#include <functional>  // for function
#include "ftxui/util/export.hpp"

namespace ftxui::animation {
/// @brief RequestAnimationFrame est une fonction qui demande à ce qu'une nouvelle trame soit
/// dessinée lors du prochain cycle d'animation.
///
/// @note Cette fonction est typiquement appelée par les composants qui ont besoin de
/// mettre à jour leur état ou leur apparence au fil du temps, comme les animations ou
/// les transitions. Ceci est utile lorsque le changement ne dépend pas des
/// événements vus par le terminal, mais plutôt du temps qui passe.
///
/// Les composants qui n'ont pas terminé leur animation peuvent appeler cette fonction pour
/// demander à ce qu'une nouvelle trame soit dessinée plus tard.
///
/// Lorsqu'il n'y a pas de nouveaux événements et pas d'animations à compléter, aucune nouvelle trame n'est
/// dessinée.
///
/// @ingroup component
FTXUI_EXPORT(COMPONENT) void RequestAnimationFrame();

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Duration = std::chrono::duration<float>;

// Paramètre de Component::OnAnimation(param).
class FTXUI_EXPORT(COMPONENT) Params {
 public:
  explicit Params(Duration duration) : duration_(duration) {}

  /// La durée que représente cette étape d'animation.
  Duration duration() const { return duration_; }

 private:
  Duration duration_;
};

namespace easing {
using Function = std::function<float(float)>;
// Interpolation linéaire (sans accélération/décélération)
FTXUI_EXPORT(COMPONENT) float Linear(float p);

// Accélération/décélération quadratique; p^2
FTXUI_EXPORT(COMPONENT) float QuadraticIn(float p);
FTXUI_EXPORT(COMPONENT) float QuadraticOut(float p);
FTXUI_EXPORT(COMPONENT) float QuadraticInOut(float p);

// Accélération/décélération cubique; p^3
FTXUI_EXPORT(COMPONENT) float CubicIn(float p);
FTXUI_EXPORT(COMPONENT) float CubicOut(float p);
FTXUI_EXPORT(COMPONENT) float CubicInOut(float p);

// Accélération/décélération quartique; p^4
FTXUI_EXPORT(COMPONENT) float QuarticIn(float p);
FTXUI_EXPORT(COMPONENT) float QuarticOut(float p);
FTXUI_EXPORT(COMPONENT) float QuarticInOut(float p);

// Accélération/décélération quintique; p^5
FTXUI_EXPORT(COMPONENT) float QuinticIn(float p);
FTXUI_EXPORT(COMPONENT) float QuinticOut(float p);
FTXUI_EXPORT(COMPONENT) float QuinticInOut(float p);

// Accélération/décélération en onde sinusoïdale; sin(p * PI/2)
FTXUI_EXPORT(COMPONENT) float SineIn(float p);
FTXUI_EXPORT(COMPONENT) float SineOut(float p);
FTXUI_EXPORT(COMPONENT) float SineInOut(float p);

// Accélération/décélération circulaire; sqrt(1 - p^2)
FTXUI_EXPORT(COMPONENT) float CircularIn(float p);
FTXUI_EXPORT(COMPONENT) float CircularOut(float p);
FTXUI_EXPORT(COMPONENT) float CircularInOut(float p);

// Accélération/décélération exponentielle, base 2
FTXUI_EXPORT(COMPONENT) float ExponentialIn(float p);
FTXUI_EXPORT(COMPONENT) float ExponentialOut(float p);
FTXUI_EXPORT(COMPONENT) float ExponentialInOut(float p);

// Accélération/décélération en onde sinusoïdale exponentiellement amortie
FTXUI_EXPORT(COMPONENT) float ElasticIn(float p);
FTXUI_EXPORT(COMPONENT) float ElasticOut(float p);
FTXUI_EXPORT(COMPONENT) float ElasticInOut(float p);

// Accélération/décélération cubique avec dépassement;
FTXUI_EXPORT(COMPONENT) float BackIn(float p);
FTXUI_EXPORT(COMPONENT) float BackOut(float p);
FTXUI_EXPORT(COMPONENT) float BackInOut(float p);

// Accélération/décélération de rebond à décroissance exponentielle
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
