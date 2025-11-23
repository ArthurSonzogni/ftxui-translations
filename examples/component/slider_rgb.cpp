// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée
// dans le fichier LICENSE.
#include <memory>  // pour allocator, shared_ptr, __shared_ptr_access
#include <string>  // pour char_traits, operator+, to_string

#include "ftxui/component/captured_mouse.hpp"  // pour ftxui
#include "ftxui/component/component.hpp"       // pour Slider, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // pour ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // pour ScreenInteractive
#include "ftxui/dom/elements.hpp"  // pour separator, operator|, Element, size, text, vbox, xflex, bgcolor, hbox, GREATER_THAN, WIDTH, border, HEIGHT, LESS_THAN
#include "ftxui/screen/color.hpp"  // pour Color

using namespace ftxui;

Element ColorTile(int red, int green, int blue) {
  return text("") | size(WIDTH, GREATER_THAN, 14) |
         size(HEIGHT, GREATER_THAN, 7) | bgcolor(Color::RGB(red, green, blue));
}

Element ColorString(int red, int green, int blue) {
  return text("RGB = (" +                    //
              std::to_string(red) + "," +    //
              std::to_string(green) + "," +  //
              std::to_string(blue) + ")"     //
  );
}

int main() {
  int red = 128;
  int green = 25;
  int blue = 100;
  auto slider_red = Slider("Red  :", &red, 0, 255, 1);
  auto slider_green = Slider("Green:", &green, 0, 255, 1);
  auto slider_blue = Slider("Blue :", &blue, 0, 255, 1);

  auto container = Container::Vertical({
      slider_red,
      slider_green,
      slider_blue,
  });

  auto renderer = Renderer(container, [&] {
    return hbox({
               ColorTile(red, green, blue),
               separator(),
               vbox({
                   slider_red->Render(),
                   separator(),
                   slider_green->Render(),
                   separator(),
                   slider_blue->Render(),
                   separator(),
                   ColorString(red, green, blue),
               }) | xflex,
           }) |
           border | size(WIDTH, LESS_THAN, 80);
  });
  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
}
