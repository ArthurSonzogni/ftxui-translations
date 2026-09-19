// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/dom/box_helper.hpp"

#include <algorithm>  // for max
#include <cstdint>
#include <vector>  // for vector

namespace ftxui::box_helper {

namespace {

int SafeRatio(int value, int numerator, int denominator) {
  return static_cast<int>(
      static_cast<int64_t>(value) * static_cast<int64_t>(numerator) /
      std::max(static_cast<int64_t>(denominator), static_cast<int64_t>(1)));
}

// 許可されたサイズが要求されたサイズより大きい場合に呼ばれる。これは
// 余分なスペースを、相対的な比率でフレキシブルな要素へ分配する。
void ComputeGrow(std::vector<Element>* elements,
                 int extra_space,
                 int flex_grow_sum) {
  for (Element& element : *elements) {
    const int added_space =
        SafeRatio(extra_space, element.flex_grow, flex_grow_sum);
    extra_space -= added_space;
    flex_grow_sum -= element.flex_grow;
    element.size = element.min_size + added_space;
  }
}

// 許可されたサイズが要求されたサイズより小さく、縮小可能な要素が
// (負の)余分なスペースを吸収できる場合に呼ばれる。これは余分なスペースを
// それらの要素に分配する。
void ComputeShrinkEasy(std::vector<Element>* elements,
                       int extra_space,
                       int flex_shrink_sum) {
  for (Element& element : *elements) {
    const int added_space = SafeRatio(
        extra_space, element.min_size * element.flex_shrink, flex_shrink_sum);
    extra_space -= added_space;
    flex_shrink_sum -= element.flex_shrink * element.min_size;
    element.size = element.min_size + added_space;
  }
}

// 許可されたサイズが要求されたサイズより小さく、縮小可能な要素が
// (負の)余分なスペースを吸収できない場合に呼ばれる。縮小可能な要素には
// ゼロを割り当て、残りの(負の)余分なスペースを他の縮小不可能な要素に
// 分配する。
void ComputeShrinkHard(std::vector<Element>* elements,
                       int extra_space,
                       int size) {
  for (Element& element : *elements) {
    if (element.flex_shrink != 0) {
      element.size = 0;
      continue;
    }

    const int added_space = SafeRatio(extra_space, element.min_size, size);

    extra_space -= added_space;
    size -= element.min_size;

    element.size = element.min_size + added_space;
  }
}

}  // namespace

void Compute(std::vector<Element>* elements, int target_size) {
  int size = 0;
  int flex_grow_sum = 0;
  int flex_shrink_sum = 0;
  int flex_shrink_size = 0;

  for (auto& element : *elements) {
    flex_grow_sum += element.flex_grow;
    flex_shrink_sum += element.min_size * element.flex_shrink;
    if (element.flex_shrink != 0) {
      flex_shrink_size += element.min_size;
    }
    size += element.min_size;
  }

  const int extra_space = target_size - size;
  if (extra_space >= 0) {
    ComputeGrow(elements, extra_space, flex_grow_sum);
  } else if (flex_shrink_size + extra_space >= 0) {
    ComputeShrinkEasy(elements, extra_space, flex_shrink_sum);

  } else {
    ComputeShrinkHard(elements, extra_space + flex_shrink_size,
                      size - flex_shrink_size);
  }
}

}  // namespace ftxui::box_helper
