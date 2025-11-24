// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに従います。
#include <ftxui/dom/elements.hpp>  // text, operator|, bold, Fit, hbox, Element のために
#include <ftxui/screen/screen.hpp>  // Full, Screen のために
#include <memory>                   // アロケータのために

#include "ftxui/dom/node.hpp"      // Render のために
#include "ftxui/screen/color.hpp"  // ftxui のために

int main() {
  using namespace ftxui;
  auto document =  //
      hbox({
          text("このテキストは"),
          text("太字") | bold,
          text("です。気に入りましたか？"),
      });
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}
