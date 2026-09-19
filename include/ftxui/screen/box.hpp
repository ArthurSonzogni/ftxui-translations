// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_BOX_HPP
#define FTXUI_SCREEN_BOX_HPP

#include "ftxui/util/export.hpp"

namespace ftxui {

/// @brief Boxは、2D空間における矩形領域を表す構造体です。
///
/// これは、x軸とy軸に沿った最小座標と最大座標によって定義されます。
/// 座標は包括的であり、ボックスには最小値と最大値の両方が含まれることに注意してください。
///
/// @ingroup screen
struct FTXUI_EXPORT(SCREEN) Box {
  int x_min = 0;
  int x_max = 0;
  int y_min = 0;
  int y_max = 0;

  static auto Intersection(Box a, Box b) -> Box;
  static auto Union(Box a, Box b) -> Box;
  void Shift(int x, int y);
  bool Contain(int x, int y) const;
  bool IsEmpty() const;
  bool operator==(const Box& other) const;
  bool operator!=(const Box& other) const;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_BOX_HPP
