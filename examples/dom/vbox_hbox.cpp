// Copyright 2020 Arthur Sonzogni. 無断複写・転載を禁じます。
// このソースコードの使用は、
// LICENSEファイルにあるMITライセンスによって管理されています。
#include <stdio.h>                  // getcharのため
#include <ftxui/dom/elements.hpp>   // filler, text, hbox, vboxのため
#include <ftxui/screen/screen.hpp>  // Full, Screenのため
#include <memory>                   // アロケータのため

#include "ftxui/dom/node.hpp"      // Renderのため
#include "ftxui/screen/color.hpp"  // ftxuiのため

int main() {
  using namespace ftxui;
  auto document =  //
      vbox({
          hbox({
              text("north-west"),
              filler(),
              text("north-east"),
          }),
          filler(),
          hbox({
              filler(),
              text("center"),
              filler(),
          }),
          filler(),
          hbox({
              text("south-west"),
              filler(),
              text("south-east"),
          }),
      });
  auto screen = Screen::Create(Dimension::Full());
  Render(screen, document);
  screen.Print();
  getchar();

  return 0;
}
