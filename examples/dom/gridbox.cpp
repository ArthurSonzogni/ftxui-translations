// 版权所有 2021 Arthur Sonzogni。保留所有权利。
// 此源代码的使用受 MIT 许可协议的约束，该协议可在
// LICENSE 文件中找到。
#include <stdio.h>  // 用于 getchar
#include <ftxui/dom/elements.hpp>  // 用于 Elements, gridbox, Fit, operator|, text, border, Element
#include <ftxui/screen/screen.hpp>  // 用于 Screen
#include <memory>                   // 用于 allocator, shared_ptr

#include "ftxui/dom/node.hpp"      // 用于 Render
#include "ftxui/screen/color.hpp"  // 用于 ftxui

int main() {
  using namespace ftxui;
  auto cell = [](const char* t) { return text(t) | border; };
  auto document =  //
      gridbox({
          {
              cell("north-west"),
              cell("north"),
              cell("north-east"),
          },
          {
              cell("center-west"),
              gridbox({
                  {
                      cell("center-north-west"),
                      cell("center-north-east"),
                  },
                  {
                      cell("center-south-west"),
                      cell("center-south-east"),
                  },
              }),
              cell("center-east"),
          },
          {
              cell("south-west"),
              cell("south"),
              cell("south-east"),
          },
      });
  auto screen = Screen::Create(Dimension::Fit(document));
  Render(screen, document);
  screen.Print();
  getchar();

  return 0;
}
