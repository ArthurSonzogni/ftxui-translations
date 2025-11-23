// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans le
// fichier LICENCE.
#include <ftxui/dom/elements.hpp>  // for operator|, text, Element, Fit, borderDouble, borderHeavy, borderLight, borderRounded, vbox
#include <ftxui/screen/screen.hpp>  // for Screen
#include <iostream>                 // for endl, cout, ostream
#include <memory>                   // for allocator

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui

int main() {
  using namespace ftxui;

  auto make_boxed = [] {
    return vbox({
        text("borderLight") | borderStyled(LIGHT, Color::Red),
        text("borderDashed") | borderStyled(DASHED, Color::Green),
        text("borderHeavy") | borderStyled(HEAVY, Color::Blue),
        text("borderDouble") | borderStyled(DOUBLE, Color::Yellow),
        text("borderRounded") | borderStyled(ROUNDED, Color::Cyan),
    });
  };

  auto document = hbox({
                      make_boxed(),
                      separator() | color(Color::Red),
                      make_boxed(),
                      separator() | color(Color::Red),
                      make_boxed(),
                  }) |
                  borderStyled(ROUNDED, Color::Red);

  auto screen =
      Screen::Create(Dimension::Fit(document), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();
  std::cout << std::endl;
}
