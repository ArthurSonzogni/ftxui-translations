// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <string>  // para string, allocator, basic_string
#include <vector>  // para vector

#include "ftxui/component/captured_mouse.hpp"  // para ftxui
#include "ftxui/component/component.hpp"  // para operator|, Maybe, Checkbox, Radiobox, Renderer, Vertical
#include "ftxui/component/component_base.hpp"      // para Component
#include "ftxui/component/screen_interactive.hpp"  // para ScreenInteractive
#include "ftxui/dom/elements.hpp"  // para Element, border, color, operator|, text
#include "ftxui/screen/color.hpp"  // para Color, Color::Red

using namespace ftxui;

int main() {
  std::vector<std::string> entries = {
      "entrada 1",
      "entrada 2",
      "entrada 3",
  };
  int menu_1_selected = 0;
  int menu_2_selected = 0;

  bool menu_1_show = false;
  bool menu_2_show = false;

  auto layout = Container::Vertical({
      Checkbox("Mostrar menu_1", &menu_1_show),
      Radiobox(&entries, &menu_1_selected) | border | Maybe(&menu_1_show),
      Checkbox("Mostrar menu_2", &menu_2_show),
      Radiobox(&entries, &menu_2_selected) | border | Maybe(&menu_2_show),

      Renderer([] {
        return text("¡Encontraste la combinación secreta!") | color(Color::Red);
      }) | Maybe([&] { return menu_1_selected == 1 && menu_2_selected == 2; }),
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(layout);
}
