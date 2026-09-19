// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_SURFACE_HPP
#define FTXUI_SCREEN_SURFACE_HPP

#include <string>  // for string, basic_string, allocator
#include <vector>  // for vector

#include "ftxui/screen/box.hpp"   // for Box
#include "ftxui/screen/cell.hpp"  // for Cell
#include "ftxui/util/export.hpp"  // for FTXUI_EXPORT

namespace ftxui {

/// @brief 一個由 Cell 組成的矩形網格。
///
/// @note 此類別先前名為 Image。
///
/// @ingroup screen
class FTXUI_EXPORT(SCREEN) Surface {
 public:
  // 建構子：
  Surface() = delete;
  Surface(int dimx, int dimy);

  // 解構函式：
  virtual ~Surface() = default;

  // 複製：
  Surface(const Surface&) = default;
  Surface& operator=(const Surface&) = default;

  // 存取網格中指定位置的一個字元。
  std::string& at(int x, int y);
  const std::string& at(int x, int y) const;

  // 存取網格中指定位置的一個儲存格（Cell）。
  Cell& CellAt(int x, int y);
  const Cell& CellAt(int x, int y) const;

  // [已棄用] CellAt 的別名。
  Cell& PixelAt(int x, int y) { return CellAt(x, y); }
  const Cell& PixelAt(int x, int y) const { return CellAt(x, y); }

  // 取得畫面尺寸。
  int dimx() const { return dimx_; }
  int dimy() const { return dimy_; }

  // 以空白字元和預設樣式填滿此表面
  void Clear();

  // ABI 保留欄位：
  virtual void Reserved1();
  virtual void Reserved2();
  virtual void Reserved3();
  virtual void Reserved4();
  virtual void Reserved5();
  virtual void Reserved6();
  virtual void Reserved7();
  virtual void Reserved8();

  Box stencil;

 protected:
  Cell& FastCellAt(int x, int y);
  const Cell& FastCellAt(int x, int y) const;

  int dimx_;
  int dimy_;
  std::vector<Cell> cells_;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_SURFACE_HPP
