// Copyright 2022 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <memory>  // pour allocator, __shared_ptr_access, shared_ptr
#include <string>  // pour to_string, operator+

#include "ftxui/component/captured_mouse.hpp"  // pour ftxui
#include "ftxui/component/component.hpp"       // pour Button, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // pour ComponentBase
#include "ftxui/component/component_options.hpp"   // pour ButtonOption
#include "ftxui/component/screen_interactive.hpp"  // pour ScreenInteractive
#include "ftxui/dom/elements.hpp"  // pour operator|, text, Element, hbox, separator, size, vbox, border, frame, vscroll_indicator, HEIGHT, LESS_THAN
#include "ftxui/screen/color.hpp"  // pour Color, Color::Default, Color::GrayDark, Color::White

using namespace ftxui;

int main() {
  int counter = 0;
  auto on_click = [&] { counter++; };

  auto style = ButtonOption::Animated(Color::Default, Color::GrayDark,
                                      Color::Default, Color::White);

  auto container = Container::Vertical({});
  for (int i = 0; i < 30; ++i) {
    auto button = Button("Button " + std::to_string(i), on_click, style);
    container->Add(button);
  }

  auto renderer = Renderer(container, [&] {
    return vbox({
               hbox({
                   text("Counter:"),
                   text(std::to_string(counter)),
               }),
               separator(),
               container->Render() | vscroll_indicator | frame |
                   size(HEIGHT, LESS_THAN, 20),
           }) |
           border;
  });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(renderer);

  return 0;
}
