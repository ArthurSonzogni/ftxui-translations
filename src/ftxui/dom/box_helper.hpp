// 版权所有 2021 Arthur Sonzogni。保留所有权利。
// 此源代码的使用受 MIT 许可证的约束，该许可证可在
// LICENSE 文件中找到。
#ifndef FTXUI_DOM_BOX_HELPER_HPP
#define FTXUI_DOM_BOX_HELPER_HPP

#include <vector>
#include "ftxui/dom/requirement.hpp"

namespace ftxui::box_helper {

struct Element {
  // 输入：
  int min_size = 0;
  int flex_grow = 0;
  int flex_shrink = 0;

  // 输出；
  int size = 0;
};

void Compute(std::vector<Element>* elements, int target_size);
}  // namespace ftxui::box_helper

#endif /* 包含守卫结束：FTXUI_DOM_BOX_HELPER_HPP */
