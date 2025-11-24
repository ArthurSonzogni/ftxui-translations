// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに準拠します。
// LICENSEファイルに記載されているMITライセンスの条件に基づき、本ソースコードを使用することができます。
#include <memory>   // for make_shared
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Element, Decorator, bgcolor, color
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/box.hpp"          // for Box
#include "ftxui/screen/color.hpp"        // for Color
#include "ftxui/screen/screen.hpp"       // for Pixel, Screen

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
          screen.PixelAt(x, y).background_color = color_;
        }
      }
    } else {
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        for (int x = box_.x_min; x <= box_.x_max; ++x) {
          Color& color = screen.PixelAt(x, y).background_color;
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
          screen.PixelAt(x, y).foreground_color = color_;
        }
      }
    } else {
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        for (int x = box_.x_min; x <= box_.x_max; ++x) {
          Color& color = screen.PixelAt(x, y).foreground_color;
          color = Color::Blend(color, color_);
        }
      }
    }
    NodeDecorator::Render(screen);
  }

  Color color_;
};

}  // namespace

/// @brief 要素の前景色を設定します。
/// @param color 出力要素の色。
/// @param child 入力要素。
/// @return 色付けされた出力要素。
/// @ingroup dom
///
/// ### 例
///
/// ```cpp
/// Element document = color(Color::Green, text("Success")),
/// ```
Element color(Color color, Element child) {
  return std::make_shared<FgColor>(std::move(child), color);
}

/// @brief 要素の背景色を設定します。
/// @param color 出力要素の色。
/// @param child 入力要素。
/// @return 色付けされた出力要素。
/// @ingroup dom
///
/// ### 例
///
/// ```cpp
/// Element document = bgcolor(Color::Green, text("Success")),
/// ```
Element bgcolor(Color color, Element child) {
  return std::make_shared<BgColor>(std::move(child), color);
}

/// @brief 前景色を使用して装飾します。
/// @param c 適用する前景色。
/// @return 色を適用するデコレータ。
/// @ingroup dom
///
/// ### 例
///
/// ```cpp
/// Element document = text("red") | color(Color::Red);
/// ```
Decorator color(Color c) {
  return [c](Element child) { return color(c, std::move(child)); };
}

/// @brief 背景色を使用して装飾します。
/// @param color 適用する背景色。
/// @return 色を適用するデコレータ。
/// @ingroup dom
///
/// ### 例
///
/// ```cpp
/// Element document = text("red") | bgcolor(Color::Red);
/// ```
Decorator bgcolor(Color color) {
  return [color](Element child) { return bgcolor(color, std::move(child)); };
}

}  // namespace ftxui