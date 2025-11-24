// Copyright 2021 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSE ファイルにあるMITライセンスによって管理されています。
#include <memory>  // for allocator, shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, separator, Element, operator|, vbox, border

using namespace ftxui;

// 複数のコンポーネントを1つにまとめ、それらの対話性を維持する方法の例。
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

  // Rendererは、その子を新しいレンダリング関数で装飾します。子がイベントに反応する方法は維持されます。
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

  // コンテナはコンポーネントをグループ化します。Container::Horizontalをレンダリングするには、
  // その子を横並びにレンダリングします。それらの対話性を維持し、矢印キーを使用してコンポーネント間を移動するロジックを提供します。
  auto composition = Container::Horizontal({leftpane, rightpane});

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(composition);
  return 0;
}

// この例を提供してくれたChris Morganに感謝します！