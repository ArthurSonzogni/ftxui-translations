// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Utilisation de ce code source régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <memory>  // pour shared_ptr, allocator, __shared_ptr_access

#include "ftxui/component/captured_mouse.hpp"  // pour ftxui
#include "ftxui/component/component.hpp"       // pour Renderer, Button, Vertical
#include "ftxui/component/component_base.hpp"  // pour ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // pour ScreenInteractive
#include "ftxui/dom/elements.hpp"  // pour operator|, Element, text, bold, border, center, color
#include "ftxui/screen/color.hpp"  // pour Color, Color::Red

int main() {
  using namespace ftxui;
  auto screen = ScreenInteractive::FitComponent();

  // Un Renderer() est un composant utilisant une fonction lambda comme paramètre pour
  // se rendre.

  // 1. Exemple de rendu focusable :
  auto renderer_focusable = Renderer([](bool focused) {
    if (focused) {
      return text("RENDU FOCUSABLE()") | center | bold | border;
    } else {
      return text(" Rendu focusable() ") | center | border;
    }
  });

  // 2. Exemples d'un rendu non focusable.
  auto renderer_non_focusable = Renderer([&] {
    return text("~~~~~ Rendu non focusable() ~~~~~");  //
  });

  // 3. Renderer peut envelopper d'autres composants pour redéfinir leur fonction Render().
  auto button = Button("Bouton quitter enveloppé", screen.ExitLoopClosure());
  auto renderer_wrap = Renderer(button, [&] {
    if (button->Focused()) {
      return button->Render() | bold | color(Color::Red);
    } else {
      return button->Render();
    }
  });

  // Rendons tout le monde :
  screen.Loop(Container::Vertical({
      renderer_focusable,
      renderer_non_focusable,
      renderer_wrap,
  }));
}