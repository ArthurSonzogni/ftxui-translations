// Copyright 2024 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスによって管理されています。
// the LICENSE file.
#ifndef FTXUI_SCREEN_IMAGE_HPP
#define FTXUI_SCREEN_IMAGE_HPP

#include <string>  // for string, basic_string, allocator
#include <vector>  // for vector

#include "ftxui/screen/box.hpp"    // for Box
#include "ftxui/screen/pixel.hpp"  // for Pixel

namespace ftxui {

/// @brief ピクセルの長方形グリッド。
/// @ingroup screen
class Image {
 public:
  // Constructors:
  Image() = delete;
  Image(int dimx, int dimy);

  // Destructor:
  virtual ~Image() = default;

  // グリッド内の指定された位置にある文字にアクセスします。
  std::string& at(int x, int int y);
  const std::string& at(int x, int y) const;

  // グリッド内の指定された位置にあるセル (Pixel) にアクセスします。
  Pixel& PixelAt(int x, int y);
  const Pixel& PixelAt(int x, int y) const;

  // 画面の寸法を取得します。
  int dimx() const { return dimx_; }
  int dimy() const { return dimy_; }

  // 画像をスペースとデフォルトスタイルで埋めます。
  void Clear();

  Box stencil;

 protected:
  int dimx_;
  int dimy_;
  std::vector<std::vector<Pixel>> pixels_;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_IMAGE_HPP