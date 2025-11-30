// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <ftxui/dom/elements.hpp>  // for hbox, text, bgcolor, operator|, vbox, Elements, window, Element, Fit
#include <ftxui/screen/screen.hpp>  // for Full, Screen
#include <memory>                   // for allocator
#include <utility>                  // for move

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for Color, ftxui

int main() {
  using namespace ftxui;

  Elements red_line;
  Elements green_line;
  Elements blue_line;
  Elements cyan_line;
  Elements magenta_line;
  Elements yellow_line;

  for (int value = 0; value < 255; value += 3) {
    int v = value * value / 255;
    red_line.push_back(text(" ") | bgcolor(Color::RGB(v, 0, 0)));
    green_line.push_back(text(" ") | bgcolor(Color::RGB(0, v, 0)));
    blue_line.push_back(text(" ") | bgcolor(Color::RGB(0, 0, v)));
    cyan_line.push_back(text(" ") | bgcolor(Color::RGB(0, v, v)));
    magenta_line.push_back(text(" ") | bgcolor(Color::RGB(v, 0, v)));
    yellow_line.push_back(text(" ") | bgcolor(Color::RGB(v, v, 0)));
  }

  auto document = vbox({
      window(text("Colores primarios"),
             vbox({
                 hbox({text("Línea roja    :"), hbox(std::move(red_line))}),
                 hbox({text("Línea verde   :"), hbox(std::move(green_line))}),
                 hbox({text("Línea azul    :"), hbox(std::move(blue_line))}),
             })),
      window(text("Colores secundarios"),
             vbox({
                 hbox({text("Línea cian    :"), hbox(std::move(cyan_line))}),
                 hbox({text("Línea magenta :"), hbox(std::move(magenta_line))}),
                 hbox({text("Línea amarilla:"), hbox(std::move(yellow_line))}),
             })),
  });

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);

  screen.Print();

  return 0;
}
