// 版权所有 2022 Arthur Sonzogni。保留所有权利。
// 此源代码的使用受 MIT 许可证的管辖，该许可证可在以下文件中找到：
// LICENSE 文件。
#include <memory>  // 用于 allocator, __shared_ptr_access, shared_ptr
#include <string>  // 用于 to_string, operator+

#include "ftxui/component/captured_mouse.hpp"  // 用于 ftxui
#include "ftxui/component/component.hpp"       // 用于 Button, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // 用于 ComponentBase
#include "ftxui/component/component_options.hpp"   // 用于 ButtonOption
#include "ftxui/component/screen_interactive.hpp"  // 用于 ScreenInteractive
#include "ftxui/dom/elements.hpp"  // 用于 operator|, text, Element, hbox, separator, size, vbox, border, frame, vscroll_indicator, HEIGHT, LESS_THAN
#include "ftxui/screen/color.hpp"  // 用于 Color, Color::Default, Color::GrayDark, Color::White

using namespace ftxui;

int main() {
  int counter = 0;
  auto on_click = [&] { counter++; };

  auto style = ButtonOption::Animated(Color::Default, Color::GrayDark,
                                      Color::Default, Color::White);

  auto container = Container::Vertical({});
  for (int i = 0; i < 30; ++i) {
    auto button = Button("Button " + std::to_string(i), on_click, style);
    container->Add(button);
  }

  auto renderer = Renderer(container, [&] {
    return vbox({
               hbox({
                   text("Counter:"),
                   text(std::to_string(counter)),
               }),
               separator(),
               container->Render() | vscroll_indicator | frame |
                   size(HEIGHT, LESS_THAN, 20),
           }) |
           border;
  });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(renderer);

  return 0;
}
