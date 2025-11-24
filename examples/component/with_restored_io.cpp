// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <cstdlib>   // for system, EXIT_SUCCESS
#include <iostream>  // for operator<<, basic_ostream, basic_ostream::operator<<, cout, endl, flush, ostream, basic_ostream<>::__ostream_type, cin
#include <memory>    // for shared_ptr, __shared_ptr_access, allocator
#include <string>    // for getline, string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, filler, Element, borderEmpty, hbox, size, paragraph, vbox, LESS_THAN, border, center, HEIGHT, WIDTH

int main() {
  using namespace ftxui;

  auto screen = ScreenInteractive::Fullscreen();

  // 當按下此按鈕時，"screen.WithRestoredIO" 將暫時卸載終端鉤子並執行所提供的回調函數。
  // 這允許應用程式以非互動模式運行。
  auto btn_run = Button("Execute with restored IO", screen.WithRestoredIO([] {
    std::cout << "This is a child program using stdin/stdout." << std::endl;
    for (int i = 0; i < 10; ++i) {
      std::cout << "Please enter 10 strings (" << i << "/10)" << std::flush;
      std::string input;
      std::getline(std::cin, input);
    }
  }));

  auto btn_quit = Button("Quit", screen.ExitLoopClosure());

  auto layout = Container::Horizontal({
      btn_run,
      btn_quit,
  });

  auto renderer = Renderer(layout, [&] {
    auto explanation = paragraph(
        "點擊此按鈕後，ScreenInteractive 將會被暫停，並暫時恢復對 stdin/stdout 的訪問，以執行一個函數。");
    auto element = vbox({
        explanation | borderEmpty,
        hbox({
            btn_run->Render(),
            filler(),
            btn_quit->Render(),
        }),
    });

    element = element | borderEmpty | border | size(WIDTH, LESS_THAN, 80) |
              size(HEIGHT, LESS_THAN, 20) | center;
    return element;
  });

  screen.Loop(renderer);
  return EXIT_SUCCESS;
}
