// Copyright 2022 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar
// en el archivo LICENSE.
#include <ftxui/component/component_options.hpp>  // for ButtonOption
#include <ftxui/component/mouse.hpp>              // for ftxui
#include <functional>                             // for function
#include <memory>                                 // for allocator, shared_ptr

#include "ftxui/component/component.hpp"  // for Button, operator|=, Renderer, Vertical, Modal
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive, Component
#include "ftxui/dom/elements.hpp"  // for operator|, separator, text, size, Element, vbox, border, GREATER_THAN, WIDTH, center, HEIGHT

using namespace ftxui;

auto button_style = ButtonOption::Animated();

// Definición del componente principal. Los detalles no son importantes.
Component MainComponent(std::function<void()> show_modal,
                        std::function<void()> exit) {
  auto component = Container::Vertical({
      Button("Show modal", show_modal, button_style),
      Button("Quit", exit, button_style),
  });
  // Mejora cómo se renderizan los dos botones:
  component |= Renderer([&](Element inner) {
    return vbox({
               text("Main component"),
               separator(),
               inner,
           })                                //
           | size(WIDTH, GREATER_THAN, 15)   //
           | size(HEIGHT, GREATER_THAN, 15)  //
           | border                          //
           | center;                         //
  });
  return component;
}

// Definición del componente modal. Los detalles no son importantes.
Component ModalComponent(std::function<void()> do_nothing,
                         std::function<void()> hide_modal) {
  auto component = Container::Vertical({
      Button("Do nothing", do_nothing, button_style),
      Button("Quit modal", hide_modal, button_style),
  });
  // Mejora cómo se renderizan los dos botones:
  component |= Renderer([&](Element inner) {
    return vbox({
               text("Modal component "),
               separator(),
               inner,
           })                               //
           | size(WIDTH, GREATER_THAN, 30)  //
           | border;                        //
  });
  return component;
}

int main(int argc, const char* argv[]) {
  auto screen = ScreenInteractive::TerminalOutput();

  // Estado de la aplicación:
  bool modal_shown = false;

  // Algunas acciones que modifican el estado:
  auto show_modal = [&] { modal_shown = true; };
  auto hide_modal = [&] { modal_shown = false; };
  auto exit = screen.ExitLoopClosure();
  auto do_nothing = [&] {};

  // Instanciar los componentes principal y modal:
  auto main_component = MainComponent(show_modal, exit);
  auto modal_component = ModalComponent(do_nothing, hide_modal);

  // Usa la función `Modal` para usar juntos el componente principal y su ventana
  // modal. El booleano |modal_shown| controla si el modal se muestra o no.
  main_component |= Modal(modal_component, &modal_shown);

  screen.Loop(main_component);
  return 0;
}
