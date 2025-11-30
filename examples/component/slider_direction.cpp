// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <array>                                  // para array
#include <cmath>                                  // para sin
#include <ftxui/component/component_base.hpp>     // para ComponentBase
#include <ftxui/component/component_options.hpp>  // para SliderOption
#include <ftxui/dom/direction.hpp>  // para Direction, Direction::Up
#include <ftxui/dom/elements.hpp>   // para size, GREATER_THAN, HEIGHT
#include <ftxui/util/ref.hpp>       // para ConstRef, Ref
#include <memory>                   // para shared_ptr, __shared_ptr_access

#include "ftxui/component/captured_mouse.hpp"  // para ftxui
#include "ftxui/component/component.hpp"  // para Horizontal, Slider, operator|=
#include "ftxui/component/screen_interactive.hpp"  // para ScreenInteractive

using namespace ftxui;

int main() {
  auto screen = ScreenInteractive::TerminalOutput();
  std::array<int, 30> values;
  for (size_t i = 0; i < values.size(); ++i) {
    values[i] = 50 + 20 * std::sin(i * 0.3);
  }

  auto layout_horizontal = Container::Horizontal({});
  for (auto& value : values) {
    // En C++17:
    SliderOption<int> option;
    option.value = &value;
    option.max = 100;
    option.increment = 5;
    option.direction = Direction::Up;
    layout_horizontal->Add(Slider<int>(option));

    /* En C++20:
    layout_horizontal->Add(Slider<int>({
        .value = &values[i],
        .max = 100,
        .increment = 5,
        .direction = Direction::Up,
    }));
    */
  }

  layout_horizontal |= size(HEIGHT, GREATER_THAN, 20);

  screen.Loop(layout_horizontal);
}
