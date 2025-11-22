// Copyright 2023 Arthur Sonzogni. All rights reserved.
// 本源代码的使用受 MIT 许可协议的约束，该协议可在 LICENSE 文件中找到。
#include <ftxui/dom/elements.hpp>  // 用于 bgcolor, operator|, operator|=, text, center, Element
#include <ftxui/dom/linear_gradient.hpp>  // 用于 LinearGradient::Stop, LinearGradient
#include <ftxui/screen/screen.hpp>        // 用于 Full, Screen
#include <memory>                         // 用于 allocator, shared_ptr

#include "ftxui/dom/node.hpp"  // 用于 Render
#include "ftxui/screen/color.hpp"  // 用于 Color, Color::DeepPink1, Color::DeepSkyBlue1, Color::Yellow, ftxui

int main() {
  using namespace ftxui;
  auto document = text("gradient") | center;

  document |= bgcolor(LinearGradient()
                          .Angle(45)
                          .Stop(Color::DeepPink1)
                          .Stop(Color::DeepSkyBlue1));
  auto screen = Screen::Create(Dimension::Full(), Dimension::Full());
  Render(screen, document);
  screen.Print();

  return 0;
}
