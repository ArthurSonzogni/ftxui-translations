// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <ftxui/dom/direction.hpp>  // for Direction, Direction::Down, Direction::Left, Direction::Right, Direction::Up
#include <memory>                   // for allocator, make_shared
#include <string>                   // for string

#include "ftxui/dom/elements.hpp"  // for Element, gauge, gaugeDirection, gaugeDown, gaugeLeft, gaugeRight, gaugeUp
#include "ftxui/dom/node.hpp"         // for Node
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/screen.hpp"    // for Screen, Pixel

namespace ftxui {

namespace {
// NOLINTNEXTLINE
static const std::string charset_horizontal[11] = {
#if defined(FTXUI_MICROSOFT_TERMINAL_FALLBACK)
    // 微軟的終端機通常使用不支援 8 個 Unicode 字元來表示整個儀表。
    // 使用較少的字元作為替代方案。
    " ", " ", " ", " ", "▌", "▌", "▌", "█", "█", "█",
#else
    " ", " ", "▏", "▎", "▍", "▌", "▋", "▊", "▉", "█",
#endif
    // 在 fuzzer 設法獲得以下情況時的額外字元：
    // int(9 * (limit - limit_int) = 9
    "█"};

// NOLINTNEXTLINE
static const std::string charset_vertical[10] = {
    "█",
    "▇",
    "▆",
    "▅",
    "▄",
    "▃",
    "▂",
    "▁",
    " ",
    // 在 fuzzer 設法獲得以下情況時的額外字元：
    // int(8 * (limit - limit_int) = 8
    " ",
};

class Gauge : public Node {
 public:
  Gauge(float progress, Direction direction)
      : progress_(progress), direction_(direction) {
    // This handle NAN correctly:
    if (!(progress_ > 0.F)) {
      progress_ = 0.F;
    }
    if (!(progress_ < 1.F)) {
      progress_ = 1.F;
    }
  }

  void ComputeRequirement() override {
    switch (direction_) {
      case Direction::Right:
      case Direction::Left:
        requirement_.flex_grow_x = 1;
        requirement_.flex_grow_y = 0;
        requirement_.flex_shrink_x = 1;
        requirement_.flex_shrink_y = 0;
        break;
      case Direction::Up:
      case Direction::Down:
        requirement_.flex_grow_x = 0;
        requirement_.flex_grow_y = 1;
        requirement_.flex_shrink_x = 0;
        requirement_.flex_shrink_y = 1;
        break;
    }
    requirement_.min_x = 1;
    requirement_.min_y = 1;
  }

  void Render(Screen& screen) override {
    switch (direction_) {
      case Direction::Right:
        RenderHorizontal(screen, /*invert=*/false);
        break;
      case Direction::Up:
        RenderVertical(screen, /*invert=*/false);
        break;
      case Direction::Left:
        RenderHorizontal(screen, /*invert=*/true);
        break;
      case Direction::Down:
        RenderVertical(screen, /*invert=*/true);
        break;
    }
  }

  void RenderHorizontal(Screen& screen, bool invert) {
    const int y = box_.y_min;
    if (y > box_.y_max) {
      return;
    }

    // Draw the progress bar horizontally.
    {
      const float progress = invert ? 1.F - progress_ : progress_;
      const auto limit =
          float(box_.x_min) + progress * float(box_.x_max - box_.x_min + 1);
      const int limit_int = static_cast<int>(limit);
      int x = box_.x_min;
      while (x < limit_int) {
        screen.at(x++, y) = charset_horizontal[9];  // NOLINT
      }
      // NOLINTNEXTLINE
      screen.at(x++, y) = charset_horizontal[int(9 * (limit - limit_int))];
      while (x <= box_.x_max) {
        screen.at(x++, y) = charset_horizontal[0];
      }
    }

    if (invert) {
      for (int x = box_.x_min; x <= box_.x_max; x++) {
        screen.PixelAt(x, y).inverted ^= true;
      }
    }
  }

