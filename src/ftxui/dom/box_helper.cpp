// Copyright 2021 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスによって管理されています。
#include "ftxui/dom/box_helper.hpp"

#include <algorithm>  // for max
#include <cstdint>
#include <vector>  // for vector

namespace ftxui::box_helper {

namespace {

int SafeRatio(int value, int numerator, int denominator) {
  return static_cast<int64_t>(value) * static_cast<int64_t>(numerator) /
         std::max(static_cast<int64_t>(denominator), static_cast<int64_t>(1));
}

// 許容されるサイズが要求されたサイズよりも大きい場合に呼び出されます。これは、
// 余分なスペースを柔軟な要素に相対的な比率で分配します。
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

// 許容されるサイズが要求されたサイズよりも小さい場合に呼び出され、
// 縮小可能な要素が（負の）余分なスペースを吸収できます。これは、
// その余分なスペースをそれらに分配します。
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

// 許容されるサイズが要求されたサイズよりも小さく、
// 縮小可能な要素が（負の）余分なスペースを吸収できない場合に呼び出されます。これは、
// 縮小可能な要素にゼロを割り当て、残りの（負の）
// 余分なスペースを他の縮小不可能な要素に分配します。
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
