// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，該條款可在 LICENSE 檔案中找到。
#include <ftxui/dom/elements.hpp>  // 為了 hbox, text, bgcolor, operator|, vbox, Elements, window, Element, Fit
#include <ftxui/screen/screen.hpp>  // 為了 Full, Screen
#include <memory>                   // 為了 allocator
#include <utility>                  // 為了 move

#include "ftxui/dom/node.hpp"      // 為了 Render
#include "ftxui/screen/color.hpp"  // 為了 Color, ftxui

int main() {
  using namespace ftxui;

  Elements red_line;
  Elements green_line;
  Elements blue_line;
  Elements cyan_line;
  Elements magenta_line;
  Elements yellow_line;

  for (int value = 0; value < 255; value += 3) {
    int v = value * value / 255;
    red_line.push_back(text(" ") | bgcolor(Color::RGB(v, 0, 0)));
    green_line.push_back(text(" ") | bgcolor(Color::RGB(0, v, 0)));
    blue_line.push_back(text(" ") | bgcolor(Color::RGB(0, 0, v)));
    cyan_line.push_back(text(" ") | bgcolor(Color::RGB(0, v, v)));
    magenta_line.push_back(text(" ") | bgcolor(Color::RGB(v, 0, v)));
    yellow_line.push_back(text(" ") | bgcolor(Color::RGB(v, v, 0)));
  }

  auto document = vbox({
      window(text("原色"),
             vbox({
                 hbox({text("紅色線    :"), hbox(std::move(red_line))}),
                 hbox({text("綠色線  :"), hbox(std::move(green_line))}),
                 hbox({text("藍色線   :"), hbox(std::move(blue_line))}),
             })),
      window(text("輔色"),
             vbox({
                 hbox({text("青色線   :"), hbox(std::move(cyan_line))}),
                 hbox({text("洋紅色線:"), hbox(std::move(magenta_line))}),
                 hbox({text("黃色線 :"), hbox(std::move(yellow_line))}),
             })),
  });

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);

  screen.Print();

  return 0;
}
