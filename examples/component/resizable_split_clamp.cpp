// Copyright 2025 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに準拠しています。
#include <memory>  // for shared_ptr, allocator, __shared_ptr_access

#include "ftxui/component/component.hpp"  // for Renderer, ResizableSplitBottom, ResizableSplitLeft, ResizableSplitRight, ResizableSplitTop
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for Element, operator|, text, center, border

using namespace ftxui;

int main() {
  auto screen = ScreenInteractive::Fullscreen();

  // State:
  int size = 40;
  int size_min = 10;
  int size_max = 80;

  // Renderers:
  auto split = ResizableSplit({
      .main = Renderer([] { return text("Left") | center; }),
      .back = Renderer([] { return text("Right") | center; }),
      .direction = Direction::Left,
      .main_size = &size,
      .min = &size_min,
      .max = &size_max,
  });

  auto renderer = Renderer(split, [&] {
    return window(text("セパレータをマウスでドラッグしてください"),
                  vbox({
                      text("最小:  " + std::to_string(size_min)),
                      text("最大:  " + std::to_string(size_max)),
                      text("サイズ: " + std::to_string(size)),
                      separator(),
                      split->Render() | flex,
                  }));
  });

  screen.Loop(renderer);
}