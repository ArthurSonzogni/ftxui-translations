// 版权所有 2021 Arthur Sonzogni. 保留所有权利。
// 本源代码的使用受 MIT 许可证的约束，该许可证可在以下位置找到：
// LICENSE 文件。
#include <memory>  // for allocator, shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, separator, Element, operator|, vbox, border

using namespace ftxui;

// 一个示例，展示如何将多个组件组合成一个并保持它们的
// 交互性。
int main() {
  auto left_count = 0;
  auto right_count = 0;

  auto left_buttons = Container::Horizontal({
      Button("Decrease", [&] { left_count--; }),
      Button("Increase", [&] { left_count++; }),
  });

  auto right_buttons = Container::Horizontal({
      Button("Decrease", [&] { right_count--; }),
      Button("Increase", [&] { right_count++; }),
  });

  // 渲染器用新的渲染函数装饰其子组件。子组件对事件的反应方式
  // 保持不变。
  auto leftpane = Renderer(left_buttons, [&] {
    return vbox({
               text("This is the left control"),
               separator(),
               text("Left button count: " + std::to_string(left_count)),
               left_buttons->Render(),
           }) |
           border;
  });

  auto rightpane = Renderer(right_buttons, [&] {
    return vbox({
               text("This is the right control"),
               separator(),
               text("Right button count: " + std::to_string(right_count)),
               right_buttons->Render(),
           }) |
           border;
  });

  // 容器将组件组合在一起。要渲染一个 Container::Horizontal，
  // 它并排渲染其子组件。它保持它们的交互性并
  // 提供使用箭头键从一个导航到另一个的逻辑。
  auto composition = Container::Horizontal({leftpane, rightpane});

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(composition);
  return 0;
}

// 感谢 Chris Morgan 提供此示例！
