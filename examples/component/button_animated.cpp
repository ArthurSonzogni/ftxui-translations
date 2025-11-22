// 版权所有 2020 Arthur Sonzogni. 保留所有权利。
// 此源代码的使用受 MIT 许可的约束，MIT 许可可在
// LICENSE 文件中找到。
#include <memory>  // for shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/component_options.hpp"   // for ButtonOption
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for gauge, separator, text, vbox, operator|, Element, border
#include "ftxui/screen/color.hpp"  // for Color, Color::Blue, Color::Green, Color::Red

using namespace ftxui;

int main() {
  int value = 50;

  // 组件树。这定义了如何使用键盘进行导航。
  auto buttons = Container::Horizontal({
      Button(
          "Decrease", [&] { value--; }, ButtonOption::Animated(Color::Red)),
      Button(
          "Reset", [&] { value = 50; }, ButtonOption::Animated(Color::Green)),
      Button(
          "Increase", [&] { value++; }, ButtonOption::Animated(Color::Blue)),
  });

  // 修改它们在屏幕上的渲染方式：
  auto component = Renderer(buttons, [&] {
    return vbox({
        vbox({
            text("value = " + std::to_string(value)),
            separator(),
            gauge(value * 0.01f),
        }) | border,
        buttons->Render(),
    });
  });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(component);
  return 0;
}
