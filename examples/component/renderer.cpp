// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <memory>  // for shared_ptr, allocator, __shared_ptr_access

#include "ftxui/component/app.hpp"             // for App
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Renderer, Button, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/dom/elements.hpp"  // for operator|, Element, text, bold, border, center, color
#include "ftxui/screen/color.hpp"  // for Color, Color::Red

int main() {
  using namespace ftxui;
  auto screen = App::FitComponent();

  // Un Renderer() est un composant utilisant une fonction lambda comme paramètre pour
  // se rendre.

  // 1. Exemple de rendu focusable :
  auto renderer_focusable = Renderer([](bool focused) {
    if (focused) {
      return text("FOCUSABLE RENDERER()") | center | bold | border;
    } else {
      return text(" Focusable renderer() ") | center | border;
    }
  });

  // 2. Exemples d'un rendu non focusable.
  auto renderer_non_focusable = Renderer([&] {
    return text("~~~~~ Non Focusable renderer() ~~~~~");  //
  });

  // 3. Renderer peut envelopper d'autres composants pour redéfinir leur fonction Render().
  auto button = Button("Wrapped quit button", screen.ExitLoopClosure());
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
