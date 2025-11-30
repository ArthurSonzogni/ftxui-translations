// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este codigo fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
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
      R"(En la teoría de la probabilidad y la estadística, el teorema de Bayes (alternativamente ley de Bayes o regla de Bayes) describe la probabilidad de un evento, basándose en el conocimiento previo de las condiciones que podrían estar relacionadas con el evento. Por ejemplo, si el cáncer está relacionado con la edad, entonces, utilizando el teorema de Bayes, la edad de una persona puede utilizarse para evaluar con mayor precisión la probabilidad de que tenga cáncer, en comparación con la evaluación de la probabilidad de cáncer realizada sin el conocimiento de la edad de la persona. Una de las muchas aplicaciones del teorema de Bayes es la inferencia bayesiana, un enfoque particular de la inferencia estadística. Cuando se aplica, las probabilidades implicadas en el teorema de Bayes pueden tener diferentes interpretaciones de probabilidad. Con la interpretación de probabilidad bayesiana, el teorema expresa cómo un grado subjetivo de creencia debe cambiar racionalmente para tener en cuenta la disponibilidad de pruebas relacionadas. La inferencia bayesiana es fundamental para la estadística bayesiana.)";

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
