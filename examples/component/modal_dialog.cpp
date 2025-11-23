// Copyright 2022 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <ftxui/component/component_options.hpp>  // for ButtonOption
#include <ftxui/component/mouse.hpp>              // for ftxui
#include <functional>                             // for function
#include <memory>                                 // for allocator, shared_ptr

#include "ftxui/component/component.hpp"  // for Button, operator|=, Renderer, Vertical, Modal
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive, Component
#include "ftxui/dom/elements.hpp"  // for operator|, separator, text, size, Element, vbox, border, GREATER_THAN, WIDTH, center, HEIGHT

using namespace ftxui;

auto button_style = ButtonOption::Animated();

// Définition du composant principal. Les détails ne sont pas importants.
Component MainComponent(std::function<void()> show_modal,
                        std::function<void()> exit) {
  auto component = Container::Vertical({
      Button("Show modal", show_modal, button_style),
      Button("Quit", exit, button_style),
  });
  // Peaufiner le rendu des deux boutons :
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

// Définition du composant modal. Les détails ne sont pas importants.
Component ModalComponent(std::function<void()> do_nothing,
                         std::function<void()> hide_modal) {
  auto component = Container::Vertical({
      Button("Do nothing", do_nothing, button_style),
      Button("Quit modal", hide_modal, button_style),
  });
  // Peaufiner le rendu des deux boutons :
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

  // État de l'application :
  bool modal_shown = false;

  // Quelques actions modifiant l'état :
  auto show_modal = [&] { modal_shown = true; };
  auto hide_modal = [&] { modal_shown = false; };
  auto exit = screen.ExitLoopClosure();
  auto do_nothing = [&] {};

  // Instancier les composants principal et modal :
  auto main_component = MainComponent(show_modal, exit);
  auto modal_component = ModalComponent(do_nothing, hide_modal);

  // Utiliser la fonction `Modal` pour associer le composant principal et sa fenêtre modale.
  // Le booléen |modal_shown| contrôle si la modale est affichée ou non.  main_component |= Modal(modal_component, &modal_shown);

  screen.Loop(main_component);
  return 0;
}
