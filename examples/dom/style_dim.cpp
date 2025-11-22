// 版权所有 2020 Arthur Sonzogni。保留所有权利。
// 本源代码的使用受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
#include <ftxui/dom/elements.hpp>  // 用于 text, operator|, dim, Fit, hbox, Element
#include <ftxui/screen/screen.hpp>  // 用于 Full, Screen
#include <memory>                   // 用于 allocator

#include "ftxui/dom/node.hpp"      // 用于 Render
#include "ftxui/screen/color.hpp"  // 用于 ftxui

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
