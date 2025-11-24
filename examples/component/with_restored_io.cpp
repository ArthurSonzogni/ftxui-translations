// Copyright 2022 Arthur Sonzogni. 無断複写・転載を禁じます。
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに従います。
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

  // このボタンを押すと、「screen.WithRestoredIO」はターミナルフックを一時的に
  // アンインストールし、提供されたコールバック関数を実行します。これにより、
  // アプリケーションを非対話モードで実行できます。
  auto btn_run = Button("Execute with restored IO", screen.WithRestoredIO([] {
    std::cout << "これはstdin/stdoutを使用する子プログラムです。" << std::endl;
    for (int i = 0; i < 10; ++i) {
      std::cout << "10個の文字列を入力してください (" << i << "/10)" << std::flush;
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
        "このボタンをクリックすると、ScreenInteractiveが一時停止され、"
        "stdin/stdoutへのアクセスが一時的に回復され、関数が実行されます。");
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
