// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに準拠しています。
// text, operator|, blink, Fit, hbox, Element のため
#include <ftxui/dom/elements.hpp>  // for text, operator|, blink, Fit, hbox, Element
// Full, Screen のため
#include <ftxui/screen/screen.hpp>  // for Full, Screen
// アロケータのため
#include <memory>                   // for allocator

// Render のため
#include "ftxui/dom/node.hpp"      // for Render
// ftxui のため
#include "ftxui/screen/color.hpp"  // for ftxui

int main() {
  using namespace ftxui;
  auto document =  //
      hbox({
          text("このテキストは"),
          text("点滅します") | blink,
          text("。気に入りましたか？"),
      });
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}
