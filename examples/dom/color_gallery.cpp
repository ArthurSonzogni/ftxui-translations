// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスによって管理されています。
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
      text("16色パレット:"),
      separator(),
      hbox(
        vbox(
          color(Color::Default, text("デフォルト")),
          color(Color::Black, text("黒")),
          color(Color::GrayDark, text("濃い灰色")),
          color(Color::GrayLight, text("薄い灰色")),
          color(Color::White, text("白")),
          color(Color::Blue, text("青")),
          color(Color::BlueLight, text("水色")),
          color(Color::Cyan, text("シアン")),
          color(Color::CyanLight, text("明るいシアン")),
          color(Color::Green, text("緑")),
          color(Color::GreenLight, text("明るい緑")),
          color(Color::Magenta, text("マゼンタ")),
          color(Color::MagentaLight, text("明るいマゼンタ")),
          color(Color::Red, text("赤")),
          color(Color::RedLight, text("明るい赤")),
          color(Color::Yellow, text("黄")),
          color(Color::YellowLight, text("明るい黄"))
        ),
        vbox(
          bgcolor(Color::Default, text("デフォルト")),
          bgcolor(Color::Black, text("黒")),
          bgcolor(Color::GrayDark, text("濃い灰色")),
          bgcolor(Color::GrayLight, text("薄い灰色")),
          bgcolor(Color::White, text("白")),
          bgcolor(Color::Blue, text("青")),
          bgcolor(Color::BlueLight, text("水色")),
          bgcolor(Color::Cyan, text("シアン")),
          bgcolor(Color::CyanLight, text("明るいシアン")),
          bgcolor(Color::Green, text("緑")),
          bgcolor(Color::GreenLight, text("明るい緑")),
          bgcolor(Color::Magenta, text("マゼンタ")),
          bgcolor(Color::MagentaLight, text("明るいマゼンタ")),
          bgcolor(Color::Red, text("赤")),
          bgcolor(Color::RedLight, text("明るい赤")),
          bgcolor(Color::Yellow, text("黄")),
          bgcolor(Color::YellowLight, text("明るい黄"))
        )
      )
    );

  // clang-format on
  auto palette_256_color_display = text("256色パレット:");
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

  // トゥルーカラー表示。
  auto true_color_display = text("トゥルーカラー: 24ビット:");
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
              ? text(" 16色パレット対応: はい")
              : text(" 16色パレット対応: いいえ"),
          Terminal::ColorSupport() >= Terminal::Color::Palette256
              ? text("256色パレット対応: はい")
              : text("256色パレット対応: いいえ"),
          Terminal::ColorSupport() >= Terminal::Color::TrueColor
              ? text("       トゥルーカラー対応: はい")
              : text("       トゥルーカラー対応: いいえ"),
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
