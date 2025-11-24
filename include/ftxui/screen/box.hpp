// Copyright 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款的約束，該條款可在 LICENSE 檔案中找到。
#ifndef FTXUI_SCREEN_BOX_HPP
#define FTXUI_SCREEN_BOX_HPP

namespace ftxui {

/// @brief Box 是一個表示二維空間中矩形區域的結構。
///
/// 它由其沿 x 軸和 y 軸的最小和最大座標定義。
/// 請注意，座標是包含性的，這表示該框包含最小值和最大值。
///
/// @ingroup screen
struct Box {
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
