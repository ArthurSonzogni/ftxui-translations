// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <array>    // for array, array<>::value_type
#include <memory>   // for make_shared, allocator
#include <string>   // for basic_string, string
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Element, BorderStyle, LIGHT, separator, DOUBLE, EMPTY, HEAVY, separatorCharacter, separatorDouble, separatorEmpty, separatorHSelector, separatorHeavy, separatorLight, separatorStyled, separatorVSelector
#include "ftxui/dom/node.hpp"      // for Node
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/color.hpp"     // for Color
#include "ftxui/screen/pixel.hpp"     // for Pixel
#include "ftxui/screen/screen.hpp"    // for Pixel, Screen

namespace ftxui {

namespace {
using Charset = std::array<std::string, 2>;  // NOLINT
using Charsets = std::array<Charset, 6>;     // NOLINT
// NOLINTNEXTLINE
const Charsets charsets = {
    Charset{"│", "─"},  // LIGHT
    Charset{"╏", "╍"},  // DASHED
    Charset{"┃", "━"},  // HEAVY
    Charset{"║", "═"},  // DOUBLE
    Charset{"│", "─"},  // ROUNDED
    Charset{" ", " "},  // EMPTY
};

class Separator : public Node {
 public:
  explicit Separator(std::string value) : value_(std::move(value)) {}

  void ComputeRequirement() override {
    requirement_.min_x = 1;
    requirement_.min_y = 1;
  }

  void Render(Screen& screen) override {
    for (int y = box_.y_min; y <= box_.y_max; ++y) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        Pixel& pixel = screen.PixelAt(x, y);
        pixel.character = value_;
        pixel.automerge = true;
      }
    }
  }

  std::string value_;
};

class SeparatorAuto : public Node {
 public:
  explicit SeparatorAuto(BorderStyle style) : style_(style) {}

  void ComputeRequirement() override {
    requirement_.min_x = 1;
    requirement_.min_y = 1;
  }

  void Render(Screen& screen) override {
    const bool is_column = (box_.x_max == box_.x_min);
    const bool is_line = (box_.y_min == box_.y_max);

    const std::string c =
        charsets[style_][int(is_line && !is_column)];  // NOLINT

    for (int y = box_.y_min; y <= box_.y_max; ++y) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        Pixel& pixel = screen.PixelAt(x, y);
        pixel.character = c;
        pixel.automerge = true;
      }
    }
  }

  BorderStyle style_;
};

class SeparatorWithPixel : public SeparatorAuto {
 public:
  explicit SeparatorWithPixel(Pixel pixel)
      : SeparatorAuto(LIGHT), pixel_(std::move(pixel)) {
    pixel_.automerge = true;
  }
  void Render(Screen& screen) override {
    for (int y = box_.y_min; y <= box_.y_max; ++y) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        screen.PixelAt(x, y) = pixel_;
      }
    }
  }

 private:
  Pixel pixel_;
};
}  // namespace

/// @brief 在兩個元素之間繪製垂直或水平分隔線。
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// 在兩個元素之間添加視覺分隔。
///
/// ### 範例
///
/// ```cpp
/// // 使用 'border' 作為函式...
/// Element document = vbox({
///   text("up"),
///   separator(),
///   text("down"),
/// });
/// ```
///
/// ### 輸出
///
/// ```bash
/// up
/// ────
/// down
/// ```
Element separator() {
  return std::make_shared<SeparatorAuto>(LIGHT);
}

/// @brief 在兩個元素之間繪製垂直或水平分隔線。
/// @param style 分隔線的樣式。
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// 在兩個元素之間添加視覺分隔。
///
/// ### 範例
///
/// ```cpp
/// // 使用 'border' 作為函式...
/// Element document = vbox({
///   text("up"),
///   separatorStyled(DOUBLE),
///   text("down"),
/// });
/// ```
///
/// ### 輸出
///
/// ```bash
/// up
/// ════
/// down
/// ```
Element separatorStyled(BorderStyle style) {
  return std::make_shared<SeparatorAuto>(style);
}

