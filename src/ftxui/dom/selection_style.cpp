// 版权所有 2024 Arthur Sonzogni. 保留所有权利。
// 本源代码的使用受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
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

/// @brief 重置元素的选中样式。
/// @param child 输入元素。
/// @return 重置选中样式的输出元素。
Element selectionStyleReset(Element child) {
  return std::make_shared<SelectionStyleReset>(std::move(child));
}

/// @brief 设置元素选中时的背景颜色。
/// 请注意，此样式是在现有样式之上应用的。
Decorator selectionBackgroundColor(Color foreground) {
  return selectionStyle([foreground](Pixel& pixel) {  //
    pixel.background_color = foreground;
  });
}

/// @brief 设置元素选中时的前景色。
/// 请注意，此样式是在现有样式之上应用的。
Decorator selectionForegroundColor(Color foreground) {
  return selectionStyle([foreground](Pixel& pixel) {  //
    pixel.foreground_color = foreground;
  });
}

/// @brief 设置元素选中时的颜色。
/// @param foreground 要应用的颜色。
/// 请注意，此样式是在现有样式之上应用的。
Decorator selectionColor(Color foreground) {
  return selectionForegroundColor(foreground);
}

/// @brief 设置元素选中时的样式。
/// @param style 要应用的样式。
/// 请注意，此样式是在现有样式之上应用的。
// NOLINTNEXTLINE
Decorator selectionStyle(std::function<void(Pixel&)> style) {
  return [style](Element child) -> Element {
    return std::make_shared<SelectionStyle>(std::move(child), style);
  };
}

}  // namespace ftxui
