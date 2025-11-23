// Copyright 2022 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui se trouve dans
// le fichier LICENSE.
#ifndef FTXUI_ANIMATION_HPP
#define FTXUI_ANIMATION_HPP

#include <chrono>      // for milliseconds, duration, steady_clock, time_point
#include <functional>  // for function

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
void RequestAnimationFrame();

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Duration = std::chrono::duration<float>;

// Paramètre de Component::OnAnimation(param).
class Params {
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
float Linear(float p);

// Accélération/décélération quadratique; p^2
float QuadraticIn(float p);
float QuadraticOut(float p);
float QuadraticInOut(float p);

// Accélération/décélération cubique; p^3
float CubicIn(float p);
float CubicOut(float p);
float CubicInOut(float p);

// Accélération/décélération quartique; p^4
float QuarticIn(float p);
float QuarticOut(float p);
float QuarticInOut(float p);

// Accélération/décélération quintique; p^5
float QuinticIn(float p);
float QuinticOut(float p);
float QuinticInOut(float p);

// Accélération/décélération en onde sinusoïdale; sin(p * PI/2)
float SineIn(float p);
float SineOut(float p);
float SineInOut(float p);

// Accélération/décélération circulaire; sqrt(1 - p^2)
float CircularIn(float p);
float CircularOut(float p);
float CircularInOut(float p);

// Accélération/décélération exponentielle, base 2
float ExponentialIn(float p);
float ExponentialOut(float p);
float ExponentialInOut(float p);

// Accélération/décélération en onde sinusoïdale exponentiellement amortie
float ElasticIn(float p);
float ElasticOut(float p);
float ElasticInOut(float p);

// Accélération/décélération cubique avec dépassement;
float BackIn(float p);
float BackOut(float p);
float BackInOut(float p);

// Accélération/décélération de rebond à décroissance exponentielle
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
