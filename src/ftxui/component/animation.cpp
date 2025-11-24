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

// イージング関数は以下から引用されています:
// https://github.com/warrenm/AHEasing/blob/master/AHEasing/easing.c
//
// 対応するライセンス:
//  Copyright (c) 2011, Auerhaus Development, LLC
//
//  このプログラムはフリーソフトウェアです。適用される法律で許可されている範囲で、いかなる保証もありません。
//  Sam Hocevarによって公開されたDo What The Fuck You Want
//  To Public License, Version 2の条件に基づいて再配布および/または変更することができます。
//  詳細については、http://sam.zoy.org/wtfpl/COPYING を参照してください。

/// @brief 直線 y = x に基づく
float Linear(float p) {
  return p;
}

/// @brief 放物線 y = x^2 に基づく
float QuadraticIn(float p) {
  return p * p;
}

// @brief 放物線 y = -x^2 + 2x に基づく
float QuadraticOut(float p) {
  return -(p * (p - 2.f));
}

// @brief 区分的二次関数に基づく
// y = (1/2)((2x)^2)             ; [0, 0.5)
// y = -(1/2)((2x-1)*(2x-3) - 1) ; [0.5, 1]
float QuadraticInOut(float p) {
  return p < 0.5f ? 2.f * p * p : (-2.f * p * p) + (4.f * p) - 1.f;
}

// @brief 三次関数 y = x^3 に基づく
float CubicIn(float p) {
  return p * p * p;
}

// @brief 三次関数 y = (x - 1)^3 + 1 に基づく
float CubicOut(float p) {
  const float f = (p - 1.f);
  return f * f * f + 1.f;
}

// @brief 区分的三次関数に基づく
// y = (1/2)((2x)^3)       ; [0, 0.5)
// y = (1/2)((2x-2)^3 + 2) ; [0.5, 1]
float CubicInOut(float p) {
  if (p < 0.5f) {
    return 4.f * p * p * p;
  }
  const float f = ((2.f * p) - 2.f);
  return 0.5f * f * f * f + 1.f;
}

// @brief 四次関数 x^4 に基づく
float QuarticIn(float p) {
  return p * p * p * p;
}

// @brief 四次関数 y = 1 - (x - 1)^4 に基づく
float QuarticOut(float p) {
  const float f = (p - 1.f);
  return f * f * f * (1.f - p) + 1.f;
}

// @brief 区分的四次関数に基づく
// y = (1/2)((2x)^4)        ; [0, 0.5)
// y = -(1/2)((2x-2)^4 - 2) ; [0.5, 1]
float QuarticInOut(float p) {
  if (p < 0.5f) {
    return 8.f * p * p * p * p;
  }
  const float f = (p - 1.f);
  return -8.f * f * f * f * f + 1.f;
}

// @brief 五次関数 y = x^5 に基づく
float QuinticIn(float p) {
  return p * p * p * p * p;
}

// @brief 五次関数 y = (x - 1)^5 + 1 に基づく
float QuinticOut(float p) {
  const float f = (p - 1.f);
  return f * f * f * f * f + 1.f;
}

// @brief 区分的五次関数に基づく
// y = (1/2)((2x)^5)       ; [0, 0.5)
// y = (1/2)((2x-2)^5 + 2) ; [0.5, 1]
float QuinticInOut(float p) {
  if (p < 0.5f) {
    return 16.f * p * p * p * p * p;
  }
  const float f = ((2.f * p) - 2.f);
  return 0.5f * f * f * f * f * f + 1.f;
}

// @brief サイン波の四半周期に基づく
float SineIn(float p) {
  return std::sin((p - 1.f) * kPi2) + 1.f;
}

// @brief サイン波の四半周期に基づく (異なる位相)
float SineOut(float p) {
  return std::sin(p * kPi2);
}

// @brief 半分のサイン波に基づく
float SineInOut(float p) {
  return 0.5f * (1.f - std::cos(p * kPi));
}

// @brief 単位円の第IV象限をシフトしたものに基づく
float CircularIn(float p) {
  return 1.f - std::sqrt(1.f - (p * p));
}

// @brief 単位円の第II象限をシフトしたものに基づく
float CircularOut(float p) {
  return std::sqrt((2.f - p) * p);
}

// @brief 区分的円関数に基づく
// y = (1/2)(1 - sqrt(1 - 4x^2))           ; [0, 0.5)
// y = (1/2)(sqrt(-(2x - 3)*(2x - 1)) + 1) ; [0.5, 1]
float CircularInOut(float p) {
  if (p < 0.5f) {
    return 0.5f * (1.f - std::sqrt(1.f - 4.f * (p * p)));
  }
  return 0.5f * (std::sqrt(-((2.f * p) - 3.f) * ((2.f * p) - 1.f)) + 1.f);
}

// @brief 指数関数 y = 2^(10(x - 1)) に基づく
float ExponentialIn(float p) {
  return (p == 0.f) ? p : std::pow(2.f, 10.f * (p - 1.f));
}

// @brief 指数関数 y = -2^(-10x) + 1 に基づく
float ExponentialOut(float p) {
  return (p == 1.f) ? p : 1.f - std::pow(2.f, -10.f * p);
}

// @brief 区分的指数関数に基づく
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

// @brief 減衰サイン波 y = sin(13pi/2*x)*pow(2, 10 * (x - 1)) に基づく
float ElasticIn(float p) {
  return std::sin(13.f * kPi2 * p) * std::pow(2.f, 10.f * (p - 1.f));
}

// @brief 減衰サイン波 y = sin(-13pi/2*(x + 1))*pow(2, -10x) + 1 に基づく
float ElasticOut(float p) {
  return std::sin(-13.f * kPi2 * (p + 1.f)) * std::pow(2.f, -10.f * p) + 1.f;
}

// @brief 区分的指数減衰サイン波に基づく:
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

// @brief オーバーシュートする三次関数 y = x^3-x*sin(x*pi) に基づく
float BackIn(float p) {
  return p * p * p - p * std::sin(p * kPi);
}

// @brief オーバーシュートする三次関数 y = 1-((1-x)^3-(1-x)*sin((1-x)*pi)) に基づく
float BackOut(float p) {
  const float f = (1.f - p);
  return 1.f - (f * f * f - f * std::sin(f * kPi));
}

// @brief 区分的オーバーシュート三次関数に基づく:
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