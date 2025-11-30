// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar
// en el archivo LICENSE.
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
