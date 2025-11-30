// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <chrono>                   // for operator""s, chrono_literals
#include <ftxui/screen/screen.hpp>  // for Screen
#include <iostream>                 // for cout, ostream
#include <string>                   // for allocator, operator<<, string
#include <thread>                   // for sleep_for

#include "ftxui/dom/elements.hpp"  // for paragraph, text, operator|, Element, border, Fit, color, hflow, spinner, vbox, bold, dim, underlined
#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/box.hpp"    // for ftxui
#include "ftxui/screen/color.hpp"  // for Color, Color::Red

int main() {
  using namespace ftxui;
  using namespace std::chrono_literals;

  auto img1 = []() { return text("img") | border; };
  auto img2 = []() { return vbox({text("big"), text("image")}) | border; };

  std::string reset_position;
  for (int i = 0;; ++i) {
    auto document =  //
        hflow(
            paragraph("¡Hola mundo! Aquí hay una imagen:"), img1(),
            paragraph(" Aquí hay un texto "), text("subrayado ") | underlined,
            paragraph(" Aquí hay un texto "), text("negrita ") | bold,
            paragraph("Hello world! Here is an image:"), img2(),
            paragraph(
                "Lorem Ipsum es simplemente un texto falso utilizado en la "
                "composición y maquetación antes de la impresión. Lorem "
                "Ipsum ha sido el texto de relleno estándar de la industria "
                "desde el año 1500, cuando un impresor desconocido tomó una "
                "galera de tipos y la mezcló de tal manera que logró hacer un "
                "libro de muestras tipográficas. No solo sobrevivió 500 años, "
                "sino que también se adaptó a la tipografía electrónica, "
                "permaneciendo esencialmente sin cambios. Fue popularizado en "
                "los años 60 con la publicación de hojas \"Letraset\" que "
                "contenían pasajes de Lorem Ipsum, y más recientemente con el "
                "software de autoedición, como Aldus PageMaker, que incluye "
                "versiones de Lorem Ipsum."),
            paragraph(" Aquí hay un texto "), text("tenue ") | dim,
            paragraph("¡Hola mundo! Aquí hay una imagen:"), img1(),
            paragraph(" Aquí hay un texto "), text("rojo ") | color(Color::Red),
            paragraph(" Un spinner "), spinner(6, i / 10)) |
        border;

    auto screen = Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    std::cout << reset_position;
    screen.Print();
    reset_position = screen.ResetPosition();

    std::this_thread::sleep_for(0.01s);
  }

  return 0;
}
