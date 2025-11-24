// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <functional>  // for function
#include <memory>      // for make_shared
#include <utility>     // for move

#include "ftxui/dom/elements.hpp"  // for Element, Decorator, bgcolor, color
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/color.hpp"        // for Color
#include "ftxui/screen/pixel.hpp"        // for Pixel
#include "ftxui/screen/screen.hpp"       // for Screen

namespace ftxui {

namespace {

class SelectionStyleReset : public NodeDecorator {
 public:
  explicit SelectionStyleReset(Element child)
      : NodeDecorator(std::move(child)) {}

  void Render(Screen& screen) final {
    auto old_style = screen.GetSelectionStyle();
    screen.SetSelectionStyle([](Pixel&) {});
    NodeDecorator::Render(screen);
    screen.SetSelectionStyle(old_style);
  }
};

class SelectionStyle : public NodeDecorator {
 public:
  SelectionStyle(Element child, const std::function<void(Pixel&)>& style)
      : NodeDecorator(std::move(child)), style_(style) {}

  void Render(Screen& screen) final {
    auto old_style = screen.GetSelectionStyle();
    auto new_style = [&, old_style](Pixel& pixel) {
      old_style(pixel);
      style_(pixel);
    };
    screen.SetSelectionStyle(new_style);
    NodeDecorator::Render(screen);
    screen.SetSelectionStyle(old_style);
  }

  std::function<void(Pixel&)> style_;
};

}  // namespace

/// @brief 重設元素的選取樣式。
/// @param child 輸入元素。
/// @return 具有重設選取樣式的輸出元素。
Element selectionStyleReset(Element child) {
  return std::make_shared<SelectionStyleReset>(std::move(child));
}

/// @brief 設定元素被選取時的背景顏色。
/// 請注意，此樣式會應用在現有樣式之上。
Decorator selectionBackgroundColor(Color foreground) {
  return selectionStyle([foreground](Pixel& pixel) {  //
    pixel.background_color = foreground;
  });
}

/// @brief 設定元素被選取時的前景顏色。
/// 請注意，此樣式會應用在現有樣式之上。
Decorator selectionForegroundColor(Color foreground) {
  return selectionStyle([foreground](Pixel& pixel) {  //
    pixel.foreground_color = foreground;
  });
}

/// @brief 設定元素被選取時的顏色。
/// @param foreground 要套用的顏色。
/// 請注意，此樣式會應用在現有樣式之上。
Decorator selectionColor(Color foreground) {
  return selectionForegroundColor(foreground);
}

/// @brief 設定元素被選取時的樣式。
/// @param style 要套用的樣式。
/// 請注意，此樣式會應用在現有樣式之上。
// NOLINTNEXTLINE
Decorator selectionStyle(std::function<void(Pixel&)> style) {
  return [style](Element child) -> Element {
    return std::make_shared<SelectionStyle>(std::move(child), style);
  };
}

}  // namespace ftxui
