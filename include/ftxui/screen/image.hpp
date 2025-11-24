// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_IMAGE_HPP
#define FTXUI_SCREEN_IMAGE_HPP

#include <string>  // for string, basic_string, allocator
#include <vector>  // for vector

#include "ftxui/screen/box.hpp"    // for Box
#include "ftxui/screen/pixel.hpp"  // for Pixel

namespace ftxui {

/// @brief 像素的矩形網格。
/// @ingroup screen
class Image {
 public:
  // 建構函式：
  Image() = delete;
  Image(int dimx, int dimy);

  // 解構函式：
  virtual ~Image() = default;

  // 存取網格中指定位置的字元。
  std::string& at(int x, int y);
  const std::string& at(int x, int y) const;

  // 存取網格中指定位置的儲存格（像素）。
  Pixel& PixelAt(int x, int y);
  const Pixel& PixelAt(int x, int y) const;

  // 取得螢幕尺寸。
  int dimx() const { return dimx_; }
  int dimy() const { return dimy_; }

  // 使用空格和預設樣式填充圖片
  void Clear();

  Box stencil;

 protected:
  int dimx_;
  int dimy_;
  std::vector<std::vector<Pixel>> pixels_;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_IMAGE_HPP
