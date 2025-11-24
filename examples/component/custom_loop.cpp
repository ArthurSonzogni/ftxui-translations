// 版權所有 2020 Arthur Sonzogni。保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，該條款可在以下位置找到：
// LICENSE 文件。
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

  // 創建一個組件來計算繪製的幀數和處理的事件數。
  int custom_loop_count = 0;
  int frame_count = 0;
  int event_count = 0;
  auto component = Renderer([&] {
    frame_count++;
    return vbox({
               text("這演示了使用自定義 ftxui::Loop。它 "),
               text("以每秒 100 次迭代的速度運行。FTXUI 事件 "),
               text("每次迭代處理一次，並根據需要渲染新幀 "),
               text("根據需要渲染"),
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
