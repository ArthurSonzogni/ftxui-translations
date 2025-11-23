// Copyright 2021 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <memory>  // for allocator, __shared_ptr_access
#include <string>  // for string, basic_string, operator+, to_string
#include <vector>  // for vector

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, Element, size, border, frame, vscroll_indicator, HEIGHT, LESS_THAN

int main() {
  using namespace ftxui;

  Component input_list = Container::Vertical({});
  std::vector<std::string> items(100, "");
  for (size_t i = 0; i < items.size(); ++i) {
    input_list->Add(Input(&(items[i]), "placeholder " + std::to_string(i)));
  }

  auto renderer = Renderer(input_list, [&] {
    return input_list->Render() | vscroll_indicator | frame | border |
           size(HEIGHT, LESS_THAN, 10);
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
}
