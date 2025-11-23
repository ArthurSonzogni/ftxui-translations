// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui se trouve dans
// le fichier LICENSE.
#include <string>  // for string, allocator, basic_string
#include <vector>  // for vector

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for operator|, Maybe, Checkbox, Radiobox, Renderer, Vertical
#include "ftxui/component/component_base.hpp"      // for Component
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for Element, border, color, operator|, text
#include "ftxui/screen/color.hpp"  // for Color, Color::Red

using namespace ftxui;

int main() {
  std::vector<std::string> entries = {
      "entry 1",
      "entry 2",
      "entry 3",
  };
  int menu_1_selected = 0;
  int menu_2_selected = 0;

  bool menu_1_show = false;
  bool menu_2_show = false;

  auto layout = Container::Vertical({
      Checkbox("Show menu_1", &menu_1_show),
      Radiobox(&entries, &menu_1_selected) | border | Maybe(&menu_1_show),
      Checkbox("Show menu_2", &menu_2_show),
      Radiobox(&entries, &menu_2_selected) | border | Maybe(&menu_2_show),

      Renderer([] {
        return text("Vous avez trouvé la combinaison secrète!") | color(Color::Red);
      }) | Maybe([&] { return menu_1_selected == 1 && menu_2_selected == 2; }),
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(layout);
}
