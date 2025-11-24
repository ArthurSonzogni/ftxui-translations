// Copyright 2020 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに従います。
#include <ftxui/dom/elements.hpp>  // for text, operator|, dim, Fit, hbox, Element
#include <ftxui/screen/screen.hpp>  // for Full, Screen
#include <memory>                   // for allocator

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui

int main() {
  using namespace ftxui;
  auto document =  //
      hbox({
          text("This text is "),
          text("dim") | dim,
          text(". Do you like it?"),
      });
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}
