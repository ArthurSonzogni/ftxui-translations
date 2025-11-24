// Copyright 2020 Arthur Sonzogni. 版權所有。
// 本原始碼受 MIT 授權條款約束，詳情請見
// LICENSE 文件。
#include <ftxui/dom/elements.hpp>  // 用於 text, operator|, blink, Fit, hbox, Element
#include <ftxui/screen/screen.hpp>  // 用於 Full, Screen
#include <memory>                   // 用於 allocator

#include "ftxui/dom/node.hpp"      // 用於 Render
#include "ftxui/screen/color.hpp"  // 用於 ftxui

int main() {
  using namespace ftxui;
  auto document =  //
      hbox({
          text("這段文字是 "),
          text("閃爍") | blink,
          text("。你喜歡嗎？"),
      });
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}