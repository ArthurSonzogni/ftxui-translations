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
#include "ftxui/screen/screen.hpp"    // for Screen, Cell

#include "ftxui/screen/terminal.hpp"  // for Quirks, GetQuirks

namespace ftxui {

namespace {
// NOLINTNEXTLINE
static const std::string charset_horizontal[11] = {
    " ", " ", "▏", "▎", "▍", "▌", "▋", "▊", "▉", "█",
    // ファザーが以下のような状況を管理する場合の追加文字:
    // int(9 * (limit - limit_int) = 9
    "█"};

// NOLINTNEXTLINE
static const std::string charset_horizontal_microsoft[11] = {
    " ", " ", " ", " ", "▌", "▌", "▌", "█", "█", "█",
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
    "▁",
    " ",
    // ファザーが以下のような状況を管理する場合の追加文字:
    // int(8 * (limit - limit_int) = 8
    " ",
};

class Gauge : public Node {
 public:
  Gauge(float progress, Direction direction, std::vector<std::string> charset = {})
      : progress_(progress),
        direction_(direction),
        charset_(std::move(charset)) {
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
    if (box_.y_min > box_.y_max) {
      return;
    }

    // `full`は`charset`内の「full」グリフのインデックス。境界セルは
    // int(full * fractional_fill)のグリフインデックスを選ぶ。
    const std::string* charset;
    int full;
    if (charset_.empty()) {
      charset = Terminal::GetQuirks().BlockCharacters()  // NOLINT
                    ? charset_horizontal
                    : charset_horizontal_microsoft;
      full = 9;
    } else {
      charset = charset_.data();
      full = static_cast<int>(charset_.size()) - 1;
    }

    // プログレスバーを割り当てられた高さいっぱいに水平に描画する:
    const float progress = invert ? 1.F - progress_ : progress_;
    const auto limit =
        float(box_.x_min) + progress * float(box_.x_max - box_.x_min + 1);
    const int limit_int = static_cast<int>(limit);

    for (int y = box_.y_min; y <= box_.y_max; y++) {
      int x = box_.x_min;
      while (x < limit_int) {
        screen.at(x++, y) = charset[full];
      }
      if (x <= box_.x_max) {
        screen.at(x++, y) = charset[int(full * (limit - limit_int))];
      }
      while (x <= box_.x_max) {
        screen.at(x++, y) = charset[0];
      }
    }

    if (invert) {
      Invert(screen);
    }
  }

  void RenderVertical(Screen& screen, bool invert) {
    if (box_.x_min > box_.x_max) {
      return;
    }

    const std::string* charset;
    int full;
    if (charset_.empty()) {
      charset = charset_vertical;
      full = 8;
    } else {
      charset = charset_.data();
      full = static_cast<int>(charset_.size()) - 1;
    }

    // プログレスバーを割り当てられた幅いっぱいに垂直に描画する:
    const float progress = invert ? progress_ : 1.F - progress_;
    const float limit =
        float(box_.y_min) + progress * float(box_.y_max - box_.y_min + 1);
    const int limit_int = static_cast<int>(limit);

    for (int x = box_.x_min; x <= box_.x_max; x++) {
      int y = box_.y_min;
      while (y < limit_int) {
        screen.at(x, y++) = charset[full];
      }
      if (y <= box_.y_max) {
        screen.at(x, y++) = charset[int(full * (limit - limit_int))];
      }
      while (y <= box_.y_max) {
        screen.at(x, y++) = charset[0];
      }
    }
    if (invert) {
      Invert(screen);
    }
  }

  void Invert(Screen& screen) {
    for (int y = box_.y_min; y <= box_.y_max; y++) {
      for (int x = box_.x_min; x <= box_.x_max; x++) {
        screen.CellAt(x, y).inverted ^= true;
      }
    }
  }

 private:
  float progress_;
  Direction direction_;
  std::vector<std::string> charset_;
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

/// @brief カスタム文字セットを使って高精細なプログレスバーを描画する。
/// @param progress 塗りつぶす領域の割合。[0,1]に属する。
/// @param charset 「empty」(インデックス0)から「full」(最後のインデックス)
/// までのグリフ。2要素の文字セットは単純な塗りつぶしのないバーになる。
/// @param direction プログレスバーの進行方向。デフォルトはRight。
/// @ingroup dom
///
/// ### Example
///
/// デフォルトのブロック文字の代わりに、カスタム文字セットで描画された
/// ゲージ。
/// ~~~cpp
/// border(gaugeCharset(0.5, {".", "#"}))
/// ~~~
///
/// #### Output
///
/// ~~~bash
/// ┌──────────────────────────────────────────────────────────────────────────┐
/// │#####################################.....................................│
/// └──────────────────────────────────────────────────────────────────────────┘
/// ~~~
Element gaugeCharset(float progress,
                      std::vector<std::string> charset,
                      Direction direction) {
  return std::make_shared<Gauge>(progress, direction, std::move(charset));
}

}  // namespace ftxui
