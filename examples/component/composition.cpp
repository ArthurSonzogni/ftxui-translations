// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <memory>  // for allocator, shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, separator, Element, operator|, vbox, border

using namespace ftxui;

// Un ejemplo de cómo componer múltiples componentes en uno y mantener su
// interactividad.
int main() {
  auto left_count = 0;
  auto right_count = 0;

  auto left_buttons = Container::Horizontal({
      Button("Decrease", [&] { left_count--; }),
      Button("Increase", [&] { left_count++; }),
  });

  auto right_buttons = Container::Horizontal({
      Button("Decrease", [&] { right_count--; }),
      Button("Increase", [&] { right_count++; }),
  });

  // Renderer decora su hijo con una nueva función de renderizado. La forma en que los
  // hijos reaccionan a los eventos se mantiene.  auto leftpane = Renderer(left_buttons, [&] {
    return vbox({
               text("Este es el control izquierdo"),
               separator(),
               text("Left button count: " + std::to_string(left_count)),
               left_buttons->Render(),
           }) |
           border;
  });

  auto rightpane = Renderer(right_buttons, [&] {
    return vbox({
               text("Este es el control derecho"),
               separator(),
               text("Right button count: " + std::to_string(right_count)),
               right_buttons->Render(),
           }) |
           border;
  });

  // Container agrupa componentes. Para renderizar un Container::Horizontal,
  // renderiza sus hijos uno al lado del otro. Mantiene su interactividad y
  // proporciona la lógica para navegar de uno a otro usando las teclas de flecha.  auto composition = Container::Horizontal({leftpane, rightpane});

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(composition);
  return 0;
}

// ¡Gracias a Chris Morgan por este ejemplo!
