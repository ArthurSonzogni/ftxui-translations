// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスによって管理されます。
#ifndef FTXUI_DOM_FLEXBOX_HELPER_HPP
#define FTXUI_DOM_FLEXBOX_HELPER_HPP

#include <vector>
#include "ftxui/dom/flexbox_config.hpp"

namespace ftxui::flexbox_helper {

// ブロックはフレックスボックス内の長方形です。
struct Block {
  // 入力:
  int min_size_x = 0;
  int min_size_y = 0;
  int flex_grow_x = 0;
  int flex_grow_y = 0;
  int flex_shrink_x = 0;
  int flex_shrink_y = 0;

  // 出力:
  int line{};
  int line_position{};
  int x = 0;
  int y = 0;
  int dim_x = 0;
  int dim_y = 0;
  bool overflow = false;
};

// ラインはブロックの行です。
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

#endif /* インクルードガードの終わり: FTXUI_DOM_FLEXBOX_HELPER_HPP */
