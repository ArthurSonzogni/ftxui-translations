// Copyright 2020 Arthur Sonzogni. All rights reserved.
// 本源代码的使用受可在 LICENSE 文件中找到的 MIT 许可协议的约束。
#ifndef FTXUI_DOM_FLEXBOX_HELPER_HPP
#define FTXUI_DOM_FLEXBOX_HELPER_HPP

#include <vector>
#include "ftxui/dom/flexbox_config.hpp"

namespace ftxui::flexbox_helper {

// 块是flexbox中的一个矩形。
struct Block {
  // 输入:
  int min_size_x = 0;
  int min_size_y = 0;
  int flex_grow_x = 0;
  int flex_grow_y = 0;
  int flex_shrink_x = 0;
  int flex_shrink_y = 0;

  // 输出:
  int line{};
  int line_position{};
  int x = 0;
  int y = 0;
  int dim_x = 0;
  int dim_y = 0;
  bool overflow = false;
};

// 行是块的行。
struct Line {
  std::vector<Block*> blocks;
  int x = 0;
  int y = 0;
  int dim_x = 0;
  int dim_y = 0;
};

struct Global {
  std::vector<Block> blocks;
  std::vector<Line> lines;
  FlexboxConfig config;
  int size_x;
  int size_y;
};

void Compute(Global& global);

}  // namespace ftxui::flexbox_helper

#endif /* include guard 结束: FTXUI_DOM_FLEXBOX_HELPER_HPP*/
