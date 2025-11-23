// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#include <chrono>                  // pour operator""s, chrono_literals
#include <ftxui/dom/elements.hpp>  // pour Element, operator|, separator, filler, hbox, size, spinner, text, vbox, bold, border, Fit, EQUAL, WIDTH
#include <ftxui/screen/screen.hpp>  // pour Full, Screen
#include <iostream>                 // pour cout, endl, ostream
#include <string>                   // pour to_string, operator<<, string
#include <thread>                   // pour sleep_for
#include <utility>                  // pour move
#include <vector>                   // pour vector

#include "ftxui/dom/node.hpp"      // pour Render
#include "ftxui/screen/color.hpp"  // pour ftxui

int main() {
  using namespace ftxui;
  using namespace std::chrono_literals;

  std::string reset_position;
  for (int index = 0; index < 200; ++index) {
    std::vector<Element> entries;
    for (int i = 0; i < 23; ++i) {
      if (i != 0) {
        entries.push_back(separator());
      }
      entries.push_back(  //
          hbox({
              text(std::to_string(i)) | size(WIDTH, EQUAL, 2),
              separator(),
              spinner(i, index) | bold,
          }));
    }
    auto document = hbox({
        vbox(std::move(entries)) | border,
        filler(),
    });
    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);
    std::cout << reset_position;
    screen.Print();
    reset_position = screen.ResetPosition();

    std::this_thread::sleep_for(0.1s);
  }
  std::cout << std::endl;
}
