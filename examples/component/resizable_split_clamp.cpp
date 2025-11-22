// 版权所有 2025 Arthur Sonzogni。保留所有权利。
// 此源代码的使用受 MIT 许可证的约束，该许可证可在
// LICENSE 文件中找到。
#include <memory>  // for shared_ptr, allocator, __shared_ptr_access

#include "ftxui/component/component.hpp"  // for Renderer, ResizableSplitBottom, ResizableSplitLeft, ResizableSplitRight, ResizableSplitTop
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for Element, operator|, text, center, border

using namespace ftxui;

int main() {
  auto screen = ScreenInteractive::Fullscreen();

  // 状态：
  int size = 40;
  int size_min = 10;
  int size_max = 80;

  // 渲染器：
  auto split = ResizableSplit({
      .main = Renderer([] { return text("Left") | center; }),
      .back = Renderer([] { return text("Right") | center; }),
      .direction = Direction::Left,
      .main_size = &size,
      .min = &size_min,
      .max = &size_max,
  });

  auto renderer = Renderer(split, [&] {
    return window(text("Drag the separator with the mouse"),
                  vbox({
                      text("Min:  " + std::to_string(size_min)),
                      text("Max:  " + std::to_string(size_max)),
                      text("Size: " + std::to_string(size)),
                      separator(),
                      split->Render() | flex,
                  }));
  });

  screen.Loop(renderer);
}
