// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <functional>  // for function
#include <memory>      // for shared_ptr, allocator, __shared_ptr_access
#include <string>      // for string, basic_string
#include <vector>      // for vector

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Slider, Checkbox, Vertical, Renderer, Button, Input, Menu, Radiobox, Toggle
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, operator|, Element, size, xflex, text, WIDTH, hbox, vbox, EQUAL, border, GREATER_THAN

using namespace ftxui;

// Muestra un componente de forma agradable con un título a la izquierda.
Component Wrap(std::string name, Component component) {
  return Renderer(component, [name, component] {
    return hbox({
               text(name) | size(WIDTH, EQUAL, 8),
               separator(),
               component->Render() | xflex,
           }) |
           xflex;
  });
}

int main() {
  auto screen = ScreenInteractive::FitComponent();

  // -- Menú
  // ----------------------------------------------------------------------
  const std::vector<std::string> menu_entries = {
      "Menu 1",
      "Menu 2",
      "Menu 3",
      "Menu 4",
  };
  int menu_selected = 0;
  auto menu = Menu(&menu_entries, &menu_selected);
  menu = Wrap("Menu", menu);

  // -- Conmutador------------------------------------------------------------------
  int toggle_selected = 0;
  std::vector<std::string> toggle_entries = {
      "Toggle_1",
      "Toggle_2",
  };
  auto toggle = Toggle(&toggle_entries, &toggle_selected);
  toggle = Wrap("Toggle", toggle);

  // -- Casilla de verificación ---------------------------------------------------------------
  bool checkbox_1_selected = false;
  bool checkbox_2_selected = false;
  bool checkbox_3_selected = false;
  bool checkbox_4_selected = false;

  auto checkboxes = Container::Vertical({
      Checkbox("checkbox1", &checkbox_1_selected),
      Checkbox("checkbox2", &checkbox_2_selected),
      Checkbox("checkbox3", &checkbox_3_selected),
      Checkbox("checkbox4", &checkbox_4_selected),
  });
  checkboxes = Wrap("Checkbox", checkboxes);

  // -- Botón de opción ---------------------------------------------------------------
  int radiobox_selected = 0;
  std::vector<std::string> radiobox_entries = {
      "Radiobox 1",
      "Radiobox 2",
      "Radiobox 3",
      "Radiobox 4",
  };
  auto radiobox = Radiobox(&radiobox_entries, &radiobox_selected);
  radiobox = Wrap("Radiobox", radiobox);

  // -- Entrada ------------------------------------------------------------------
  std::string input_label;
  auto input = Input(&input_label, "placeholder");
  input = Wrap("Input", input);

  // -- Botón -----------------------------------------------------------------
  std::string button_label = "Quit";
  std::function<void()> on_button_clicked_;
  auto button = Button(&button_label, screen.ExitLoopClosure());
  button = Wrap("Button", button);

  // -- Deslizador -----------------------------------------------------------------
  int slider_value_1 = 12;
  int slider_value_2 = 56;
  int slider_value_3 = 128;
  auto sliders = Container::Vertical({
      Slider("R:", &slider_value_1, 0, 256, 1),
      Slider("G:", &slider_value_2, 0, 256, 1),
      Slider("B:", &slider_value_3, 0, 256, 1),
  });
  sliders = Wrap("Slider", sliders);

  // Un texto largo:
  auto lorel_ipsum = Renderer([] {
    return vbox({
        text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. "),
        text("Sed do eiusmod tempor incididunt ut labore et dolore magna "
             "aliqua. "),
        text("Ut enim ad minim veniam, quis nostrud exercitation ullamco "
             "laboris nisi ut aliquip ex ea commodo consequat. "),
        text("Duis aute irure dolor in reprehenderit in voluptate velit esse "
             "cillum dolore eu fugiat nulla pariatur. "),
        text("Excepteur sint occaecat cupidatat non proident, sunt in culpa "
             "qui officia deserunt mollit anim id est laborum. "),

    });
  });
  lorel_ipsum = Wrap("Lorel Ipsum", lorel_ipsum);

  // -- Diseño
  // -----------------------------------------------------------------
  auto layout = Container::Vertical({
      menu,
      toggle,
      checkboxes,
      radiobox,
      input,
      sliders,
      button,
      lorel_ipsum,
  });

  auto component = Renderer(layout, [&] {
    return vbox({
               menu->Render(),
               separator(),
               toggle->Render(),
               separator(),
               checkboxes->Render(),
               separator(),
               radiobox->Render(),
               separator(),
               input->Render(),
               separator(),
               sliders->Render(),
               separator(),
               button->Render(),
               separator(),
               lorel_ipsum->Render(),
           }) |
           xflex | size(WIDTH, GREATER_THAN, 40) | border;
  });

  screen.Loop(component);

  return 0;
}
