// 版權所有 2023 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，該條款可在 LICENSE 文件中找到。
// the LICENSE file.
#include <ftxui/dom/elements.hpp>  // 用於 bgcolor, operator|, operator|=, text, center, Element
#include <ftxui/dom/linear_gradient.hpp>  // 用於 LinearGradient::Stop, LinearGradient
#include <ftxui/screen/screen.hpp>        // 用於 Full, Screen
#include <memory>                         // 用於 allocator, shared_ptr

#include "ftxui/dom/node.hpp"  // 用於 Render
#include "ftxui/screen/color.hpp"  // 用於 Color, Color::DeepPink1, Color::DeepSkyBlue1, Color::Yellow, ftxui

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
