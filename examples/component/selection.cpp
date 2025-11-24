// Copyright 2020 Arthur Sonzogni. 無断転載を禁じます。
// このソースコードの使用は、以下に記載されているMITライセンスに準拠します。
// LICENSEファイル。
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
      text("FTXUI: ユーザーインターフェースを構築するための強力なライブラリ。"),
      text("豊富なコンポーネントセットと宣言的なスタイルをお楽しみください。"),
      text("最小限の労力で美しく応答性の高いUIを作成します。"),
      text("コミュニティに参加して、FTXUIの力を体験してください。"),
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

  // コンポーネント:
  auto renderer = Renderer(quit, [&] {
    return vbox({
        text("選択変更: " + std::to_string(selection_change_counter) +
             " times"),
        text("現在選択中: "),
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
        window(text("異なるスタイルのグリッド分割"),
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
