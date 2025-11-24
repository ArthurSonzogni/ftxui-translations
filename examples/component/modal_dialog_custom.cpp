// Copyright 2020 Arthur Sonzogni. 無断転載を禁じます。
// このソースコードの使用は、以下に記載されているMITライセンスに準拠します。
// LICENSEファイル。
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

  // 2つのレイヤーがあります。1つはdepth = 0、もう1つはdepth = 1のモーダルウィンドウです。
  int depth = 0;

  // FTXUIの現在の評価。
  std::string rating = "3/5 stars";

  // depth=0には2つのボタンがあります。1つはFTXUIの評価用、もう1つは終了用です。
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

  // depth=1には、「モーダル」ウィンドウがあります。
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
