// Copyright 2020 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、以下で見つけることができるMITライセンスによって管理されています。
// LICENSEファイル。
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
      R"(確率論および統計学において、ベイズの定理（またはベイズの法則、ベイズの規則）は、ある事象に関連する可能性のある条件の事前知識に基づいて、その事象の確率を記述します。例えば、癌が年齢と関連している場合、ベイズの定理を使用することで、その人の年齢を知らない状態での癌の確率の評価と比較して、その人が癌である確率をより正確に評価するために年齢を使用できます。ベイズの定理の多くの応用の1つは、統計的推論への特定のアプローチであるベイズ推論です。適用される場合、ベイズの定理に含まれる確率は、異なる確率解釈を持つことがあります。ベイズ確率解釈では、この定理は、関連する証拠の入手可能性を考慮して、主観的な信頼度が合理的に変化すべき方法を表現します。ベイズ推論は、ベイズ統計学の基礎です。) ";

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
