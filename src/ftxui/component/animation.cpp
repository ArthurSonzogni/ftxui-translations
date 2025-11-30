#include <cmath>    // for sin, pow, sqrt, cos
#include <utility>  // for move

#include "ftxui/component/animation.hpp"

// NOLINTBEGIN(*-magic-numbers)
namespace ftxui::animation {

namespace easing {

namespace {
constexpr float kPi = 3.14159265358979323846f;
constexpr float kPi2 = kPi / 2.f;
}  // namespace

// Las funciones de aceleración han sido tomadas de:
// https://github.com/warrenm/AHEasing/blob/master/AHEasing/easing.c
//
// Licencia correspondiente:
//  Copyright (c) 2011, Auerhaus Development, LLC
//
//  Este programa es software libre. Se proporciona sin ninguna garantía, en
//  la medida permitida por la ley aplicable. Puedes redistribuirlo
//  y/o modificarlo bajo los términos de la Licencia Pública Do What The Fuck You Want
//  To, Versión 2, publicada por Sam Hocevar. Consulta
//  http://sam.zoy.org/wtfpl/COPYING para más detalles.

/// @brief Modelado según la línea y = x
float Linear(float p) {
  return p;
}

/// @brief Modelado según la parábola y = x^2
float QuadraticIn(float p) {
  return p * p;
}

// @brief Modelado según la parábola y = -x^2 + 2x
float QuadraticOut(float p) {
  return -(p * (p - 2.f));
}

// @brief Modelado según la cuadrática a trozos
// y = (1/2)((2x)^2)             ; [0, 0.5)
// y = -(1/2)((2x-1)*(2x-3) - 1) ; [0.5, 1]
float QuadraticInOut(float p) {
  return p < 0.5f ? 2.f * p * p : (-2.f * p * p) + (4.f * p) - 1.f;
}

// @brief Modelado según la cúbica y = x^3
float CubicIn(float p) {
  return p * p * p;
}

// @brief Modelado según la cúbica y = (x - 1)^3 + 1
float CubicOut(float p) {
  const float f = (p - 1.f);
  return f * f * f + 1.f;
}

// @brief Modelado según la cúbica a trozos
// y = (1/2)((2x)^3)       ; [0, 0.5)
// y = (1/2)((2x-2)^3 + 2) ; [0.5, 1]
float CubicInOut(float p) {
  if (p < 0.5f) {
    return 4.f * p * p * p;
  }
  const float f = ((2.f * p) - 2.f);
  return 0.5f * f * f * f + 1.f;
}

// @brief Modelado según la cuártica x^4
float QuarticIn(float p) {
  return p * p * p * p;
}

// @brief Modelado según la cuártica y = 1 - (x - 1)^4
float QuarticOut(float p) {
  const float f = (p - 1.f);
  return f * f * f * (1.f - p) + 1.f;
}

// @brief Modelado según la cuártica a trozos
// y = (1/2)((2x)^4)        ; [0, 0.5)
// y = -(1/2)((2x-2)^4 - 2) ; [0.5, 1]
float QuarticInOut(float p) {
  if (p < 0.5f) {
    return 8.f * p * p * p * p;
  }
  const float f = (p - 1.f);
  return -8.f * f * f * f * f + 1.f;
}

// @brief Modelado según la quíntica y = x^5
float QuinticIn(float p) {
  return p * p * p * p * p;
}

// @brief Modelado según la quíntica y = (x - 1)^5 + 1
float QuinticOut(float p) {
  const float f = (p - 1.f);
  return f * f * f * f * f + 1.f;
}

// @brief Modelado según la quíntica a trozos
// y = (1/2)((2x)^5)       ; [0, 0.5)
// y = (1/2)((2x-2)^5 + 2) ; [0.5, 1]
float QuinticInOut(float p) {
  if (p < 0.5f) {
    return 16.f * p * p * p * p * p;
  }
  const float f = ((2.f * p) - 2.f);
  return 0.5f * f * f * f * f * f + 1.f;
}

// @brief Modelado según el cuarto de ciclo de onda sinusoidal
float SineIn(float p) {
  return std::sin((p - 1.f) * kPi2) + 1.f;
}

// @brief Modelado según el cuarto de ciclo de onda sinusoidal (fase diferente)
float SineOut(float p) {
  return std::sin(p * kPi2);
}

// @brief Modelado según la media onda sinusoidal
float SineInOut(float p) {
  return 0.5f * (1.f - std::cos(p * kPi));
}

// @brief Modelado según el cuadrante IV desplazado del círculo unitario
float CircularIn(float p) {
  return 1.f - std::sqrt(1.f - (p * p));
}

// @brief Modelado según el cuadrante II desplazado del círculo unitario
float CircularOut(float p) {
  return std::sqrt((2.f - p) * p);
}

// @brief Modelado según la función circular a trozos
// y = (1/2)(1 - sqrt(1 - 4x^2))           ; [0, 0.5)
// y = (1/2)(sqrt(-(2x - 3)*(2x - 1)) + 1) ; [0.5, 1]
float CircularInOut(float p) {
  if (p < 0.5f) {
    return 0.5f * (1.f - std::sqrt(1.f - 4.f * (p * p)));
  }
  return 0.5f * (std::sqrt(-((2.f * p) - 3.f) * ((2.f * p) - 1.f)) + 1.f);
}

// @brief Modelado según la función exponencial y = 2^(10(x - 1))
float ExponentialIn(float p) {
  return (p == 0.f) ? p : std::pow(2.f, 10.f * (p - 1.f));
}

// @brief Modelado según la función exponencial y = -2^(-10x) + 1
float ExponentialOut(float p) {
  return (p == 1.f) ? p : 1.f - std::pow(2.f, -10.f * p);
}

// @brief Modelado según la exponencial a trozos
// y = (1/2)2^(10(2x - 1))         ; [0,0.5)
// y = -(1/2)*2^(-10(2x - 1))) + 1 ; [0.5,1]
float ExponentialInOut(float p) {
  if (p == 0.f || p == 1.f) {
    return p;
  }

  if (p < 0.5f) {
    return 0.5f * std::pow(2.f, (20.f * p) - 10.f);
  }
  return -0.5f * std::pow(2.f, (-20.f * p) + 10.f) + 1.f;
}

// @brief Modelado según la onda sinusoidal amortiguada y = sin(13pi/2*x)*pow(2, 10 * (x - 1))
float ElasticIn(float p) {
  return std::sin(13.f * kPi2 * p) * std::pow(2.f, 10.f * (p - 1.f));
}

// @brief Modelado según la onda sinusoidal amortiguada y = sin(-13pi/2*(x + 1))*pow(2,
// -10x) +
// 1
float ElasticOut(float p) {
  return std::sin(-13.f * kPi2 * (p + 1.f)) * std::pow(2.f, -10.f * p) + 1.f;
}

// @brief Modelado según la onda sinusoidal amortiguada exponencialmente a trozos:
// y = (1/2)*sin(13pi/2*(2*x))*pow(2, 10 * ((2*x) - 1))      ; [0,0.5)
// y = (1/2)*(sin(-13pi/2*((2x-1)+1))*pow(2,-10(2*x-1)) + 2) ; [0.5, 1]
float ElasticInOut(float p) {
  if (p < 0.5f) {
    return 0.5f * std::sin(13.f * kPi2 * (2.f * p)) *
           std::pow(2.f, 10.f * ((2.f * p) - 1.f));
  }
  return 0.5f * (std::sin(-13.f * kPi2 * ((2.f * p - 1.f) + 1.f)) *
                     std::pow(2.f, -10.f * (2.f * p - 1.f)) +
                 2.f);
}

// @brief Modelado según la cúbica con sobrepaso y = x^3-x*sin(x*pi)
float BackIn(float p) {
  return p * p * p - p * std::sin(p * kPi);
}

// @brief Modelado según la cúbica con sobrepaso y = 1-((1-x)^3-(1-x)*sin((1-x)*pi))
float BackOut(float p) {
  const float f = (1.f - p);
  return 1.f - (f * f * f - f * std::sin(f * kPi));
}

// @brief Modelado según la función cúbica con sobrepaso a trozos:
// y = (1/2)*((2x)^3-(2x)*sin(2*x*pi))           ; [0, 0.5)
// y = (1/2)*(1-((1-x)^3-(1-x)*sin((1-x)*pi))+1) ; [0.5, 1]
float BackInOut(float p) {
  if (p < 0.5f) {
    const float f = 2.f * p;
    return 0.5f * (f * f * f - f * std::sin(f * kPi));
  }
  const float f = (1.f - (2.f * p - 1.f));
  return 0.5f * (1.f - (f * f * f - f * std::sin(f * kPi))) + 0.5f;
}

float BounceIn(float p) {
  return 1.f - BounceOut(1.f - p);
}

float BounceOut(float p) {
  if (p < 4.f / 11.f) {
    return (121.f * p * p) / 16.f;
  }

  if (p < 8.f / 11.f) {
    return (363.f / 40.f * p * p) - (99.f / 10.f * p) + 17.f / 5.f;
  }

  if (p < 9.f / 10.f) {
    return (4356.f / 361.f * p * p) - (35442.f / 1805.f * p) + 16061.f / 1805.f;
  }

  return (54.f / 5.f * p * p) - (513 / 25.f * p) + 268 / 25.f;
}

float BounceInOut(float p) {
  if (p < 0.5f) {
    return 0.5f * BounceIn(p * 2.f);
  }
  return 0.5f * BounceOut(p * 2.f - 1.f) + 0.5f;
}

}  // namespace easing

Animator::Animator(float* from,
                   float to,
                   Duration duration,
                   easing::Function easing_function,
                   Duration delay)
    : value_(from),
      from_(*from),
      to_(to),
      duration_(duration),
      easing_function_(std::move(easing_function)),
      current_(-delay) {
  RequestAnimationFrame();
}

void Animator::OnAnimation(Params& params) {
  current_ += params.duration();

  if (current_ >= duration_) {
    *value_ = to_;
    return;
  }

  if (current_ <= Duration()) {
    *value_ = from_;
  } else {
    *value_ = from_ + (to_ - from_) * easing_function_(current_ / duration_);
  }

  RequestAnimationFrame();
}

}  // namespace ftxui::animation

// NOLINTEND(*-magic-numbers)