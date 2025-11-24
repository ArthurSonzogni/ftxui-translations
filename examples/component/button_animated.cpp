// 版權所有 2020 Arthur Sonzogni。保留所有權利。
// 本原始碼的使用受 MIT 許可證的約束，該許可證可在 LICENSE 文件中找到。
#include <memory>  // 用於 shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string

#include "ftxui/component/captured_mouse.hpp"  // 用於 ftxui
#include "ftxui/component/component.hpp"  // 用於 Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // 用於 ComponentBase
#include "ftxui/component/component_options.hpp"   // 用於 ButtonOption
#include "ftxui/component/screen_interactive.hpp"  // 用於 ScreenInteractive
#include "ftxui/dom/elements.hpp"  // 用於 gauge, separator, text, vbox, operator|, Element, border
#include "ftxui/screen/color.hpp"  // 用於 Color, Color::Blue, Color::Green, Color::Red

using namespace ftxui;

int main() {
  int value = 50;

  // 元件樹。這定義了如何使用鍵盤導航。
  auto buttons = Container::Horizontal({
      Button(
          "Decrease", [&] { value--; }, ButtonOption::Animated(Color::Red)),
      Button(
          "Reset", [&] { value = 50; }, ButtonOption::Animated(Color::Green)),
      Button(
          "Increase", [&] { value++; }, ButtonOption::Animated(Color::Blue)),
  });

  // 修改它們在螢幕上的渲染方式：
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
