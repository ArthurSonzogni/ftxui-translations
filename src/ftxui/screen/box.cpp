// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權約束，詳見
// LICENSE 文件。
#include "ftxui/screen/box.hpp"

#include <algorithm>

namespace ftxui {
/// @return 包含在 |a| 和 |b| 中的最大 Box。
// static
Box Box::Intersection(Box a, Box b) {
  return Box{
      std::max(a.x_min, b.x_min),
      std::min(a.x_max, b.x_max),
      std::max(a.y_min, b.y_min),
      std::min(a.y_max, b.y_max),
  };
}

/// @return 包含 |a| 和 |b| 的最小 Box。
// static
Box Box::Union(Box a, Box b) {
  return Box{
      std::min(a.x_min, b.x_min),
      std::max(a.x_max, b.x_max),
      std::min(a.y_min, b.y_min),
      std::max(a.y_max, b.y_max),
  };
}

/// 將 Box 依據 (x,y) 進行位移。
/// @param x 水平位移。
/// @param y 垂直位移。
void Box::Shift(int x, int y) {
  x_min += x;
  x_max += x;
  y_min += y;
  y_max += y;
}

/// @return (x,y) 是否包含在 Box 內。
bool Box::Contain(int x, int y) const {
  return x_min <= x &&  //
         x_max >= x &&  //
         y_min <= y &&  //
         y_max >= y;
}

/// @return Box 是否為空。
bool Box::IsEmpty() const {
  return x_min > x_max || y_min > y_max;
}

/// @return |other| 是否與 |this| 相同。
bool Box::operator==(const Box& other) const {
  return (x_min == other.x_min) && (x_max == other.x_max) &&
         (y_min == other.y_min) && (y_max == other.y_max);
}

/// @return |other| 和 |this| 是否不同。
bool Box::operator!=(const Box& other) const {
  return !operator==(other);
}

}  // namespace ftxui
