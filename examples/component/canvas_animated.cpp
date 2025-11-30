// Copyright 2021 Arthur Sonzogni. All rights reserved.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSED.
#include <cmath>                   // for sin, cos
#include <ftxui/dom/elements.hpp>  // for canvas, Element, separator, hbox, operator|, border
#include <ftxui/screen/screen.hpp>  // for Pixel
#include <memory>   // for allocator, shared_ptr, __shared_ptr_access
#include <string>   // for string, basic_string
#include <utility>  // for move
#include <vector>   // for vector, __alloc_traits<>::value_type

#include "ftxui/component/component.hpp"  // for Renderer, CatchEvent, Horizontal, Menu, Tab
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/event.hpp"               // for Event
#include "ftxui/component/mouse.hpp"               // for Mouse
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/canvas.hpp"                    // for Canvas
#include "ftxui/screen/color.hpp"  // for Color, Color::Red, Color::Blue, Color::Green, ftxui

int main() {
  using namespace ftxui;

  int mouse_x = 0;
  int mouse_y = 0;

  // Un triángulo que sigue al ratón, usando caracteres braille.
  auto renderer_line_braille = Renderer([&] {
    auto c = Canvas(100, 100);
    c.DrawText(0, 0, "Varias líneas (braille)");
    c.DrawPointLine(mouse_x, mouse_y, 80, 10, Color::Red);
    c.DrawPointLine(80, 10, 80, 40, Color::Blue);
    c.DrawPointLine(80, 40, mouse_x, mouse_y, Color::Green);
    return canvas(std::move(c));
  });

  // Un triángulo que sigue al ratón, usando caracteres de bloque.
  auto renderer_line_block = Renderer([&] {
    auto c = Canvas(100, 100);
    c.DrawText(0, 0, "Varias líneas (bloque)");
    c.DrawBlockLine(mouse_x, mouse_y, 80, 10, Color::Red);
    c.DrawBlockLine(80, 10, 80, 40, Color::Blue);
    c.DrawBlockLine(80, 40, mouse_x, mouse_y, Color::Green);
    return canvas(std::move(c));
  });

  // Un círculo que sigue al ratón, usando caracteres braille.
  auto renderer_circle_braille = Renderer([&] {
    auto c = Canvas(100, 100);
    c.DrawText(0, 0, "Un círculo (braille)");
    c.DrawPointCircle(mouse_x, mouse_y, 30);
    return canvas(std::move(c));
  });

  // Un círculo que sigue al ratón, usando caracteres de bloque.
  auto renderer_circle_block = Renderer([&] {
    auto c = Canvas(100, 100);
    c.DrawText(0, 0, "Un círculo (bloque)");
    c.DrawBlockCircle(mouse_x, mouse_y, 30);
    return canvas(std::move(c));
  });

  // Un círculo relleno que sigue al ratón, usando caracteres braille.
  auto renderer_circle_filled_braille = Renderer([&] {
    auto c = Canvas(100, 100);
    c.DrawText(0, 0, "Un círculo relleno (braille)");
    c.DrawPointCircleFilled(mouse_x, mouse_y, 30);
    return canvas(std::move(c));
  });

  // Un círculo relleno que sigue al ratón, usando caracteres de bloque.
  auto renderer_circle_filled_block = Renderer([&] {
    auto c = Canvas(100, 100);
    c.DrawText(0, 0, "Un círculo relleno (bloque)");
    c.DrawBlockCircleFilled(mouse_x, mouse_y, 30);
    return canvas(std::move(c));
  });

  // Una elipse que sigue al ratón, usando caracteres braille.
  auto renderer_ellipse_braille = Renderer([&] {
    auto c = Canvas(100, 100);
    c.DrawText(0, 0, "Una elipse (braille)");
    c.DrawPointEllipse(mouse_x / 2, mouse_y / 2, mouse_x / 2, mouse_y / 2);
    return canvas(std::move(c));
  });

  // Una elipse que sigue al ratón, usando caracteres de bloque.
  auto renderer_ellipse_block = Renderer([&] {
    auto c = Canvas(100, 100);
    c.DrawText(0, 0, "Una elipse (bloque)");
    c.DrawBlockEllipse(mouse_x / 2, mouse_y / 2, mouse_x / 2, mouse_y / 2);
    return canvas(std::move(c));
  });

  // Una elipse rellena que sigue al ratón, usando caracteres braille.
  auto renderer_ellipse_filled_braille = Renderer([&] {
    auto c = Canvas(100, 100);
    c.DrawText(0, 0, "Una elipse rellena (braille)");
    c.DrawPointEllipseFilled(mouse_x / 2, mouse_y / 2, mouse_x / 2,
                             mouse_y / 2);
    return canvas(std::move(c));
  });

  // Una elipse rellena que sigue al ratón, usando caracteres de bloque.
  auto renderer_ellipse_filled_block = Renderer([&] {
    auto c = Canvas(100, 100);
    c.DrawText(0, 0, "Una elipse rellena (bloque)");
    c.DrawBlockEllipseFilled(mouse_x / 2, mouse_y / 2, mouse_x / 2,
                             mouse_y / 2);
    c.DrawBlockEllipse(mouse_x / 2, mouse_y / 2, mouse_x / 2, mouse_y / 2);
    return canvas(std::move(c));
  });

  // Un texto que sigue al ratón
  auto renderer_text = Renderer([&] {
    auto c = Canvas(100, 100);
    c.DrawText(0, 0, "Un trozo de texto");
    c.DrawText(mouse_x, mouse_y, "Esto es un trozo de texto con efectos",
               [](Pixel& p) {
                 p.foreground_color = Color::Red;
                 p.underlined = true;
                 p.bold = true;
               });
    return canvas(std::move(c));
  });

  auto renderer_plot_1 = Renderer([&] {
    auto c = Canvas(100, 100);
    c.DrawText(0, 0, "Un gráfico");

    std::vector<int> ys(100);
    for (int x = 0; x < 100; x++) {
      float dx = float(x - mouse_x);
      float dy = 50.f;
      ys[x] = int(dy + 20 * cos(dx * 0.14) + 10 * sin(dx * 0.42));
    }
    for (int x = 1; x < 99; x++) {
      c.DrawPointLine(x, ys[x], x + 1, ys[x + 1]);
    }

    return canvas(std::move(c));
  });

  auto renderer_plot_2 = Renderer([&] {
    auto c = Canvas(100, 100);
    c.DrawText(0, 0, "Un gráfico simétrico relleno");
    std::vector<int> ys(100);
    for (int x = 0; x < 100; x++) {
      ys[x] = int(30 +                                  //
                  10 * cos(x * 0.2 - mouse_x * 0.05) +  //
                  5 * sin(x * 0.4) +                    //
                  5 * sin(x * 0.3 - mouse_y * 0.05));   //
    }
    for (int x = 0; x < 100; x++) {
      c.DrawPointLine(x, 50 + ys[x], x, 50 - ys[x], Color::Red);
    }

    return canvas(std::move(c));
  });

  auto renderer_plot_3 = Renderer([&] {
    auto c = Canvas(100, 100);
    c.DrawText(0, 0, "Un gráfico gaussiano 2D");
    int size = 15;

    // mouse_x = 5mx + 3*my
    // mouse_y = 0mx + -5my + 90
    float my = (mouse_y - 90) / -5.f;
    float mx = (mouse_x - 3 * my) / 5.f;
    std::vector<std::vector<float>> ys(size, std::vector<float>(size));
    for (int y = 0; y < size; y++) {
      for (int x = 0; x < size; x++) {
        float dx = x - mx;
        float dy = y - my;
        ys[y][x] = -1.5 + 3.0 * std::exp(-0.2f * (dx * dx + dy * dy));
      }
    }
    for (int y = 0; y < size; y++) {
      for (int x = 0; x < size; x++) {
        if (x != 0) {
          c.DrawPointLine(
              5 * (x - 1) + 3 * (y - 0), 90 - 5 * (y - 0) - 5 * ys[y][x - 1],
              5 * (x - 0) + 3 * (y - 0), 90 - 5 * (y - 0) - 5 * ys[y][x]);
        }
        if (y != 0) {
          c.DrawPointLine(
              5 * (x - 0) + 3 * (y - 1), 90 - 5 * (y - 1) - 5 * ys[y - 1][x],
              5 * (x - 0) + 3 * (y - 0), 90 - 5 * (y - 0) - 5 * ys[y][x]);
        }
      }
    }

    return canvas(std::move(c));
  });

  int selected_tab = 12;
  auto tab = Container::Tab(
      {
          renderer_line_braille,
          renderer_line_block,
          renderer_circle_braille,
          renderer_circle_block,
          renderer_circle_filled_braille,
          renderer_circle_filled_block,
          renderer_ellipse_braille,
          renderer_ellipse_block,
          renderer_ellipse_filled_braille,
          renderer_ellipse_filled_block,

          renderer_plot_1,
          renderer_plot_2,
          renderer_plot_3,

          renderer_text,
      },
      &selected_tab);

  // Esto captura la última posición del ratón.
  auto tab_with_mouse = CatchEvent(tab, [&](Event e) {
    if (e.is_mouse()) {
      mouse_x = (e.mouse().x - 1) * 2;
      mouse_y = (e.mouse().y - 1) * 4;
    }
    return false;
  });

  std::vector<std::string> tab_titles = {
      "línea (braille)",
      "línea (bloque)",
      "círculo (braille)",
      "círculo (bloque)",
      "círculo relleno (braille)",
      "círculo relleno (bloque)",
      "elipse (braille)",
      "elipse (bloque)",
      "elipse rellena (braille)",
      "elipse rellena (bloque)",
      "trazado_1 simple",
      "trazado_2 relleno",
      "trazado_3 3D",
      "texto",
  };
  auto tab_toggle = Menu(&tab_titles, &selected_tab);

  auto component = Container::Horizontal({
      tab_with_mouse,
      tab_toggle,
  });

  // Agrega un separador para decorar todo el componente:
  auto component_renderer = Renderer(component, [&] {
    return hbox({
               tab_with_mouse->Render(),
               separator(),
               tab_toggle->Render(),
           }) |
           border;
  });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(component_renderer);

  return 0;
}