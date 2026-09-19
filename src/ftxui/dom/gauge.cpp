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
    // 在 fuzzer 設法獲得以下情況時的額外字元：
    // int(9 * (limit - limit_int) = 9
    "█"};

// NOLINTNEXTLINE
static const std::string charset_horizontal_microsoft[11] = {
    " ", " ", " ", " ", "▌", "▌", "▌", "█", "█", "█",
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
  Gauge(float progress, Direction direction, std::vector<std::string> charset = {})
      : progress_(progress),
        direction_(direction),
        charset_(std::move(charset)) {
    // 這能正確處理 NAN：
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

    // `full` 是 `charset` 中「full」字符的索引；邊界
    // 儲存格會選擇索引 int(full * fractional_fill) 的字符。
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

    // 在整個分配到的高度範圍內水平繪製進度條：
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

    // 在整個分配到的寬度範圍內垂直繪製進度條：
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

/// @brief 使用自訂字元集繪製高解析度進度條。
/// @param progress 要填滿的區域比例。屬於 [0,1]。
/// @param charset 從「空」（索引 0）到「滿」（最後一個索引）的字符；
/// 只有 2 個項目的字元集會產生單純無陰影的進度條。
/// @param direction 進度條前進的方向。預設為
/// Right。
/// @ingroup dom
///
/// ### 範例
///
/// 使用自訂字元集而非預設方塊字符繪製的進度條。
/// ~~~cpp
/// border(gaugeCharset(0.5, {".", "#"}))
/// ~~~
///
/// #### 輸出
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
