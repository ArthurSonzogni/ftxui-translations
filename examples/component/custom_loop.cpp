// Copyright 2020 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに従います。
#include <stdlib.h>                   // for EXIT_SUCCESS
#include <chrono>                     // for milliseconds
#include <ftxui/component/event.hpp>  // for Event
#include <ftxui/component/mouse.hpp>  // for ftxui
#include <ftxui/dom/elements.hpp>  // for text, separator, Element, operator|, vbox, border
#include <memory>                  // for allocator, shared_ptr
#include <string>                  // for operator+, to_string
#include <thread>                  // for sleep_for

#include "ftxui/component/component.hpp"  // for CatchEvent, Renderer, operator|=
#include "ftxui/component/loop.hpp"       // for Loop
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive

int main() {
  using namespace ftxui;
  auto screen = ScreenInteractive::FitComponent();

  // 描画されたフレーム数と処理されたイベント数を数えるコンポーネントを作成します。
  int custom_loop_count = 0;
  int frame_count = 0;
  int event_count = 0;
  auto component = Renderer([&] {
    frame_count++;
    return vbox({
               text("これはカスタムftxui::Loopの使用例です。これは、"),
               text("毎秒100回のイテレーションで実行されます。FTXUIイベントは、"),
               text("イテレーションごとに一度すべて処理され、新しいフレームが"),
               text("必要に応じてレンダリングされます"),
               separator(),
               text("ftxui event count: " + std::to_string(event_count)),
               text("ftxui frame count: " + std::to_string(frame_count)),
               text("Custom loop count: " + std::to_string(custom_loop_count)),
           }) |
           border;
  });

  component |= CatchEvent([&](Event) -> bool {
    event_count++;
    return false;
  });

  Loop loop(&screen, component);

  while (!loop.HasQuitted()) {
    custom_loop_count++;
    loop.RunOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return EXIT_SUCCESS;
}