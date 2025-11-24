// Copyright 2020 Arthur Sonzogni. 著作權所有。
// 本原始碼的使用受 MIT 授權條款約束，該條款可在
// LICENSE 檔案中找到。
#include <memory>  // for allocator, shared_ptr, __shared_ptr_access
#include <string>  // for string, basic_string, char_traits, operator+
#include <vector>  // for vector

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Renderer, Horizontal, Tab
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, Element, filler, text, hbox, separator, center, vbox, bold, border, clear_under, dbox, size, GREATER_THAN, HEIGHT

int main() {
  using namespace ftxui;
  auto screen = ScreenInteractive::TerminalOutput();

  // 有兩個圖層。一個在 depth = 0，另一個是深度為 1 的模態視窗；
  int depth = 0;

  // FTXUI 的當前評分。
  std::string rating = "3/5 stars";

  // 在 depth=0 時，有兩個按鈕。一個用於評分 FTXUI，另一個用於退出。
  auto button_rate_ftxui = Button("Rate FTXUI", [&] { depth = 1; });
  auto button_quit = Button("Quit", screen.ExitLoopClosure());

  auto depth_0_container = Container::Horizontal({
      button_rate_ftxui,
      button_quit,
  });
  auto depth_0_renderer = Renderer(depth_0_container, [&] {
    return vbox({
               text("Modal dialog example"),
               separator(),
               text("☆☆☆ FTXUI:" + rating + " ☆☆☆") | bold,
               filler(),
               hbox({
                   button_rate_ftxui->Render(),
                   filler(),
                   button_quit->Render(),
               }),
           }) |
           border | size(HEIGHT, GREATER_THAN, 18) | center;
  });

  // 在 depth=1 時，即為「模態」視窗。
  std::vector<std::string> rating_labels = {
      "1/5 stars", "2/5 stars", "3/5 stars", "4/5 stars", "5/5 stars",
  };
  auto on_rating = [&](std::string new_rating) {
    rating = new_rating;
    depth = 0;
  };
  auto depth_1_container = Container::Horizontal({
      Button(&rating_labels[0], [&] { on_rating(rating_labels[0]); }),
      Button(&rating_labels[1], [&] { on_rating(rating_labels[1]); }),
      Button(&rating_labels[2], [&] { on_rating(rating_labels[2]); }),
      Button(&rating_labels[3], [&] { on_rating(rating_labels[3]); }),
      Button(&rating_labels[4], [&] { on_rating(rating_labels[4]); }),
  });

  auto depth_1_renderer = Renderer(depth_1_container, [&] {
    return vbox({
               text("Do you like FTXUI?"),
               separator(),
               hbox(depth_1_container->Render()),
           }) |
           border;
  });

  auto main_container = Container::Tab(
      {
          depth_0_renderer,
          depth_1_renderer,
      },
      &depth);

  auto main_renderer = Renderer(main_container, [&] {
    Element document = depth_0_renderer->Render();

    if (depth == 1) {
      document = dbox({
          document,
          depth_1_renderer->Render() | clear_under | center,
      });
    }
    return document;
  });

  screen.Loop(main_renderer);
  return 0;
}
