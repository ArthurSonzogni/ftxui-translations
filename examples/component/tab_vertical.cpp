// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <memory>  // pour allocator, __shared_ptr_access, shared_ptr
#include <string>  // pour string, basic_string
#include <vector>  // pour vector

#include "ftxui/component/captured_mouse.hpp"  // pour ftxui
#include "ftxui/component/component.hpp"  // pour Radiobox, Horizontal, Menu, Renderer, Tab
#include "ftxui/component/component_base.hpp"      // pour ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // pour ScreenInteractive
#include "ftxui/dom/elements.hpp"  // pour Element, separator, hbox, operator|, border

using namespace ftxui;

int main() {
  std::vector<std::string> tab_values{
      "tab_1",
      "tab_2",
      "tab_3",
  };
  int tab_selected = 0;
  auto tab_menu = Menu(&tab_values, &tab_selected);

  std::vector<std::string> tab_1_entries{
      "Forest",
      "Water",
      "I don't know",
  };
  int tab_1_selected = 0;

  std::vector<std::string> tab_2_entries{
      "Hello",
      "Hi",
      "Hay",
  };
  int tab_2_selected = 0;

  std::vector<std::string> tab_3_entries{
      "Table",
      "Nothing",
      "Is",
      "Empty",
  };
  int tab_3_selected = 0;
  auto tab_container = Container::Tab(
      {
          Radiobox(&tab_1_entries, &tab_1_selected),
          Radiobox(&tab_2_entries, &tab_2_selected),
          Radiobox(&tab_3_entries, &tab_3_selected),
      },
      &tab_selected);

  auto container = Container::Horizontal({
      tab_menu,
      tab_container,
  });

  auto renderer = Renderer(container, [&] {
    return hbox({
               tab_menu->Render(),
               separator(),
               tab_container->Render(),
           }) |
           border;
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
}
