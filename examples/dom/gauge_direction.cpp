// Copyright 2022 Arthur Sonzogni. All rights reserved. (日本語訳: 無断転載を禁じます。)
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスによって管理されています。
// (Use of this source code is governed by the MIT license that can be found in the LICENSE file.)
#include <chrono>                  // operator""s, chrono_literals のため
#include <ftxui/dom/elements.hpp>  // filler, operator|, separator, text, border, Element, vbox, vtext, hbox, center, gaugeDown, gaugeLeft, gaugeRight, gaugeUp のため
#include <ftxui/screen/screen.hpp>  // Screen のため
#include <iostream>                 // cout, endl, ostream のため
#include <string>  // allocator, operator+, operator<<, string, to_string のため
#include <thread>  // sleep_for のため

#include "ftxui/dom/node.hpp"      // Render のため
#include "ftxui/screen/color.hpp"  // ftxui のため

int main() {
  using namespace ftxui;
  using namespace std::chrono_literals;

  std::string reset_position;
  for (float percentage = 0.0f; percentage <= 1.0f; percentage += 0.002f) {
    std::string data_downloaded =
        std::to_string(int(percentage * 5000)) + "/5000";

    auto gauge_up =  //
        hbox({
            vtext("gauge vertical"),
            separator(),
            gaugeUp(percentage),
        }) |
        border;

    auto gauge_down =  //
        hbox({
            vtext("gauge vertical"),
            separator(),
            gaugeDown(percentage),
        }) |
        border;

    auto gauge_right =  //
        vbox({
            text("gauge horizontal"),
            separator(),
            gaugeRight(percentage),
        }) |
        border;

    auto gauge_left =  //
        vbox({
            text("gauge horizontal"),
            separator(),
            gaugeLeft(percentage),
        }) |
        border;

    auto document = hbox({
        gauge_up,
        filler(),
        vbox({
            gauge_right,
            filler(),
            text(data_downloaded) | border | center,
            filler(),
            gauge_left,
        }),
        filler(),
        gauge_down,
    });

    auto screen = Screen(32, 16);
    Render(screen, document);
    std::cout << reset_position;
    screen.Print();
    reset_position = screen.ResetPosition();

    std::this_thread::sleep_for(0.01s);
  }
  std::cout << std::endl;
}
