// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui se trouve dans
// le fichier LICENSE.
#include <ftxui/dom/elements.hpp>  // pour text, operator|, bold, Fit, hbox, Element
#include <ftxui/screen/screen.hpp>  // pour Full, Screen
#include <memory>                   // pour allocator

#include "ftxui/dom/node.hpp"      // pour Render
#include "ftxui/screen/color.hpp"  // pour ftxui

int main() {
  using namespace ftxui;
  auto document =  //
      hbox({
          text("This text is "),
          text("bold") | bold,
          text(". Do you like it?"),
      });
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}