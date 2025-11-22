// 版权所有 2020 Arthur Sonzogni。保留所有权利。
// 此源代码的使用受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
#include <ftxui/dom/elements.hpp>  // for text, bgcolor, hbox, operator|, Elements, Fit, vbox, Element
#include <ftxui/screen/color_info.hpp>  // for ColorInfo
#include <ftxui/screen/screen.hpp>      // for Full, Screen
#include <utility>                      // for move
#include <vector>                       // for vector, allocator

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for Color, Color::Palette256, ftxui

using namespace ftxui;
#include "./color_info_sorted_2d.ipp"  // for ColorInfoSorted2D

int main() {
  std::vector<std::vector<ColorInfo>> info_columns = ColorInfoSorted2D();

  // 绘制每一列
  Elements columns_elements;
  for (auto& column : info_columns) {
    Elements column_elements;
    for (auto& it : column) {
      column_elements.push_back(hbox({
          text("     ") | bgcolor(Color(Color::Palette256(it.index_256))),
          text(it.name),
      }));
    }
    columns_elements.push_back(vbox(std::move(column_elements)));
  }
  auto document = hbox(std::move(columns_elements));

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);

  screen.Print();

  return 0;
}
