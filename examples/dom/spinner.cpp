// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款的約束，該條款可在 LICENSE 檔案中找到。
#include <chrono>                  // for operator""s, chrono_literals
#include <ftxui/dom/elements.hpp>  // for Element, operator|, separator, filler, hbox, size, spinner, text, vbox, bold, border, Fit, EQUAL, WIDTH
#include <ftxui/screen/screen.hpp>  // for Full, Screen
#include <iostream>                 // for cout, endl, ostream
#include <string>                   // for to_string, operator<<, string
#include <thread>                   // for sleep_for
#include <utility>                  // for move
#include <vector>                   // for vector

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui

int main() {
  using namespace ftxui;
  using namespace std::chrono_literals;

  std::string reset_position;
  for (int index = 0; index < 200; ++index) {
    std::vector<Element> entries;
    for (int i = 0; i < 23; ++i) {
      if (i != 0) {
        entries.push_back(separator());
      }
      entries.push_back(  //
          hbox({
              text(std::to_string(i)) | size(WIDTH, EQUAL, 2),
              separator(),
              spinner(i, index) | bold,
          }));
    }
    auto document = hbox({
        vbox(std::move(entries)) | border,
        filler(),
    });
    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);
    std::cout << reset_position;
    screen.Print();
    reset_position = screen.ResetPosition();

    std::this_thread::sleep_for(0.1s);
  }
  std::cout << std::endl;
}
