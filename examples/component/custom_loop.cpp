// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#include <stdlib.h>                   // for EXIT_SUCCESS
#include <chrono>                     // for milliseconds
#include <ftxui/component/event.hpp>  // for Event
#include <ftxui/component/mouse.hpp>  // for ftxui
#include <ftxui/dom/elements.hpp>  // for text, separator, Element, operator|, vbox, border
#include <memory>                  // for allocator, shared_ptr
#include <string>                  // for operator+, to_string
#include <thread>                  // for sleep_for

#include "ftxui/component/component.hpp"  // for CatchEvent, Renderer, operator|=
#include "ftxui/component/loop.hpp"       // for Loop
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive

int main() {
  using namespace ftxui;
  auto screen = ScreenInteractive::FitComponent();

  // Crée un composant comptant le nombre de frames dessinées et d'événements gérés.
  int custom_loop_count = 0;
  int frame_count = 0;
  int event_count = 0;
  auto component = Renderer([&] {
    frame_count++;
    return vbox({
               text("Ceci démontre l'utilisation d'une ftxui::Loop personnalisée. Elle "),
               text("s'exécute à 100 itérations par seconde. Les événements FTXUI "),
               text("sont tous traités une fois par itération et une nouvelle image "),
               text("est rendue si nécessaire"),
               separator(),
               text("ftxui event count: " + std::to_string(event_count)),
               text("ftxui frame count: " + std::to_string(frame_count)),
               text("Custom loop count: " + std::to_string(custom_loop_count)),
           }) |
           border;
  });

  component |= CatchEvent([&](Event) -> bool {
    event_count++;
    return false;
  });

  Loop loop(&screen, component);

  while (!loop.HasQuitted()) {
    custom_loop_count++;
    loop.RunOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return EXIT_SUCCESS;
}
