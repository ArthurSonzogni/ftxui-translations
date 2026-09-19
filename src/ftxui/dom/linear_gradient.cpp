// Copyright 2023 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>                      // for max, min, sort, copy
#include <cmath>                          // for fmod, cos, sin
#include <cstddef>                        // for size_t
#include <ftxui/dom/linear_gradient.hpp>  // for LinearGradient::Stop, LinearGradient
#include <memory>    // for allocator_traits<>::value_type, make_shared
#include <optional>  // for optional, operator!=, operator<
#include <utility>   // for move
#include <vector>    // for vector

#include "ftxui/dom/elements.hpp"  // for Element, Decorator, bgcolor, color
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/box.hpp"          // for Box
#include "ftxui/screen/color.hpp"   // for Color, Color::Default, Color::Blue
#include "ftxui/screen/screen.hpp"  // for Cell, Screen

namespace ftxui {
namespace {

struct LinearGradientNormalized {
  float angle = 0.F;
  std::vector<Color> colors;
  std::vector<float> positions;  // Sorted.
};

// 將 LinearGradient 轉換為標準化版本。
LinearGradientNormalized Normalize(LinearGradient gradient) {
  // Handle gradient of size 0.
  if (gradient.stops.empty()) {
    return LinearGradientNormalized{
        0.F,
        {Color::Default, Color::Default},
        {0.F, 1.F},
    };
  }

  // Fill in the two extent, if not provided.
  if (!gradient.stops.front().position) {
    gradient.stops.front().position = 0.F;
  }
  if (!gradient.stops.back().position) {
    gradient.stops.back().position = 1.F;
  }

  // Fill in the blank, by interpolating positions.
  size_t last_checkpoint = 0;
  for (size_t i = 1; i < gradient.stops.size(); ++i) {
    if (!gradient.stops[i].position) {
      continue;
    }

    if (i - last_checkpoint >= 2) {
      const float min = gradient.stops[i].position.value();  // NOLINT
      const float max =
          gradient.stops[last_checkpoint].position.value();  // NOLINT
      for (size_t j = last_checkpoint + 1; j < i; ++j) {
        gradient.stops[j].position = min + (max - min) *
                                               float(j - last_checkpoint) /
                                               float(i - last_checkpoint);
      }
    }

    last_checkpoint = i;
  }

  // Sort the stops by position.
  std::sort(
      gradient.stops.begin(), gradient.stops.end(),
      [](const auto& a, const auto& b) { return a.position < b.position; });

  // If we don't being with zero, add a stop at zero.
  if (gradient.stops.front().position != 0) {
    gradient.stops.insert(gradient.stops.begin(),
                          {gradient.stops.front().color, 0.F});
  }
  // If we don't end with one, add a stop at one.
  if (gradient.stops.back().position != 1) {
    gradient.stops.push_back({gradient.stops.back().color, 1.F});
  }

  // Normalize the angle.
  LinearGradientNormalized normalized;
  const float modulo = 360.F;
  normalized.angle =
      std::fmod(std::fmod(gradient.angle, modulo) + modulo, modulo);
  for (auto& stop : gradient.stops) {
    normalized.colors.push_back(stop.color);
    // NOLINTNEXTLINE
    normalized.positions.push_back(stop.position.value());
  }
  return normalized;
}

Color Interpolate(const LinearGradientNormalized& gradient, float t) {
  // Find the right color in the gradient's stops.
  size_t i = 1;
  while (true) {
    // 請注意，由於浮點精度，`t` 可能會略大於 1.0。
    // 這就是為什麼我們需要處理 `t` 大於最後一個停止點位置的情況。
    // 請參閱 https://github.com/ArthurSonzogni/FTXUI/issues/998
    if (i >= gradient.positions.size()) {
      const float half = 0.5F;
      return Color::Interpolate(half, gradient.colors.back(),
                                gradient.colors.back());
    }
    if (t <= gradient.positions[i]) {
      break;
    }
    ++i;
  }

  const float t0 = gradient.positions[i - 1];
  const float t1 = gradient.positions[i - 0];
  const float tt = (t - t0) / (t1 - t0);

  const Color& c0 = gradient.colors[i - 1];
  const Color& c1 = gradient.colors[i - 0];
  const Color& cc = Color::Interpolate(tt, c0, c1);

  return cc;
}

class LinearGradientColor : public NodeDecorator {
 public:
  explicit LinearGradientColor(Element child,
                               const LinearGradient& gradient,
                               bool background_color)
      : NodeDecorator(std::move(child)),
        gradient_(Normalize(gradient)),
        background_color_{background_color} {}

