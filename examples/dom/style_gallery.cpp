// 版权所有 2020 Arthur Sonzogni. 保留所有权利。
// 本源代码的使用受 MIT 许可证的约束，该许可证可在
// LICENSE 文件中找到。
#include <ftxui/dom/elements.hpp>  // for text, operator|, Element, bgcolor, color, blink, bold, dim, inverted, underlined, Fit, hbox
#include <ftxui/screen/screen.hpp>  // for Full, Screen
#include <memory>                   // for allocator

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for Color, Color::Blue, ftxui

int main() {
  using namespace ftxui;
  // clang-format off
  auto document =
    hbox({
      text("normal")                                    , text(" ") ,
      text("bold")               | bold                 , text(" ") ,
      text("italic")             | italic               , text(" ") ,
      text("dim")                | dim                  , text(" ") ,
      text("inverted")           | inverted             , text(" ") ,
      text("underlined")         | underlined           , text(" ") ,
      text("underlinedDouble")   | underlinedDouble     , text(" ") ,
      text("blink")              | blink                , text(" ") ,
      text("strikethrough")      | strikethrough        , text(" ") ,
      text("color")              | color(Color::Blue)   , text(" ") ,
      text("bgcolor")            | bgcolor(Color::Blue) , text(" ") ,
      text("hyperlink")          | hyperlink("https://github.com/ArthurSonzogni/FTXUI"),
    });
  // clang-format on
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}
