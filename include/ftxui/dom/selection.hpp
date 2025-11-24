// Copyright 2024 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱
// LICENSE 檔案。

#ifndef FTXUI_DOM_SELECTION_HPP
#define FTXUI_DOM_SELECTION_HPP

#include <functional>

#include <sstream>
#include "ftxui/screen/box.hpp"    // for Box
#include "ftxui/screen/pixel.hpp"  // for Pixel

namespace ftxui {

/// @brief 表示終端機使用者介面中的選取範圍。
///
/// Selection 是一個類別，表示終端機使用者介面中選取範圍的兩個端點。
///
/// @ingroup dom
class Selection {
 public:
  Selection();  // 空選取範圍。
  Selection(int start_x, int start_y, int end_x, int end_y);

  const Box& GetBox() const;

  Selection SaturateHorizontal(Box box);
  Selection SaturateVertical(Box box);
  bool IsEmpty() const { return empty_; }

  void AddPart(const std::string& part, int y, int left, int right);
  std::string GetParts() { return parts_.str(); }

 private:
  Selection(int start_x, int start_y, int end_x, int end_y, Selection* parent);

  const int start_x_ = 0;
  const int start_y_ = 0;
  const int end_x_ = 0;
  const int end_y_ = 0;
  const Box box_ = {};
  Selection* const parent_ = this;
  const bool empty_ = true;
  std::stringstream parts_;

  // 最後插入部分的座標。
  int x_ = 0;
  int y_ = 0;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_DOM_SELECTION_HPP */