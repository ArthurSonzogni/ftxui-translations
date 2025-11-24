// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 此原始碼的使用受 MIT 授權條款約束，該條款可在 LICENSE 檔案中找到。
#include <string>  // for char_traits, operator+, string, basic_string

#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // for Ref

using namespace ftxui;

Element LoremIpsum() {
  return vbox({
      text("FTXUI：一個用於構建使用者介面的強大函式庫。"),
      text("享受豐富的組件和宣告式風格。"),
      text("以最少的工作量建立美觀且回應迅速的使用者介面。"),
      text("加入社群，體驗 FTXUI 的強大功能。"),
  });
}

int main() {
  auto screen = ScreenInteractive::TerminalOutput();

  auto quit =
      Button("Quit", screen.ExitLoopClosure(), ButtonOption::Animated());

  int selection_change_counter = 0;
  std::string selection_content = "";
  screen.SelectionChange([&] {
    selection_change_counter++;
    selection_content = screen.GetSelection();
  });

  // 組件:
  auto renderer = Renderer(quit, [&] {
    return vbox({
        text("選取已變更: " + std::to_string(selection_change_counter) +
             " 次"),
        text("目前選取: "),
        paragraph(selection_content) | vscroll_indicator | frame | border |
            size(HEIGHT, EQUAL, 10),
        window(text("水平分割"), hbox({
                                             LoremIpsum(),
                                             separator(),
                                             LoremIpsum(),
                                             separator(),
                                             LoremIpsum(),
                                         })),
        window(text("垂直分割"), vbox({
                                           LoremIpsum(),
                                           separator(),
                                           LoremIpsum(),
                                           separator(),
                                           LoremIpsum(),
                                       })),
        window(text("Grid split with different style"),
               vbox({
                   hbox({
                       LoremIpsum(),
                       separator(),
                       LoremIpsum()                                   //
                           | selectionBackgroundColor(Color::Yellow)  //
                           | selectionColor(Color::Black)             //
                           | selectionStyleReset,
                       separator(),
                       LoremIpsum() | selectionColor(Color::Blue),
                   }),
                   separator(),
                   hbox({
                       LoremIpsum() | selectionColor(Color::Red),
                       separator(),
                       LoremIpsum() | selectionStyle([](Pixel& pixel) {
                         pixel.underlined_double = true;
                       }),
                       separator(),
                       LoremIpsum(),
                   }),
               })),
        quit->Render(),
    });
  });

  screen.Loop(renderer);
}
