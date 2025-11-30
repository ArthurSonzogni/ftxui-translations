// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <ftxui/dom/elements.hpp>  // para text, operator|, Element, bgcolor, color, blink, bold, dim, inverted, underlined, Fit, hbox
#include <ftxui/screen/screen.hpp>  // para Full, Screen
#include <memory>                   // para allocator

#include "ftxui/dom/node.hpp"      // para Render
#include "ftxui/screen/color.hpp"  // para Color, Color::Blue, ftxui

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