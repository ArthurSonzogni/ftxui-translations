// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <ftxui/dom/elements.hpp>  // for text, center, separator, operator|, flex, Element, vbox, Fit, hbox, border
#include <ftxui/screen/screen.hpp>  // for Full, Screen
#include <memory>                   // for allocator

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui

int main() {
  using namespace ftxui;
  auto document = hbox({
                      text("left-column"),
                      separator(),
                      vbox({
                          center(text("top")) | flex,
                          separator(),
                          center(text("bottom")),
                      }) | flex,
                      separator(),
                      text("right-column"),
                  }) |
                  border;
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}
