// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <ftxui/dom/elements.hpp>  // para texto, operator|, underlinedDouble, Fit, hbox, Element
#include <ftxui/screen/screen.hpp>  // para Full, Screen
#include <memory>                   // para asignador

#include "ftxui/dom/node.hpp"      // para Render
#include "ftxui/screen/color.hpp"  // para ftxui

int main() {
  using namespace ftxui;
  auto document =  //
      hbox({
          text("Este texto está "),
          text("subrayado doble") | underlinedDouble,
          text(". ¿Te gusta?"),
      });
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}