  void RenderVertical(Screen& screen, bool invert) {
    const int x = box_.x_min;
    if (x > box_.x_max) {
      return;
    }

    // Draw the progress bar vertically:
    {
      const float progress = invert ? progress_ : 1.F - progress_;
      const float limit =
          float(box_.y_min) + progress * float(box_.y_max - box_.y_min + 1);
      const int limit_int = static_cast<int>(limit);
      int y = box_.y_min;
      while (y < limit_int) {
        screen.at(x, y++) = charset_vertical[8];  // NOLINT
      }
      // NOLINTNEXTLINE
      screen.at(x, y++) = charset_vertical[int(8 * (limit - limit_int))];
      while (y <= box_.y_max) {
        screen.at(x, y++) = charset_vertical[0];
      }
    }

    if (invert) {
      for (int y = box_.y_min; y <= box_.y_max; y++) {
        screen.PixelAt(x, y).inverted ^= true;
      }
    }
  }

 private:
  float progress_;
  Direction direction_;
};

}  // namespace

/// @brief 繪製一個指定方向的高解析度進度條。
/// @param progress 填充區域的比例。範圍為 [0,1]。
/// @param direction 進度條的進展方向。
/// @ingroup dom
Element gaugeDirection(float progress, Direction direction) {
  return std::make_shared<Gauge>(progress, direction);
}

/// @brief 繪製一個從左到右進展的高解析度進度條。
/// @param progress 填充區域的比例。範圍為 [0,1]。
/// @ingroup dom
///
/// ### 範例
///
/// 一個儀表。它可以用來表示進度條。
/// ~~~cpp
/// border(gaugeRight(0.5))
/// ~~~
///
/// #### 輸出
///
/// ~~~bash
/// ┌──────────────────────────────────────────────────────────────────────────┐
/// │█████████████████████████████████████                                     │
/// └──────────────────────────────────────────────────────────────────────────┘
/// ~~~
Element gaugeRight(float progress) {
  return gaugeDirection(progress, Direction::Right);
}

/// @brief 繪製一個從右到左進展的高解析度進度條。
/// @param progress 填充區域的比例。範圍為 [0,1]。
/// @ingroup dom
///
/// ### 範例
///
/// 一個儀表。它可以用來表示進度條。
/// ~~~cpp
/// border(gaugeLeft(0.5))
/// ~~~
///
/// #### 輸出
///
/// ~~~bash
/// ┌──────────────────────────────────────────────────────────────────────────┐
/// │                                     █████████████████████████████████████│
/// └──────────────────────────────────────────────────────────────────────────┘
/// ~~~
Element gaugeLeft(float progress) {
  return gaugeDirection(progress, Direction::Left);
}

/// @brief 繪製一個從下到上進展的高解析度進度條。
/// @param progress 填充區域的比例。範圍為 [0,1]。
/// @ingroup dom
///
/// ### 範例
///
/// 一個儀表。它可以用來表示進度條。
/// ~~~cpp
/// border(gaugeUp(0.5))
/// ~~~
///
/// #### 輸出
///
/// ~~~bash
///  ┌─┐
///  │ │
///  │ │
///  │ │
///  │ │
///  │█│
///  │█│
///  │█│
///  │█│
///  └─┘
/// ~~~
Element gaugeUp(float progress) {
  return gaugeDirection(progress, Direction::Up);
}

/// @brief 繪製一個從上到下進展的高解析度進度條。
/// @param progress 填充區域的比例。範圍為 [0,1]。
/// @ingroup dom
///
/// ### 範例
///
/// 一個儀表。它可以用來表示進度條。
/// ~~~cpp
/// border(gaugeDown(0.5))
/// ~~~
///
/// #### 輸出
///
/// ~~~bash
///  ┌─┐
///  │█│
///  │█│
///  │█│
///  │█│
///  │ │
///  │ │
///  │ │
///  │ │
///  └─┘
/// ~~~
Element gaugeDown(float progress) {
  return gaugeDirection(progress, Direction::Down);
}

/// @brief 繪製一個高解析度進度條。
/// @param progress 填充區域的比例。範圍為 [0,1]。
/// @ingroup dom
///
/// ### 範例
///
/// 一個儀表。它可以用來表示進度條。
/// ~~~cpp
/// border(gauge(0.5))
/// ~~~
///
/// #### 輸出
///
/// ~~~bash
/// ┌──────────────────────────────────────────────────────────────────────────┐
/// │█████████████████████████████████████                                     │
/// └──────────────────────────────────────────────────────────────────────────┘
/// ~~~
Element gauge(float progress) {
  return gaugeRight(progress);
}

}  // namespace ftxui
