// Copyright 2024 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスによって管理されます。
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

/// @brief 要素の選択スタイルをリセットします。
/// @param child 入力要素。
/// @return 選択スタイルがリセットされた出力要素。
Element selectionStyleReset(Element child) {
  return std::make_shared<SelectionStyleReset>(std::move(child));
}

/// @brief 要素が選択されたときの背景色を設定します。
/// スタイルは既存のスタイルに重ねて適用されることに注意してください。
Decorator selectionBackgroundColor(Color foreground) {
  return selectionStyle([foreground](Pixel& pixel) {  //
    pixel.background_color = foreground;
  });
}

/// @brief 要素が選択されたときの描画色を設定します。
/// スタイルは既存のスタイルに重ねて適用されることに注意してください。
Decorator selectionForegroundColor(Color foreground) {
  return selectionStyle([foreground](Pixel& pixel) {  //
    pixel.foreground_color = foreground;
  });
}

/// @brief 要素が選択されたときの色を設定します。
/// @param foreground 適用する色。
/// スタイルは既存のスタイルに重ねて適用されることに注意してください。
Decorator selectionColor(Color foreground) {
  return selectionForegroundColor(foreground);
}

/// @brief 要素が選択されたときのスタイルを設定します。
/// @param style 適用するスタイル。
/// スタイルは既存のスタイルに重ねて適用されることに注意してください。
// NOLINTNEXTLINE
Decorator selectionStyle(std::function<void(Pixel&)> style) {
  return [style](Element child) -> Element {
    return std::make_shared<SelectionStyle>(std::move(child), style);
  };
}

}  // namespace ftxui