// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <memory>  // for shared_ptr, __shared_ptr_access
#include <string>  // for string, basic_string, operator+, to_string
#include <vector>  // for vector

#include "ftxui/component/captured_mouse.hpp"      // for ftxui
#include "ftxui/component/component.hpp"           // for Radiobox, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, Element, size, border, frame, HEIGHT, LESS_THAN

using namespace ftxui;

int main() {
  std::vector<std::string> entries;
  int selected = 0;

  for (int i = 0; i < 30; ++i) {
    entries.push_back("RadioBox " + std::to_string(i));
  }
  auto radiobox = Radiobox(&entries, &selected);
  auto renderer = Renderer(radiobox, [&] {
    return radiobox->Render() | vscroll_indicator | frame |
           size(HEIGHT, LESS_THAN, 10) | border;
  });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(renderer);

  return 0;
}