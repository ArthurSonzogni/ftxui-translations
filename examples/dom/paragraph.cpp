// 版权所有 2020 Arthur Sonzogni。保留所有权利。
// 本源代码的使用受 MIT 许可证约束，可在 LICENSE 文件中找到。
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
      R"(在概率论和统计学中，贝叶斯定理（或称贝叶斯法则、贝叶斯规则）描述了一个事件的概率，该概率基于与该事件相关的先验条件知识。例如，如果癌症与年龄相关，那么使用贝叶斯定理，一个人的年龄可以用来更准确地评估他们患癌症的概率，相比于在不了解其年龄的情况下对癌症概率的评估。贝叶斯定理的众多应用之一是贝叶斯推断，这是一种特殊的统计推断方法。应用时，贝叶斯定理中涉及的概率可能具有不同的概率解释。根据贝叶斯概率解释，该定理表达了主观信念程度应如何理性地改变以解释相关证据的可用性。贝叶斯推断是贝叶斯统计学的基础。)";

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
