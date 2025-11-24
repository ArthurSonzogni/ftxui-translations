// 版權所有 2021 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，該條款可在 LICENSE 檔案中找到。
#include <memory>  // for allocator, shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, separator, Element, operator|, vbox, border

using namespace ftxui;

// 本範例展示如何將多個元件組合成一個，並保持它們的互動性。
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

  // Renderer 使用新的渲染函數裝飾其子元件。子元件對事件的反應方式保持不變。
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

  // Container 將元件分組在一起。要渲染 Container::Horizontal，
  // 它會將其子元件並排渲染。它保持它們的互動性，並提供使用箭頭鍵在元件之間導航的邏輯。
  auto composition = Container::Horizontal({leftpane, rightpane});

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(composition);
  return 0;
}

// 感謝 Chris Morgan 提供此範例！