// 版权所有 2020 Arthur Sonzogni。保留所有权利。
// 本源代码受 MIT 许可证的约束，可在
// LICENSE 文件中找到。
#include <string>  // 用于 operator+，to_string

#include "ftxui/component/captured_mouse.hpp"  // 用于 ftxui
#include "ftxui/component/component.hpp"  // 用于 Button, Vertical, Renderer, Horizontal, operator|
#include "ftxui/component/component_base.hpp"      // 用于 Component
#include "ftxui/component/component_options.hpp"   // 用于 ButtonOption
#include "ftxui/component/screen_interactive.hpp"  // 用于 ScreenInteractive
#include "ftxui/dom/elements.hpp"  // 用于 Element, separator, text, border
#include "ftxui/screen/color.hpp"  // 用于 Color, Color::Blue, Color::Green, Color::Red

using namespace ftxui;

int main() {
  int value = 0;
  auto action = [&] { value++; };
  auto action_renderer =
      Renderer([&] { return text("count = " + std::to_string(value)); });

  auto buttons =
      Container::Vertical({
          action_renderer,
          Renderer([] { return separator(); }),
          Container::Horizontal({
              Container::Vertical({
                  Button("Ascii 1", action, ButtonOption::Ascii()),
                  Button("Ascii 2", action, ButtonOption::Ascii()),
                  Button("Ascii 3", action, ButtonOption::Ascii()),
              }),
              Renderer([] { return separator(); }),
              Container::Vertical({
                  Button("Simple 1", action, ButtonOption::Simple()),
                  Button("Simple 2", action, ButtonOption::Simple()),
                  Button("Simple 3", action, ButtonOption::Simple()),
              }),
              Renderer([] { return separator(); }),
              Container::Vertical({
                  Button("Animated 1", action, ButtonOption::Animated()),
                  Button("Animated 2", action, ButtonOption::Animated()),
                  Button("Animated 3", action, ButtonOption::Animated()),
              }),
              Renderer([] { return separator(); }),
              Container::Vertical({
                  Button("Animated 4", action,
                         ButtonOption::Animated(Color::Red)),
                  Button("Animated 5", action,
                         ButtonOption::Animated(Color::Green)),
                  Button("Animated 6", action,
                         ButtonOption::Animated(Color::Blue)),
              }),
          }),
      }) |
      border;

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(buttons);
  return 0;
}