/// @brief 使用 LIGHT 樣式在兩個元素之間繪製垂直或水平分隔線。
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// 在兩個元素之間添加視覺分隔。
///
/// ### 範例
///
/// ```cpp
/// // 使用 'border' 作為函式...
/// Element document = vbox({
///   text("up"),
///   separatorLight(),
///   text("down"),
/// });
/// ```
///
/// ### 輸出
///
/// ```bash
/// up
/// ────
/// down
/// ```
Element separatorLight() {
  return std::make_shared<SeparatorAuto>(LIGHT);
}

/// @brief 使用 DASHED 樣式在兩個元素之間繪製垂直或水平分隔線。
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// 在兩個元素之間添加視覺分隔。
///
/// ### 範例
///
/// ```cpp
/// // 使用 'border' 作為函式...
/// Element document = vbox({
///   text("up"),
///   separatorLight(),
///   text("down"),
/// });
/// ```
///
/// ### 輸出
///
/// ```bash
/// up
/// ╍╍╍╍
/// down
/// ```
Element separatorDashed() {
  return std::make_shared<SeparatorAuto>(DASHED);
}

/// @brief 使用 HEAVY 樣式在兩個元素之間繪製垂直或水平分隔線。
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// 在兩個元素之間添加視覺分隔。
///
/// ### 範例
///
/// ```cpp
/// // 使用 'border' 作為函式...
/// Element document = vbox({
///   text("up"),
///   separatorHeavy(),
///   text("down"),
/// });
/// ```
///
/// ### 輸出
///
/// ```bash
/// up
/// ━━━━
/// down
/// ```
Element separatorHeavy() {
  return std::make_shared<SeparatorAuto>(HEAVY);
}

/// @brief 使用 DOUBLE 樣式在兩個元素之間繪製垂直或水平分隔線。
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// 在兩個元素之間添加視覺分隔。
///
/// ### 範例
///
/// ```cpp
/// // 使用 'border' 作為函式...
/// Element document = vbox({
///   text("up"),
///   separatorDouble(),
///   text("down"),
/// });
/// ```
///
/// ### 輸出
///
/// ```bash
/// up
/// ════
/// down
/// ```
Element separatorDouble() {
  return std::make_shared<SeparatorAuto>(DOUBLE);
}

/// @brief 使用 EMPTY 樣式在兩個元素之間繪製垂直或水平分隔線。
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// 在兩個元素之間添加視覺分隔。
///
/// ### 範例
///
/// ```cpp
/// // 使用 'border' 作為函式...
/// Element document = vbox({
///   text("up"),
///   separator(),
///   text("down"),
/// });
/// ```
///
/// ### 輸出
///
/// ```bash
/// up
///
/// down
/// ```
Element separatorEmpty() {
  return std::make_shared<SeparatorAuto>(EMPTY);
}

/// @brief 在兩個元素之間繪製垂直或水平分隔線。
/// @param value 用於填充分隔線區域的字元。
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// 在兩個元素之間添加視覺分隔。
///
/// ### 範例
///
/// ```cpp
/// // 使用 'border' 作為函式...
/// Element document = vbox({
///   text("up"),
///   separator(),
///   text("down"),
/// });
/// ```
///
/// ### 輸出
///
/// ```bash
/// up
/// ────
/// down
/// ```
Element separatorCharacter(std::string value) {
  return std::make_shared<Separator>(std::move(value));
}

/// @brief 在兩個元素之間繪製一個由給定像素填充的分隔線。
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorHeavy
/// @see separatorDouble
/// @see separatorStyled
///
/// ### 範例
///
/// ```cpp
/// Pixel empty;
/// Element document = vbox({
///   text("Up"),
///   separator(empty),
///   text("Down"),
/// })
/// ```
///
/// ### 輸出
///
/// ```bash
/// Up
///
/// Down
/// ```
Element separator(Pixel pixel) {
  return std::make_shared<SeparatorWithPixel>(std::move(pixel));
}

