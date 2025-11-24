// 版權所有 2020 Arthur Sonzogni。保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，該條款可在
// LICENSE 檔案中找到。
#include <ftxui/dom/elements.hpp>  // for text, operator|, bold, Fit, hbox, Element
#include <ftxui/screen/screen.hpp>  // for Full, Screen
#include <memory>                   // for allocator

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui

int main() {
  using namespace ftxui;
  auto document =  //
      hbox({
          text("This text is an "),
          text("hyperlink") | hyperlink("https://www.google.com"),
          text(". Do you like it?"),
      });
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}