// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#include <ftxui/dom/elements.hpp>  // for text, operator|, border, Element, vbox, center, Fit, dbox
#include <ftxui/screen/screen.hpp>  // for Full, Screen
#include <memory>                   // for allocator

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui

int main() {
  using namespace ftxui;
  auto document = dbox({
      vbox({
          text("line_1"),
          text("line_2"),
          text("line_3"),
          text("line_4"),
          text("line_5"),
      }) | border,
      text("overlay") | border | center,
  });
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);

  screen.Print();

  return 0;
}
