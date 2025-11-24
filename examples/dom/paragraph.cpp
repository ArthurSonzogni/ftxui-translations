// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱 `LICENSE` 檔案。
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
      R"(在機率論與統計學中，貝氏定理（或稱貝氏法則、貝氏規律）描述了一個事件的機率，其基礎是與該事件可能相關的條件的先驗知識。例如，如果癌症與年齡相關，那麼使用貝氏定理，一個人的年齡可以用來更準確地評估他們患癌症的機率，而不是在不知道該人年齡的情況下評估患癌症的機率。貝氏定理的眾多應用之一是貝氏推論，這是一種特殊的統計推論方法。當應用時，貝氏定理中涉及的機率可能具有不同的機率解釋。在貝氏機率解釋中，該定理表達了主觀的信念程度應如何理性地改變，以考慮相關證據的可用性。貝氏推論是貝氏統計學的基礎。)";

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
