// 版權所有 2020 Arthur Sonzogni。保留所有權利。
// 本原始碼受 MIT 授權條款約束，詳情請參閱 LICENSE 文件。
#include <stdlib.h>                // for EXIT_SUCCESS
#include <ftxui/dom/elements.hpp>  // for text, operator|, vbox, border, Element, Fit, hbox
#include <ftxui/screen/screen.hpp>  // for Full, Screen
#include <memory>                   // for allocator

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui

int main() {
  using namespace ftxui;
  auto document =  //
      hbox({
          vbox({
              text("Line 1"),
              text("Line 2"),
              text("Line 3"),
          }) | border,

          vbox({
              text("Line 4"),
              text("Line 5"),
              text("Line 6"),
          }) | border,

          vbox({
              text("Line 7"),
              text("Line 8"),
              text("Line 9"),
          }) | border,
      });
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();
  return EXIT_SUCCESS;
}
