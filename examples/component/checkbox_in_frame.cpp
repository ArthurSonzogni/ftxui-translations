// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <array>   // for array
#include <memory>  // for shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Checkbox, Renderer, Vertical
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, Element, size, border, frame, vscroll_indicator, HEIGHT, LESS_THAN

using namespace ftxui;

int main() {
  std::array<bool, 30> states;

  auto container = Container::Vertical({});
  for (int i = 0; i < 30; ++i) {
    states[i] = false;
    container->Add(Checkbox("Checkbox" + std::to_string(i), &states[i]));
  }

  auto renderer = Renderer(container, [&] {
    return container->Render() | vscroll_indicator | frame |
           size(HEIGHT, LESS_THAN, 10) | border;
  });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(renderer);

  return 0;
}
