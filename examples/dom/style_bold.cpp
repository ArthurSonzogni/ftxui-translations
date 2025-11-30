// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <ftxui/dom/elements.hpp>  // para text, operator|, bold, Fit, hbox, Element
#include <ftxui/screen/screen.hpp>  // para Full, Screen
#include <memory>                   // para allocator

#include "ftxui/dom/node.hpp"      // para Render
#include "ftxui/screen/color.hpp"  // para ftxui

int main() {
  using namespace ftxui;
  auto document =  //
      hbox({
          text("Este texto es "),
          text("negrita") | bold,
          text(". ¿Te gusta?"),
      });
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}