// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <memory>   // for make_shared
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Element, Decorator, bgcolor, color
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/box.hpp"          // for Box
#include "ftxui/screen/color.hpp"        // for Color
#include "ftxui/screen/screen.hpp"       // for Cell, Screen

namespace ftxui {

namespace {
class BgColor : public NodeDecorator {
 public:
  BgColor(Element child, Color color)
      : NodeDecorator(std::move(child)), color_(color) {}

  void Render(Screen& screen) override {
    if (color_.IsOpaque()) {
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        for (int x = box_.x_min; x <= box_.x_max; ++x) {
          screen.CellAt(x, y).background_color = color_;
        }
      }
    } else {
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        for (int x = box_.x_min; x <= box_.x_max; ++x) {
          Color& color = screen.CellAt(x, y).background_color;
          color = Color::Blend(color, color_);
        }
      }
    }
    NodeDecorator::Render(screen);
  }

  Color color_;
};

class FgColor : public NodeDecorator {
 public:
  FgColor(Element child, Color color)
      : NodeDecorator(std::move(child)), color_(color) {}

  void Render(Screen& screen) override {
    if (color_.IsOpaque()) {
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        for (int x = box_.x_min; x <= box_.x_max; ++x) {
          screen.CellAt(x, y).foreground_color = color_;
        }
      }
    } else {
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        for (int x = box_.x_min; x <= box_.x_max; ++x) {
          Color& color = screen.CellAt(x, y).foreground_color;
          color = Color::Blend(color, color_);
        }
      }
    }
    NodeDecorator::Render(screen);
  }

  Color color_;
};

}  // namespace

/// @brief 設定元素的前景顏色。
/// @param color 輸出元素的顏色。
/// @param child 輸入元素。
/// @return 著色的輸出元素。
/// @ingroup dom
///
/// ### 範例
///
/// ```cpp
/// Element document = color(Color::Green, text("Success")),
/// ```
Element color(Color color, Element child) {
  return std::make_shared<FgColor>(std::move(child), color);
}

/// @brief 設定元素的背景顏色。
/// @param color 輸出元素的顏色。
/// @param child 輸入元素。
/// @return 著色的輸出元素。
/// @ingroup dom
///
/// ### 範例
///
/// ```cpp
/// Element document = bgcolor(Color::Green, text("Success")),
/// ```
Element bgcolor(Color color, Element child) {
  return std::make_shared<BgColor>(std::move(child), color);
}

/// @brief 使用前景顏色進行裝飾。
/// @param c 要應用的前景顏色。
/// @return 應用顏色的裝飾器。
/// @ingroup dom
///
/// ### 範例
///
/// ```cpp
/// Element document = text("red") | color(Color::Red);
/// ```
Decorator color(Color c) {
  return [c](Element child) { return color(c, std::move(child)); };
}

/// @brief 使用背景顏色進行裝飾。
/// @param color 要應用的背景顏色。
/// @return 應用顏色的裝飾器。
/// @ingroup dom
///
/// ### 範例
///
/// ```cpp
/// Element document = text("red") | bgcolor(Color::Red);
/// ```
Decorator bgcolor(Color color) {
  return [color](Element child) { return bgcolor(color, std::move(child)); };
}

}  // namespace ftxui
