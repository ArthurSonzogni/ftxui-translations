// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSE ファイルに見つかる MIT ライセンスによって管理されています。
#include "ftxui/screen/box.hpp"

#include <algorithm>

namespace ftxui {
/// @return |a| と |b| の両方に含まれる最大のBoxを返します。
// static
Box Box::Intersection(Box a, Box b) {
  return Box{
      std::max(a.x_min, b.x_min),
      std::min(a.x_max, b.x_max),
      std::max(a.y_min, b.y_min),
      std::min(a.y_max, b.y_max),
  };
}

/// @return |a| と |b| の両方を含む最小のBoxを返します。
// static
Box Box::Union(Box a, Box b) {
  return Box{
      std::min(a.x_min, b.x_min),
      std::max(a.x_max, b.x_max),
      std::min(a.y_min, b.y_min),
      std::max(a.y_max, b.y_max),
  };
}

/// Boxを (x,y) だけシフトします。
/// @param x 水平シフト。
/// @param y 垂直シフト。
void Box::Shift(int x, int y) {
  x_min += x;
  x_max += x;
  y_min += y;
  y_max += y;
}

/// @return (x,y) がBox内に含まれているかどうか。
bool Box::Contain(int x, int y) const {
  return x_min <= x &&  //
         x_max >= x &&  //
         y_min <= y &&  //
         y_max >= y;
}

/// @return Boxが空かどうか。
bool Box::IsEmpty() const {
  return x_min > x_max || y_min > y_max;
}

/// @return |other| が |this| と同じかどうか。
bool Box::operator==(const Box& other) const {
  return (x_min == other.x_min) && (x_max == other.x_max) &&
         (y_min == other.y_min) && (y_max == other.y_max);
}

/// @return |other| と |this| が異なるかどうか。
bool Box::operator!=(const Box& other) const {
  return !operator==(other);
}

}  // namespace ftxui
