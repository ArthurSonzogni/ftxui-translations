// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <chrono>                   // for operator""s, chrono_literals
#include <ftxui/screen/screen.hpp>  // for Full, Screen
#include <iostream>                 // for cout, ostream
#include <memory>                   // for allocator, shared_ptr
#include <string>                   // for string, operator<<
#include <thread>                   // for sleep_for

#include "ftxui/dom/elements.hpp"  // for hflow, paragraph, separator, hbox, vbox, filler, operator|, border, Element
#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/box.hpp"    // for ftxui

using namespace std::chrono_literals;
int main() {
  using namespace ftxui;
  std::string p =
      R"(En théorie des probabilités et en statistique, le théorème de Bayes (alternativement loi de Bayes ou règle de Bayes) décrit la probabilité d'un événement, basée sur une connaissance préalable des conditions qui pourraient être liées à l'événement. Par exemple, si le cancer est lié à l'âge, alors, en utilisant le théorème de Bayes, l'âge d'une personne peut être utilisé pour évaluer plus précisément la probabilité qu'elle ait un cancer, par rapport à l'évaluation de la probabilité de cancer faite sans connaissance de l'âge de la personne. L'une des nombreuses applications du théorème de Bayes est l'inférence bayésienne, une approche particulière de l'inférence statistique. Lorsqu'elles sont appliquées, les probabilités impliquées dans le théorème de Bayes peuvent avoir différentes interprétations de probabilité. Avec l'interprétation bayésienne de la probabilité, le théorème exprime comment un degré de croyance subjectif devrait rationnellement changer pour tenir compte de la disponibilité de preuves connexes. L'inférence bayésienne est fondamentale pour les statistiques bayésiennes.)";

  std::string reset_position;
  while (true) {
    auto document = vbox({
                        hflow(paragraph(p)),
                        separator(),
                        hflow(paragraph(p)),
                        separator(),
                        hbox({
                            hflow(paragraph(p)),
                            separator(),
                            hflow(paragraph(p)),
                        }),
                    }) |
                    border;

    document = vbox(filler(), document);

    // auto screen = Screen::Create(Dimension::Fit(document));
    // Render(screen, document);
    // screen.Print();
    // getchar();

    auto screen = Screen::Create(Dimension::Full());
    Render(screen, document);
    std::cout << reset_position;
    screen.Print();
    reset_position = screen.ResetPosition();

    std::this_thread::sleep_for(0.01s);
  }

  return 0;
}
