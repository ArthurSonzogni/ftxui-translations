// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
// Copyright 2020 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスに従います。
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
    // Microsoftのターミナルは、完全なゲージを表す8つのUnicode文字を処理できないフォントを使用していることがよくあります。
    // そのため、より少ない文字でフォールバックします。
    " ", " ", " ", " ", "▌", "▌", "▌", "█", "█", "█",
#else
    " ", " ", "▏", "▎", "▍", "▌", "▋", "▊", "▉", "█",
#endif
    // ファザーが以下のような状況を管理する場合の追加文字:
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
    " ",
    " ",
    // ファザーが以下のような状況を管理する場合の追加文字:
    // int(8 * (limit - limit_int) = 8
    " ",
};

class Gauge : public Node {
 public:
  Gauge(float progress, Direction direction)
      : progress_(progress), direction_(direction) {
    // This handle NAN correctly:
    // これはNANを正しく処理します:
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
    // プログレスバーを水平に描画します。
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
    // プログレスバーを垂直に描画します。
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

/// @brief 指定された方向に進行する高精細プログレスバーを描画します。
/// @param progress 塗りつぶされる領域の割合。[0,1]に属します。
/// @param direction プログレスバーの進行方向。
/// @ingroup dom
Element gaugeDirection(float progress, Direction direction) {
  return std::make_shared<Gauge>(progress, direction);
}

/// @brief 左から右へ進行する高精細プログレスバーを描画します。
/// @param progress 塗りつぶされる領域の割合。[0,1]に属します。
/// @ingroup dom
///
/// ### Example
///
/// ゲージ。プログレスバーとして使用できます。
/// ~~~cpp
/// border(gaugeRight(0.5))
/// ~~~
///
/// #### Output
///
/// ~~~bash
/// ┌──────────────────────────────────────────────────────────────────────────┐
/// │█████████████████████████████████████                                     │
/// └──────────────────────────────────────────────────────────────────────────┘
/// ~~~
Element gaugeRight(float progress) {
  return gaugeDirection(progress, Direction::Right);
}

/// @brief 右から左へ進行する高精細プログレスバーを描画します。
/// @param progress 塗りつぶされる領域の割合。[0,1]に属します。
/// @ingroup dom
///
/// ### Example
///
/// ゲージ。プログレスバーとして使用できます。
/// ~~~cpp
/// border(gaugeLeft(0.5))
/// ~~~
///
/// #### Output
///
/// ~~~bash
/// ┌──────────────────────────────────────────────────────────────────────────┐
/// │                                     █████████████████████████████████████│
/// └──────────────────────────────────────────────────────────────────────────┘
/// ~~~
Element gaugeLeft(float progress) {
  return gaugeDirection(progress, Direction::Left);
}

/// @brief 下から上へ進行する高精細プログレスバーを描画します。
/// @param progress 塗りつぶされる領域の割合。[0,1]に属します。
/// @ingroup dom
///
/// ### Example
///
/// ゲージ。プログレスバーとして使用できます。
/// ~~~cpp
/// border(gaugeUp(0.5))
/// ~~~
///
/// #### Output
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

/// @brief 上から下へ進行する高精細プログレスバーを描画します。
/// @param progress 塗りつぶされる領域の割合。[0,1]に属します。
/// @ingroup dom
///
/// ### Example
///
/// ゲージ。プログレスバーとして使用できます。
/// ~~~cpp
/// border(gaugeDown(0.5))
/// ~~~
///
/// #### Output
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

/// @brief 高精細プログレスバーを描画します。
/// @param progress 塗りつぶされる領域の割合。[0,1]に属します。
/// @ingroup dom
///
/// ### Example
///
/// ゲージ。プログレスバーとして使用できます。
/// ~~~cpp
/// border(gauge(0.5))
/// ~~~
///
/// #### Output
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