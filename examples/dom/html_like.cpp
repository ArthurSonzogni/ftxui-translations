// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼受 MIT 授權條款約束，詳情請參閱
// LICENSE 檔案。
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
            paragraph("哈囉世界！這是一張圖片："), img1(),
            paragraph(" 這是一段文字 "), text("加底線的 ") | underlined,
            paragraph(" 這是一段文字 "), text("粗體的 ") | bold,
            paragraph("哈囉世界！這是一張圖片："), img2(),
            paragraph(
                "Lorem Ipsum 僅是排版和印刷前的假文。Lorem Ipsum 自 15 世紀以來一直是印刷業的標準假文，當時一位匿名印刷商將不同的文字片段組合成一本字體樣本。它不僅存活了五個世紀，還適應了電子排版，內容卻未曾改變。它在 1960 年代因 Letraset 公司發售含有 Lorem Ipsum 段落的字體樣本而廣受歡迎，最近則因其被包含在 Aldus PageMaker 等桌面出版應用程式中而再次普及。"),
            paragraph(" 這是一段文字 "), text("暗淡的 ") | dim,
            paragraph("哈囉世界！這是一張圖片："), img1(),
            paragraph(" 這是一段文字 "), text("紅色的 ") | color(Color::Red),
            paragraph(" 一個旋轉器 "), spinner(6, i / 10)) |
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