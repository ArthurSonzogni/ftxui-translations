// 版权所有 2020 Arthur Sonzogni. 保留所有权利。
// 本源代码受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
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
