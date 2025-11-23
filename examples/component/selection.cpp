// Copyright 2020 Arthur Sonzogni. All rights reserved.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <string>  // pour char_traits, operator+, string, basic_string

#include "ftxui/component/component.hpp"       // pour Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // pour ComponentBase
#include "ftxui/component/component_options.hpp"  // pour InputOption
#include "ftxui/component/screen_interactive.hpp"  // pour Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // pour text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // pour Ref

using namespace ftxui;

Element LoremIpsum() {
  return vbox({
      text("FTXUI : Une bibliothèque puissante pour construire des interfaces utilisateur."),
      text("Profitez d'un riche ensemble de composants et d'un style déclaratif."),
      text("Créez des interfaces utilisateur belles et réactives avec un minimum d'effort."),
      text("Rejoignez la communauté et découvrez la puissance de FTXUI."),
  });
}

int main() {
  auto screen = ScreenInteractive::TerminalOutput();

  auto quit =
      Button("Quitter", screen.ExitLoopClosure(), ButtonOption::Animated());

  int selection_change_counter = 0;
  std::string selection_content = "";
  screen.SelectionChange([&] {
    selection_change_counter++;
    selection_content = screen.GetSelection();
  });

  // Les composants :
  auto renderer = Renderer(quit, [&] {
    return vbox({
        text("Sélection changée : " + std::to_string(selection_change_counter) +
             " fois"),
        text("Actuellement sélectionné : "),
        paragraph(selection_content) | vscroll_indicator | frame | border |
            size(HEIGHT, EQUAL, 10),
        window(text("Séparation horizontale"), hbox({
                                             LoremIpsum(),
                                             separator(),
                                             LoremIpsum(),
                                             separator(),
                                             LoremIpsum(),
                                         })),
        window(text("Séparation verticale"), vbox({
                                           LoremIpsum(),
                                           separator(),
                                           LoremIpsum(),
                                           separator(),
                                           LoremIpsum(),
                                       })),
        window(text("Séparation en grille avec un style différent"),
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