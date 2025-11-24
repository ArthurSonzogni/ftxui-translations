// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに準拠しています。
// LICENSEファイルに記載されているMITライセンスの条件に従って使用されます。
#include <stdio.h>                 // getcharのため
#include <ftxui/dom/elements.hpp>  // operator|, size, Element, text, hcenter, Decorator, Fit, WIDTH, hflow, window, EQUAL, GREATER_THAN, HEIGHT, bold, border, dim, LESS_THAN のため
#include <ftxui/screen/screen.hpp>  // Full, Screen のため
#include <string>  // allocator, char_traits, operator+, to_string, string のため

#include "ftxui/dom/node.hpp"      // Renderのため
#include "ftxui/screen/color.hpp"  // ftxuiのため

int main() {
  using namespace ftxui;
  auto make_box = [](int dimx, int dimy) {
    std::string title = std::to_string(dimx) + "x" + std::to_string(dimy);
    return window(text(title) | hcenter | bold,
                  text("content") | hcenter | dim) |
           size(WIDTH, EQUAL, dimx) | size(HEIGHT, EQUAL, dimy);
  };

  auto style = size(WIDTH, GREATER_THAN, 20) | border |
               size(HEIGHT, GREATER_THAN, 30) | size(WIDTH, LESS_THAN, 50);

  auto document = hflow({
                      make_box(7, 7),
                      make_box(7, 5),
                      make_box(5, 7),
                      make_box(10, 4),
                      make_box(10, 4),
                      make_box(10, 4),
                      make_box(10, 4),
                      make_box(11, 4),
                      make_box(11, 4),
                      make_box(11, 4),
                      make_box(11, 4),
                      make_box(12, 4),
                      make_box(12, 5),
                      make_box(12, 4),
                      make_box(13, 4),
                      make_box(13, 3),
                      make_box(13, 3),
                      make_box(10, 3),
                  }) |
                  style;

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();
  getchar();

  return 0;
}
