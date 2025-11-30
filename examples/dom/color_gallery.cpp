// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <ftxui/screen/color_info.hpp>  // para ColorInfo
#include <ftxui/screen/screen.hpp>      // para Full, Screen
#include <ftxui/screen/terminal.hpp>  // para ColorSupport, Color, Palette16, Palette256, TrueColor
#include <memory>                     // para allocator, shared_ptr
#include <utility>                    // para move
#include <vector>                     // para vector

#include "ftxui/dom/elements.hpp"  // para text, bgcolor, color, vbox, hbox, separator, operator|, Elements, Element, Fit, border
#include "ftxui/dom/node.hpp"      // para Render
#include "ftxui/screen/color.hpp"  // para Color, Color::Black, Color::Blue, Color::BlueLight, Color::Cyan, Color::CyanLight, Color::Default, Color::GrayDark, Color::GrayLight, Color::Green, Color::GreenLight, Color::Magenta, Color::MagentaLight, Color::Red, Color::RedLight, Color::White, Color::Yellow, Color::YellowLight, Color::Palette256, ftxui

using namespace ftxui;
#include "./color_info_sorted_2d.ipp"  // para ColorInfoSorted2D

int main() {
  // clang-format off
  auto basic_color_display =
    vbox(
      text("Paleta de 16 colores:"),
      separator(),
      hbox(
        vbox(
          color(Color::Default, text("Default")),
          color(Color::Black, text("Black")),
          color(Color::GrayDark, text("GrayDark")),
          color(Color::GrayLight, text("GrayLight")),
          color(Color::White, text("White")),
          color(Color::Blue, text("Blue")),
          color(Color::BlueLight, text("BlueLight")),
          color(Color::Cyan, text("Cyan")),
          color(Color::CyanLight, text("CyanLight")),
          color(Color::Green, text("Green")),
          color(Color::GreenLight, text("GreenLight")),
          color(Color::Magenta, text("Magenta")),
          color(Color::MagentaLight, text("MagentaLight")),
          color(Color::Red, text("Red")),
          color(Color::RedLight, text("RedLight")),
          color(Color::Yellow, text("Yellow")),
          color(Color::YellowLight, text("YellowLight"))
        ),
        vbox(
          bgcolor(Color::Default, text("Default")),
          bgcolor(Color::Black, text("Black")),
          bgcolor(Color::GrayDark, text("GrayDark")),
          bgcolor(Color::GrayLight, text("GrayLight")),
          bgcolor(Color::White, text("White")),
          bgcolor(Color::Blue, text("Blue")),
          bgcolor(Color::BlueLight, text("BlueLight")),
          bgcolor(Color::Cyan, text("Cyan")),
          bgcolor(Color::CyanLight, text("CyanLight")),
          bgcolor(Color::Green, text("Green")),
          bgcolor(Color::GreenLight, text("GreenLight")),
          bgcolor(Color::Magenta, text("Magenta")),
          bgcolor(Color::MagentaLight, text("MagentaLight")),
          bgcolor(Color::Red, text("Red")),
          bgcolor(Color::RedLight, text("RedLight")),
          bgcolor(Color::Yellow, text("Yellow")),
          bgcolor(Color::YellowLight, text("YellowLight"))
        )
      )
    );

  // clang-format on
  auto palette_256_color_display = text("Paleta de 256 colores:");
  {
    std::vector<std::vector<ColorInfo>> info_columns = ColorInfoSorted2D();
    Elements columns;
    for (auto& column : info_columns) {
      Elements column_elements;
      for (auto& it : column) {
        column_elements.push_back(
            text("   ") | bgcolor(Color(Color::Palette256(it.index_256))));
      }
      columns.push_back(hbox(std::move(column_elements)));
    }
    palette_256_color_display = vbox({
        palette_256_color_display,
        separator(),
        vbox(columns),
    });
  }

  // Visualización de color verdadero.
  auto true_color_display = text("Colores verdaderos: 24bits:");
  {
    const int max_value = 255;
    const int value_increment = 8;
    const int hue_increment = 6;
    int saturation = max_value;
    Elements array;
    for (int value = 0; value < max_value; value += 2 * value_increment) {
      Elements line;
      for (int hue = 0; hue < max_value; hue += hue_increment) {
        line.push_back(
            text("▀")                                    //
            | color(Color::HSV(hue, saturation, value))  //
            | bgcolor(Color::HSV(hue, saturation, value + value_increment)));
      }
      array.push_back(hbox(std::move(line)));
    }
    true_color_display = vbox({
        true_color_display,
        separator(),
        vbox(std::move(array)),
    });
  }

  auto terminal_info =
      vbox({
          Terminal::ColorSupport() >= Terminal::Color::Palette16
              ? text(" Soporte de paleta de 16 colores : Sí")
              : text(" Soporte de paleta de 16 colores : No"),
          Terminal::ColorSupport() >= Terminal::Color::Palette256
              ? text("Soporte de paleta de 256 colores : Sí")
              : text("Soporte de paleta de 256 colores : No"),
          Terminal::ColorSupport() >= Terminal::Color::TrueColor
              ? text("       Soporte de color verdadero : Sí")
              : text("       Soporte de color verdadero : No"),
      }) |
      border;

  auto document = vbox({hbox({
                            basic_color_display,
                            text(" "),
                            palette_256_color_display,
                            text(" "),
                            true_color_display,
                        }),
                        terminal_info});
  // clang-format on

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);

  screen.Print();

  return 0;
}
