// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 許可證約束，該許可證可在
// LICENSE 文件中找到。
#include <memory>  // for shared_ptr, allocator, __shared_ptr_access

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Renderer, Button, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, Element, text, bold, border, center, color
#include "ftxui/screen/color.hpp"  // for Color, Color::Red

int main() {
  using namespace ftxui;
  auto screen = ScreenInteractive::FitComponent();

  // Renderer() 是一個組件，它使用 lambda 函數作為參數來
  // 渲染自己。
  // 1. 可聚焦渲染器的範例：
  auto renderer_focusable = Renderer([](bool focused) {
    if (focused) {
      return text("FOCUSABLE RENDERER()") | center | bold | border;
    } else {
      return text(" Focusable renderer() ") | center | border;
    }
  });

  // 2. 不可聚焦渲染器的範例。
  auto renderer_non_focusable = Renderer([&] {
    return text("~~~~~ Non Focusable renderer() ~~~~~");  //
  });

  // 3. Renderer 可以包裝其他組件以重新定義它們的 Render() 函數。
  auto button = Button("Wrapped quit button", screen.ExitLoopClosure());
  auto renderer_wrap = Renderer(button, [&] {
    if (button->Focused()) {
      return button->Render() | bold | color(Color::Red);
    } else {
      return button->Render();
    }
  });

  // 讓我們渲染所有組件：
  screen.Loop(Container::Vertical({
      renderer_focusable,
      renderer_non_focusable,
      renderer_wrap,
  }));
}
