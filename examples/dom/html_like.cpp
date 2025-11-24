// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスに従って行われます。
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
            paragraph("こんにちは世界！ここに画像があります:"), img1(),
            paragraph(" ここにテキストがあります "), text("underlined ") | underlined,
            paragraph(" ここにテキストがあります "), text("bold ") | bold,
            paragraph("こんにちは世界！ここに画像があります:"), img2(),
            paragraph(
                "Lorem Ipsumは、印刷前の組版とレイアウトに使用される単なるダミーテキストです。Lorem Ipsumは、1500年代に匿名の印刷業者が活字見本帳を作成するためにテキストの断片を組み合わせて以来、印刷業界の標準ダミーテキストです。それは5世紀以上にわたって生き残っただけでなく、その内容が変更されることなく、電子組版にも適応しました。1960年代には、Lorem Ipsumのパッセージを含むLetrasetシートのリリースによって普及し、さらに最近では、Aldus PageMakerのようなデスクトップパブリッシングアプリケーションにそのバージョンが組み込まれています。"),
            paragraph(" ここにテキストがあります "), text("dim ") | dim,
            paragraph("こんにちは世界！ここに画像があります:"), img1(),
            paragraph(" ここにテキストがあります "), text("red ") | color(Color::Red),
            paragraph(" スピナー "), spinner(6, i / 10)) |
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
