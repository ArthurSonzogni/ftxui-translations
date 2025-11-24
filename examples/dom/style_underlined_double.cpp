// 版權所有 2020 Arthur Sonzogni。保留所有權利。
// 此原始碼受 MIT 授權條款約束，詳情請參閱
// LICENSE 文件。
#include <ftxui/dom/elements.hpp>  // for text, operator|, underlinedDouble, Fit, hbox, Element
#include <ftxui/screen/screen.hpp>  // for Full, Screen
#include <memory>                   // for allocator

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui

int main() {
  using namespace ftxui;
  auto document =  //
      hbox({
          text("This text is "),
          text("underlinedDouble") | underlinedDouble,
          text(". Do you like it?"),
      });
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}