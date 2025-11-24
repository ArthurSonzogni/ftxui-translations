// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスによって管理されています。
#include <stdlib.h>  // EXIT_SUCCESSのため
#include <memory>    // allocator, __shared_ptr_accessのため
#include <string>  // string, operator+, basic_string, to_string, char_traitsのため
#include <vector>  // vector, __alloc_traits<>::value_typeのため

#include "ftxui/component/captured_mouse.hpp"  // ftxuiのため
#include "ftxui/component/component.hpp"  // Menu, Renderer, Horizontal, Verticalのため
#include "ftxui/component/component_base.hpp"  // ComponentBaseのため
#include "ftxui/component/screen_interactive.hpp"  // Component, ScreenInteractiveのため
#include "ftxui/dom/elements.hpp"  // text, Element, operator|, window, flex, vboxのため

using namespace ftxui;

Component Window(std::string title, Component component) {
  return Renderer(component, [component, title] {  //
    return window(text(title), component->Render()) | flex;
  });
}

int main() {
  int menu_selected[] = {0, 0, 0};
  std::vector<std::vector<std::string>> menu_entries = {
      {
          "Ananas",
          "Raspberry",
          "Citrus",
      },
      {
          "Potatoes",
          "Weat",
          "Rise",
      },
      {
          "Carrot",
          "Lettuce",
          "Tomato",
      },
  };

  int menu_selected_global = 0;
  auto menu_global = Container::Vertical(
      {
          Window("Menu 1", Menu(&menu_entries[0], &menu_selected[0])),
          Window("Menu 2", Menu(&menu_entries[1], &menu_selected[1])),
          Window("Menu 3", Menu(&menu_entries[2], &menu_selected[2])),
      },
      &menu_selected_global);

  auto info = Renderer([&] {
    int g = menu_selected_global;
    std::string value = menu_entries[g][menu_selected[g]];
    return window(text("Content"),  //
                  vbox({
                      text("menu_selected_global = " + std::to_string(g)),
                      text("menu_selected[0]     = " +
                           std::to_string(menu_selected[0])),
                      text("menu_selected[1]     = " +
                           std::to_string(menu_selected[1])),
                      text("menu_selected[2]     = " +
                           std::to_string(menu_selected[2])),
                      text("Value                = " + value),
                  })) |
           flex;
  });

  auto global = Container::Horizontal({
      menu_global,
      info,
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(global);
  return EXIT_SUCCESS;
}