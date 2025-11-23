// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver
// dans le fichier LICENSE.
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
      window(text("Couleurs primaires"),
             vbox({
                 hbox({text("Ligne rouge   :"), hbox(std::move(red_line))}),
                 hbox({text("Ligne verte   :"), hbox(std::move(green_line))}),
                 hbox({text("Ligne bleue   :"), hbox(std::move(blue_line))}),
             })),
      window(text("Couleurs secondaires"),
             vbox({
                 hbox({text("Ligne cyan    :"), hbox(std::move(cyan_line))}),
                 hbox({text("Ligne magenta :"), hbox(std::move(magenta_line))}),
                 hbox({text("Ligne jaune   :"), hbox(std::move(yellow_line))}),
             })),
  });

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);

  screen.Print();

  return 0;
}