 private:
  void Render(Screen& screen) override {
    const float degtorad = 0.01745329251F;
    const float dx = std::cos(gradient_.angle * degtorad);
    const float dy = std::sin(gradient_.angle * degtorad);

    // Project every corner to get the extent of the gradient.
    const float p1 = float(box_.x_min) * dx + float(box_.y_min) * dy;
    const float p2 = float(box_.x_min) * dx + float(box_.y_max) * dy;
    const float p3 = float(box_.x_max) * dx + float(box_.y_min) * dy;
    const float p4 = float(box_.x_max) * dx + float(box_.y_max) * dy;
    const float min = std::min({p1, p2, p3, p4});
    const float max = std::max({p1, p2, p3, p4});

    // Renormalize the projection to [0, 1] using the extent and projective
    // geometry.
    const float dX = dx / (max - min);
    const float dY = dy / (max - min);
    const float dZ = -min / (max - min);

    // Project every pixel to get the color.
    if (background_color_) {
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        for (int x = box_.x_min; x <= box_.x_max; ++x) {
          const float t = float(x) * dX + float(y) * dY + dZ;
          screen.CellAt(x, y).background_color = Interpolate(gradient_, t);
        }
      }
    } else {
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        for (int x = box_.x_min; x <= box_.x_max; ++x) {
          const float t = float(x) * dX + float(y) * dY + dZ;
          screen.CellAt(x, y).foreground_color = Interpolate(gradient_, t);
        }
      }
    }

    NodeDecorator::Render(screen);
  }

  LinearGradientNormalized gradient_;
  bool background_color_;
};

}  // namespace

/// @brief 建立「空」漸變。這通常會接著呼叫 LinearGradient::Angle() 和 LinearGradient::Stop()。
/// 範例：
/// ```cpp
///  auto gradient =
///   LinearGradient()
///    .Angle(45)
///    .Stop(Color::Red, 0.0)
///    .Stop(Color::Green, 0.5)
///    .Stop(Color::Blue, 1.0);;
/// ```
LinearGradient::LinearGradient() = default;

/// @brief 建立一個包含兩種顏色的漸變。
/// @param begin 漸變開始處的顏色。
/// @param end 漸變結束處的顏色。
LinearGradient::LinearGradient(Color begin, Color end)
    : LinearGradient(0, begin, end) {}

/// @brief 建立一個包含兩種顏色和角度的漸變。
/// @param a 漸變的角度。
/// @param begin 漸變開始處的顏色。
/// @param end 漸變結束處的顏色。
LinearGradient::LinearGradient(float a, Color begin, Color end) : angle(a) {
  stops.push_back({begin, {}});
  stops.push_back({end, {}});
}

/// @brief 設定漸變的角度。
/// @param a 漸變的角度。
/// @return 漸變。
LinearGradient& LinearGradient::Angle(float a) {
  angle = a;
  return *this;
}

/// @brief 為漸變添加一個顏色停止點。
/// @param c 停止點的顏色。
/// @param p 停止點的位置。
LinearGradient& LinearGradient::Stop(Color c, float p) {
  stops.push_back({c, p});
  return *this;
}

/// @brief 為漸變添加一個顏色停止點。
/// @param c 停止點的顏色。
/// @return 漸變。
/// @note 停止點的位置是從附近的停止點內插而來的。
LinearGradient& LinearGradient::Stop(Color c) {
  stops.push_back({c, {}});
  return *this;
}

/// @brief 使用線性漸變效果設定元素的前景色。
/// @param gradient 要應用於輸出元素的漸變效果。
/// @param child 輸入元素。
/// @return 已著色的輸出元素。
/// @ingroup dom
///
/// ### 範例
///
/// ```cpp
/// color(LinearGradient{0, {Color::Red, Color::Blue}}, text("Hello"))
/// ```
Element color(const LinearGradient& gradient, Element child) {
  return std::make_shared<LinearGradientColor>(std::move(child), gradient,
                                               /*background_color*/ false);
}

/// @brief 使用線性漸變效果設定元素的背景顏色。
/// @param gradient 要應用於輸出元素的漸變效果。
/// @param child 輸入元素。
/// @return 已著色的輸出元素。
/// @ingroup dom
///
/// ### 範例
///
/// ```cpp
/// bgcolor(LinearGradient{0, {Color::Red, Color::Blue}}, text("Hello"))
/// ```
Element bgcolor(const LinearGradient& gradient, Element child) {
  return std::make_shared<LinearGradientColor>(std::move(child), gradient,
                                               /*background_color*/ true);
}

/// @brief 使用線性漸變效果裝飾前景色。
/// @param gradient 要應用於輸出元素的漸變效果。
/// @return 應用顏色的 Decorator。
/// @ingroup dom
///
/// ### 範例
///
/// ```cpp
/// text("Hello") | color(LinearGradient{0, {Color::Red, Color::Blue}})
/// ```
Decorator color(const LinearGradient& gradient) {
  return
      [gradient](Element child) { return color(gradient, std::move(child)); };
}

/// @brief 使用線性漸變效果裝飾背景顏色。
/// @param gradient 要應用於輸出元素的漸變效果。
/// @return 應用顏色的 Decorator。
/// @ingroup dom
///
/// ### 範例
///
/// ```cpp
/// text("Hello") | color(LinearGradient{0, {Color::Red, Color::Blue}})
/// ```
Decorator bgcolor(const LinearGradient& gradient) {
  return
      [gradient](Element child) { return bgcolor(gradient, std::move(child)); };
}

}  // namespace ftxui
