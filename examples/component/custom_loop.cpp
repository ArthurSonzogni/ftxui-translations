// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
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

  // Crea un componente que cuenta el número de frames dibujados y eventos manejados.
  int custom_loop_count = 0;
  int frame_count = 0;
  int event_count = 0;
  auto component = Renderer([&] {
    frame_count++;
    return vbox({
               text("Esto demuestra el uso de un ftxui::Loop personalizado. Se "),
               text("ejecuta a 100 iteraciones por segundo. Los eventos de FTXUI "),
               text("se procesan una vez por iteración y se renderiza un nuevo frame "),
               text("según sea necesario"),
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
