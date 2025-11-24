// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，該條款可在 LICENSE 檔案中找到。
#include <ftxui/dom/elements.hpp>  // 用於 text, operator|, underlined, Fit, hbox, Element
#include <ftxui/screen/screen.hpp>  // 用於 Full, Screen
#include <memory>                   // 用於 allocator

#include "ftxui/dom/node.hpp"      // 用於 Render
#include "ftxui/screen/color.hpp"  // 用於 ftxui

int main() {
  using namespace ftxui;
  auto document =  //
      hbox({
          text("這段文字是 "),
          text("加底線"),
          text("。你喜歡它嗎？"),
      });
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}
