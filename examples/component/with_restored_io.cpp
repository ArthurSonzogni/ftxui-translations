// Copyright 2022 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui se trouve
// dans le fichier LICENSE.
#include <cstdlib>   // for system, EXIT_SUCCESS
#include <iostream>  // for operator<<, basic_ostream, basic_ostream::operator<<, cout, endl, flush, ostream, basic_ostream<>::__ostream_type, cin
#include <memory>    // for shared_ptr, __shared_ptr_access, allocator
#include <string>    // for getline, string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, filler, Element, borderEmpty, hbox, size, paragraph, vbox, LESS_THAN, border, center, HEIGHT, WIDTH

int main() {
  using namespace ftxui;

  auto screen = ScreenInteractive::Fullscreen();

  // En appuyant sur ce bouton, "screen.WithRestoredIO" désinstallera
  // temporairement le hook du terminal et exécutera la fonction de rappel
  // fournie. Cela permet d'exécuter l'application en mode non interactif.
  auto btn_run = Button("Execute with restored IO", screen.WithRestoredIO([] {
    std::cout << "This is a child program using stdin/stdout." << std::endl;
    for (int i = 0; i < 10; ++i) {
      std::cout << "Please enter 10 strings (" << i << "/10)" << std::flush;
      std::string input;
      std::getline(std::cin, input);
    }
  }));

  auto btn_quit = Button("Quit", screen.ExitLoopClosure());

  auto layout = Container::Horizontal({
      btn_run,
      btn_quit,
  });

  auto renderer = Renderer(layout, [&] {
    auto explanation = paragraph(
        "Après avoir cliqué sur ce bouton, le ScreenInteractive sera "
        "suspendu et l'accès à stdin/stdout sera temporairement "
        "restauré pour l'exécution d'une fonction.");
    auto element = vbox({
        explanation | borderEmpty,
        hbox({
            btn_run->Render(),
            filler(),
            btn_quit->Render(),
        }),
    });

    element = element | borderEmpty | border | size(WIDTH, LESS_THAN, 80) |
              size(HEIGHT, LESS_THAN, 20) | center;
    return element;
  });

  screen.Loop(renderer);
  return EXIT_SUCCESS;
}