/// @brief 繪製一個水平條，其中左右兩側之間的區域以不同顏色顯示。
/// @param left 活躍區域的左限。
/// @param right 活躍區域的右限。
/// @param selected_color 選定區域的顏色。
/// @param unselected_color 未選定區域的顏色。
///
/// ### 範例
///
/// ```cpp
/// Element document = separatorHSelector(2,5, Color::White, Color::Blue);
/// ```
Element separatorHSelector(float left,
                           float right,
                           Color unselected_color,
                           Color selected_color) {
  class Impl : public Node {
   public:
    Impl(float left, float right, Color selected_color, Color unselected_color)
        : left_(left),
          right_(right),
          unselected_color_(unselected_color),
          selected_color_(selected_color) {}
    void ComputeRequirement() override {
      requirement_.min_x = 1;
      requirement_.min_y = 1;
    }

    void Render(Screen& screen) override {
      if (box_.y_max < box_.y_min) {
        return;
      }

      // This are the two location with an empty demi-cell.
      int demi_cell_left = int(left_ * 2.F - 1.F);    // NOLINT
      int demi_cell_right = int(right_ * 2.F + 2.F);  // NOLINT

      const int y = box_.y_min;
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        Pixel& pixel = screen.PixelAt(x, y);

        const int a = (x - box_.x_min) * 2;
        const int b = a + 1;
        const bool a_empty = demi_cell_left == a || demi_cell_right == a;
        const bool b_empty = demi_cell_left == b || demi_cell_right == b;

        if (!a_empty && !b_empty) {
          pixel.character = "─";
          pixel.automerge = true;
        } else {
          pixel.character = a_empty ? "╶" : "╴";  // NOLINT
          pixel.automerge = false;
        }

        if (demi_cell_left <= a && b <= demi_cell_right) {
          pixel.foreground_color = selected_color_;
        } else {
          pixel.foreground_color = unselected_color_;
        }
      }
    }

    float left_;
    float right_;
    Color unselected_color_;
    Color selected_color_;
  };
  return std::make_shared<Impl>(left, right, unselected_color, selected_color);
}

/// @brief 繪製一個垂直條，其中上方/下方之間的區域以不同顏色顯示。
/// @param up 活躍區域的上限。
/// @param down 活躍區域的下限。
/// @param selected_color 選定區域的顏色。
/// @param unselected_color 未選定區域的顏色。
///
/// ### 範例
///
/// ```cpp
/// Element document = separatorHSelector(2,5, Color::White, Color::Blue);
/// ```
Element separatorVSelector(float up,
                           float down,
                           Color unselected_color,
                           Color selected_color) {
  class Impl : public Node {
   public:
    Impl(float up, float down, Color unselected_color, Color selected_color)
        : up_(up),
          down_(down),
          unselected_color_(unselected_color),
          selected_color_(selected_color) {}
    void ComputeRequirement() override {
      requirement_.min_x = 1;
      requirement_.min_y = 1;
    }

    void Render(Screen& screen) override {
      if (box_.x_max < box_.x_min) {
        return;
      }

      // This are the two location with an empty demi-cell.
      const int demi_cell_up = int(up_ * 2 - 1);
      const int demi_cell_down = int(down_ * 2 + 2);

      const int x = box_.x_min;
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        Pixel& pixel = screen.PixelAt(x, y);

        const int a = (y - box_.y_min) * 2;
        const int b = a + 1;
        const bool a_empty = demi_cell_up == a || demi_cell_down == a;
        const bool b_empty = demi_cell_up == b || demi_cell_down == b;

        if (!a_empty && !b_empty) {
          pixel.character = "│";
          pixel.automerge = true;
        } else {
          pixel.character = a_empty ? "╷" : "╵";  // NOLINT
          pixel.automerge = false;
        }

        if (demi_cell_up <= a && b <= demi_cell_down) {
          pixel.foreground_color = selected_color_;
        } else {
          pixel.foreground_color = unselected_color_;
        }
      }
    }

    float up_;
    float down_;
    Color unselected_color_;
    Color selected_color_;
  };
  return std::make_shared<Impl>(up, down, unselected_color, selected_color);
}

}  // namespace ftxui