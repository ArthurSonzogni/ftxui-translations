// Copyright 2025 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、以下に記載されているMITライセンスに準拠します。
// LICENSEファイル。
#include <ftxui/dom/elements.hpp>  // for text, operator|, inverted, Fit, hbox, Element
#include <ftxui/screen/screen.hpp>  // for Full, Screen
#include <memory>                   // for allocator

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui

int main() {
  using namespace ftxui;
  auto document = hbox({
      text("This text is "),
      text("italic") | italic,
      text(". Do you like it?"),
  });
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}
