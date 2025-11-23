// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#include <memory>  // pour allocator, shared_ptr, __shared_ptr_access
#include <string>  // pour operator+, string, char_traits, basic_string

#include "ftxui/component/captured_mouse.hpp"  // pour ftxui
#include "ftxui/component/component.hpp"       // pour Button, Vertical, Renderer
#include "ftxui/component/component_base.hpp"  // pour ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // pour ScreenInteractive
#include "ftxui/dom/elements.hpp"  // pour separator, text, Element, operator|, vbox, border

using namespace ftxui;

void Nested(std::string path) {
  auto screen = ScreenInteractive::FitComponent();
  auto back_button = Button("Retour", screen.ExitLoopClosure());
  auto goto_1 = Button("Aller à /1", [path] { Nested(path + "/1"); });
  auto goto_2 = Button("Aller à /2", [path] { Nested(path + "/2"); });
  auto goto_3 = Button("Aller à /3", [path] { Nested(path + "/3"); });
  auto layout = Container::Vertical({
      back_button,
      goto_1,
      goto_2,
      goto_3,
  });
  auto renderer = Renderer(layout, [&] {
    return vbox({
               text("chemin: " + path),
               separator(),
               back_button->Render(),
               goto_1->Render(),
               goto_2->Render(),
               goto_3->Render(),
           }) |
           border;
  });
  screen.Loop(renderer);
}

int main() {
  auto screen = ScreenInteractive::FitComponent();
  auto button_quit = Button("Quitter", screen.ExitLoopClosure());
  auto button_nested = Button("Imbriqué", [] { Nested(""); });
  screen.Loop(Container::Vertical({
      button_quit,
      button_nested,
  }));
  return 0;
}
