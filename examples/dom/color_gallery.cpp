// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <ftxui/screen/color_info.hpp>  // for ColorInfo
#include <ftxui/screen/screen.hpp>      // for Full, Screen
#include <ftxui/screen/terminal.hpp>  // for ColorSupport, Color, Palette16, Palette256, TrueColor
#include <memory>                     // for allocator, shared_ptr
#include <utility>                    // for move
#include <vector>                     // for vector

#include "ftxui/dom/elements.hpp"  // for text, bgcolor, color, vbox, hbox, separator, operator|, Elements, Element, Fit, border
#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for Color, Color::Black, Color::Blue, Color::BlueLight, Color::Cyan, Color::CyanLight, Color::Default, Color::GrayDark, Color::GrayLight, Color::Green, Color::GreenLight, Color::Magenta, Color::MagentaLight, Color::Red, Color::RedLight, Color::White, Color::Yellow, Color::YellowLight, Color::Palette256, ftxui

using namespace ftxui;
#include "./color_info_sorted_2d.ipp"  // for ColorInfoSorted2D

int main() {
  // clang-format off
  auto basic_color_display =
    vbox(
      text("Palette de 16 couleurs:"),
      separator(),
      hbox(
        vbox(
          color(Color::Default, text("Défaut")),
          color(Color::Black, text("Noir")),
          color(Color::GrayDark, text("GrisFoncé")),
          color(Color::GrayLight, text("GrisClair")),
          color(Color::White, text("Blanc")),
          color(Color::Blue, text("Bleu")),
          color(Color::BlueLight, text("BleuClair")),
          color(Color::Cyan, text("Cyan")),
          color(Color::CyanLight, text("CyanClair")),
          color(Color::Green, text("Vert")),
          color(Color::GreenLight, text("VertClair")),
          color(Color::Magenta, text("Magenta")),
          color(Color::MagentaLight, text("MagentaClair")),
          color(Color::Red, text("Rouge")),
          color(Color::RedLight, text("RougeClair")),
          color(Color::Yellow, text("Jaune")),
          color(Color::YellowLight, text("JauneClair"))
        ),
        vbox(
          bgcolor(Color::Default, text("Défaut")),
          bgcolor(Color::Black, text("Noir")),
          bgcolor(Color::GrayDark, text("GrisFoncé")),
          bgcolor(Color::GrayLight, text("GrisClair")),
          bgcolor(Color::White, text("Blanc")),
          bgcolor(Color::Blue, text("Bleu")),
          bgcolor(Color::BlueLight, text("BleuClair")),
          bgcolor(Color::Cyan, text("Cyan")),
          bgcolor(Color::CyanLight, text("CyanClair")),
          bgcolor(Color::Green, text("Vert")),
          bgcolor(Color::GreenLight, text("VertClair")),
          bgcolor(Color::Magenta, text("Magenta")),
          bgcolor(Color::MagentaLight, text("MagentaClair")),
          bgcolor(Color::Red, text("Rouge")),
          bgcolor(Color::RedLight, text("RougeClair")),
          bgcolor(Color::Yellow, text("Jaune")),
          bgcolor(Color::YellowLight, text("JauneClair"))
        )
      )
    );

  // clang-format on
  auto palette_256_color_display = text("Palette de 256 couleurs:");
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

  // Affichage des vraies couleurs.
  auto true_color_display = text("VraiesCouleurs: 24bits:");
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
              ? text(" Prise en charge de la palette 16 couleurs : Oui")
              : text(" Prise en charge de la palette 16 couleurs : Non"),
          Terminal::ColorSupport() >= Terminal::Color::Palette256
              ? text(" Prise en charge de la palette 256 couleurs : Oui")
              : text(" Prise en charge de la palette 256 couleurs : Non"),
          Terminal::ColorSupport() >= Terminal::Color::TrueColor
              ? text(" Prise en charge des vraies couleurs : Oui")
              : text(" Prise en charge des vraies couleurs : Non"),
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
