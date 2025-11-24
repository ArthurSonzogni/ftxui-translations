// Copyright 2021 Arthur Sonzogni. 版權所有。
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱
// LICENSE 檔案。
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

// 當允許的尺寸大於請求的尺寸時呼叫。這會
// 將額外空間按相對比例分配給彈性元素。
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

// 當允許的尺寸小於請求的尺寸時呼叫，且
// 可縮小元素可以吸收（負數）額外空間。這會將
// 額外空間分配給這些元素。
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

// 當允許的尺寸小於請求的尺寸時呼叫，且
// 可縮小元素無法吸收（負數）額外空間。這會將
// 零分配給可縮小元素，並將剩餘的（負數）
// 額外空間分配給其他不可縮小元素。
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
