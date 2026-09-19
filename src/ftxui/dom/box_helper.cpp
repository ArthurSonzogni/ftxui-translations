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

// 當允許的大小大於所要求的大小時呼叫。這會
// 將額外的空間依比例分配給可伸縮的元素。
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

// 當允許的大小小於所要求的大小，且可縮減元素能
// 吸收（負的）額外空間時呼叫。這會將 extra_space
// 分配給這些元素。
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

// 當允許的大小小於所要求的大小，且可縮減元素
// 無法吸收（負的）額外空間時呼叫。這會將可縮減元素
// 賦值為零，並將剩餘的（負）extra_space 分配給
// 其他不可縮減的元素。
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
