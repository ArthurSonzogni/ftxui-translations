// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <string>  // for char_traits, operator+, string, basic_string

#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // for Ref

using namespace ftxui;

Element LoremIpsum() {
  return vbox({
      text("FTXUI: Una potente biblioteca para construir interfaces de usuario."),
      text("Disfruta de un rico conjunto de componentes y un estilo declarativo."),
      text("Crea UIs hermosas y responsivas con un mínimo esfuerzo."),
      text("Únete a la comunidad y experimenta el poder de FTXUI."),
  });
}

int main() {
  auto screen = ScreenInteractive::TerminalOutput();

  auto quit =
      Button("Quit", screen.ExitLoopClosure(), ButtonOption::Animated());

  int selection_change_counter = 0;
  std::string selection_content = "";
  screen.SelectionChange([&] {
    selection_change_counter++;
    selection_content = screen.GetSelection();
  });

  // Los componentes:
  auto renderer = Renderer(quit, [&] {
    return vbox({
        text("Selección cambiada: " + std::to_string(selection_change_counter) +
             " veces"),
        text("Actualmente seleccionado: "),
        paragraph(selection_content) | vscroll_indicator | frame | border |
            size(HEIGHT, EQUAL, 10),
        window(text("División horizontal"), hbox({
                                             LoremIpsum(),
                                             separator(),
                                             LoremIpsum(),
                                             separator(),
                                             LoremIpsum(),
                                         })),
        window(text("División vertical"), vbox({
                                           LoremIpsum(),
                                           separator(),
                                           LoremIpsum(),
                                           separator(),
                                           LoremIpsum(),
                                       })),
        window(text("División en cuadrícula con estilo diferente"),
               vbox({
                   hbox({
                       LoremIpsum(),
                       separator(),
                       LoremIpsum()                                   //
                           | selectionBackgroundColor(Color::Yellow)  //
                           | selectionColor(Color::Black)             //
                           | selectionStyleReset,
                       separator(),
                       LoremIpsum() | selectionColor(Color::Blue),
                   }),
                   separator(),
                   hbox({
                       LoremIpsum() | selectionColor(Color::Red),
                       separator(),
                       LoremIpsum() | selectionStyle([](Pixel& pixel) {
                         pixel.underlined_double = true;
                       }),
                       separator(),
                       LoremIpsum(),
                   }),
               })),
        quit->Render(),
    });
  });

  screen.Loop(renderer);
}
