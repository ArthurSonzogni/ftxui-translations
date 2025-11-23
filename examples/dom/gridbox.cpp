// Copyright 2021 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <stdio.h>  // pour getchar
#include <ftxui/dom/elements.hpp>  // pour Elements, gridbox, Fit, operator|, text, border, Element
#include <ftxui/screen/screen.hpp>  // pour Screen
#include <memory>                   // pour allocator, shared_ptr

#include "ftxui/dom/node.hpp"      // pour Render
#include "ftxui/screen/color.hpp"  // pour ftxui

int main() {
  using namespace ftxui;
  auto cell = [](const char* t) { return text(t) | border; };
  auto document =  //
      gridbox({
          {
              cell("north-west"),
              cell("north"),
              cell("north-east"),
          },
          {
              cell("center-west"),
              gridbox({
                  {
                      cell("center-north-west"),
                      cell("center-north-east"),
                  },
                  {
                      cell("center-south-west"),
                      cell("center-south-east"),
                  },
              }),
              cell("center-east"),
          },
          {
              cell("south-west"),
              cell("south"),
              cell("south-east"),
          },
      });
  auto screen = Screen::Create(Dimension::Fit(document));
  Render(screen, document);
  screen.Print();
  getchar();

  return 0;
}