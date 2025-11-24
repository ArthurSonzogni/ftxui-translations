// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに従います。
#include <sstream>  // IWYU pragma: keep
#include <string>
#include <vector>

#include "ftxui/screen/image.hpp"
#include "ftxui/screen/pixel.hpp"

namespace ftxui {

namespace {
Pixel& dev_null_pixel() {
  static Pixel pixel;
  return pixel;
}
}  // namespace

Image::Image(int dimx, int dimy)
    : stencil{0, dimx - 1, 0, dimy - 1},
      dimx_(dimx),
      dimy_(dimy),
      pixels_(dimy, std::vector<Pixel>(dimx)) {}

/// @brief 指定された位置のセル内の文字にアクセスします。
/// @param x x軸に沿ったセルの位置。
/// @param y y軸に沿ったセルの位置。
  return PixelAt(x, y).character;
}

/// @brief 指定された位置のセル内の文字にアクセスします。
/// @param x x軸に沿ったセルの位置。
/// @param y y軸に沿ったセルの位置。
const std::string& Image::at(int x, int y) const {
  return PixelAt(x, y).character;
}

/// @brief 指定された位置のセル (ピクセル) にアクセスします。
/// @param x x軸に沿ったセルの位置。
/// @param y y軸に沿ったセルの位置。
Pixel& Image::PixelAt(int x, int y) {
  return stencil.Contain(x, y) ? pixels_[y][x] : dev_null_pixel();
}

/// @brief 指定された位置のセル (ピクセル) にアクセスします。
/// @param x x軸に沿ったセルの位置。
/// @param y y軸に沿ったセルの位置。
const Pixel& Image::PixelAt(int x, int y) const {
  return stencil.Contain(x, y) ? pixels_[y][x] : dev_null_pixel();
}

/// @brief 画面からすべてのピクセルをクリアします。
void Image::Clear() {
  for (auto& line : pixels_) {
    for (auto& cell : line) {
      cell = Pixel();
    }
  }
}

}  // namespace ftxui
