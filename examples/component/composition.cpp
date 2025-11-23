// Copyright 2021 Arthur Sonzogni. All rights reserved.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#include <memory>  // for allocator, shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, separator, Element, operator|, vbox, border

using namespace ftxui;

// Un exemple de comment composer plusieurs composants en un seul et maintenir leur
// interactivité.
int main() {
  auto left_count = 0;
  auto right_count = 0;

  auto left_buttons = Container::Horizontal({
      Button("Decrease", [&] { left_count--; }),
      Button("Increase", [&] { left_count++; }),
  });

  auto right_buttons = Container::Horizontal({
      Button("Decrease", [&] { right_count--; }),
      Button("Increase", [&] { right_count++; }),
  });

  // Renderer décore son enfant avec une nouvelle fonction de rendu. La façon dont les
  // enfants réagissent aux événements est maintenue.  auto leftpane = Renderer(left_buttons, [&] {
    return vbox({
               text("This is the left control"),
               separator(),
               text("Left button count: " + std::to_string(left_count)),
               left_buttons->Render(),
           }) |
           border;
  });

  auto rightpane = Renderer(right_buttons, [&] {
    return vbox({
               text("This is the right control"),
               separator(),
               text("Right button count: " + std::to_string(right_count)),
               right_buttons->Render(),
           }) |
           border;
  });

  // Container groupe les composants ensemble. Pour rendre un Container::Horizontal,
  // il rend ses enfants côte à côte. Il maintient leur interactivité et
  // fournit la logique pour naviguer de l'un à l'autre en utilisant les touches fléchées.  auto composition = Container::Horizontal({leftpane, rightpane});

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(composition);
  return 0;
}

// Merci à Chris Morgan pour cet exemple !
