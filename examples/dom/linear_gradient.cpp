// Copyright 2023 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <ftxui/dom/elements.hpp>  // para bgcolor, operator|, operator|=, text, center, Element
#include <ftxui/dom/linear_gradient.hpp>  // para LinearGradient::Stop, LinearGradient
#include <ftxui/screen/screen.hpp>        // para Full, Screen
#include <memory>                         // para allocator, shared_ptr

#include "ftxui/dom/node.hpp"  // para Render
#include "ftxui/screen/color.hpp"  // para Color, Color::DeepPink1, Color::DeepSkyBlue1, Color::Yellow, ftxui

int main() {
  using namespace ftxui;
  auto document = text("gradient") | center;

  document |= bgcolor(LinearGradient()
                          .Angle(45)
                          .Stop(Color::DeepPink1)
                          .Stop(Color::DeepSkyBlue1));
  auto screen = Screen::Create(Dimension::Full(), Dimension::Full());
  Render(screen, document);
  screen.Print();

  return 0;
}
