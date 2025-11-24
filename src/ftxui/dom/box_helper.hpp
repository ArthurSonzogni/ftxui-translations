// Copyright 2021 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスによって管理されています。
// LICENSEファイルを参照してください。
#ifndef FTXUI_DOM_BOX_HELPER_HPP
#define FTXUI_DOM_BOX_HELPER_HPP

#include <vector>
#include "ftxui/dom/requirement.hpp"

namespace ftxui::box_helper {

struct Element {
  // 入力:
  int min_size = 0;
  int flex_grow = 0;
  int flex_shrink = 0;

  // 出力;
  int size = 0;
};

void Compute(std::vector<Element>* elements, int target_size);
}  // namespace ftxui::box_helper

#endif /* インクルードガードの終了: FTXUI_DOM_BOX_HELPER_HPP */