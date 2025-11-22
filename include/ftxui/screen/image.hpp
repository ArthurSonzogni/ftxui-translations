// Copyright 2024 Arthur Sonzogni. 保留所有权利。
// 本源代码的使用受 MIT 许可协议的约束，该协议可在 LICENSE 文件中找到。
#ifndef FTXUI_SCREEN_IMAGE_HPP
#define FTXUI_SCREEN_IMAGE_HPP

#include <string>  // for string, basic_string, allocator
#include <vector>  // for vector

#include "ftxui/screen/box.hpp"    // for Box
#include "ftxui/screen/pixel.hpp"  // for Pixel

namespace ftxui {

/// @brief 像素的矩形网格。
/// @ingroup screen
class Image {
 public:
  // 构造函数：
  Image() = delete;
  Image(int dimx, int dimy);

  // 析构函数：
  virtual ~Image() = default;

  // 访问网格中给定位置的字符。
  std::string& at(int x, int y);
  const std::string& at(int x, int y) const;

  // 访问网格中给定位置的单元格 (Pixel)。
  Pixel& PixelAt(int x, int y);
  const Pixel& PixelAt(int x, int y) const;

  // 获取屏幕尺寸。
  int dimx() const { return dimx_; }
  int dimy() const { return dimy_; }

  // 用空格和默认样式填充图像
  void Clear();

  Box stencil;

 protected:
  int dimx_;
  int dimy_;
  std::vector<std::vector<Pixel>> pixels_;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_IMAGE_HPP
