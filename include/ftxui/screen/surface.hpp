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

/// @brief Cellの長方形グリッド。
///
/// @note このクラスは以前はImageという名前でした。
///
/// @ingroup screen
class FTXUI_EXPORT(SCREEN) Surface {
 public:
  // コンストラクタ:
  Surface() = delete;
  Surface(int dimx, int dimy);

  // デストラクタ:
  virtual ~Surface() = default;

  // コピー:
  Surface(const Surface&) = default;
  Surface& operator=(const Surface&) = default;

  // グリッド内の指定位置にある文字にアクセスします。
  std::string& at(int x, int y);
  const std::string& at(int x, int y) const;

  // グリッド内の指定位置にあるセル(Cell)にアクセスします。
  Cell& CellAt(int x, int y);
  const Cell& CellAt(int x, int y) const;

  // [非推奨] CellAtのエイリアス。
  Cell& PixelAt(int x, int y) { return CellAt(x, y); }
  const Cell& PixelAt(int x, int y) const { return CellAt(x, y); }

  // 画面の寸法を取得します。
  int dimx() const { return dimx_; }
  int dimy() const { return dimy_; }

  // サーフェスをスペースとデフォルトスタイルで埋めます。
  void Clear();

  // ABI予約:
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
