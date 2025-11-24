// Copyright 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱
// LICENSE 文件。
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

/// @brief 存取給定位置的單元格中的字元。
/// @param x 沿 X 軸的單元格位置。
/// @param y 沿 Y 軸的單元格位置。
std::string& Image::at(int x, int y) {
  return PixelAt(x, y).character;
}

/// @brief 存取給定位置的單元格中的字元。
/// @param x 沿 X 軸的單元格位置。
/// @param y 沿 Y 軸的單元格位置。
const std::string& Image::at(int x, int y) const {
  return PixelAt(x, y).character;
}

/// @brief 存取給定位置的單元格 (Pixel)。
/// @param x 沿 X 軸的單元格位置。
/// @param y 沿 Y 軸的單元格位置。
Pixel& Image::PixelAt(int x, int y) {
  return stencil.Contain(x, y) ? pixels_[y][x] : dev_null_pixel();
}

/// @brief 存取給定位置的單元格 (Pixel)。
/// @param x 沿 X 軸的單元格位置。
/// @param y 沿 Y 軸的單元格位置。
const Pixel& Image::PixelAt(int x, int y) const {
  return stencil.Contain(x, y) ? pixels_[y][x] : dev_null_pixel();
}

/// @brief 清除螢幕上的所有像素。
void Image::Clear() {
  for (auto& line : pixels_) {
    for (auto& cell : line) {
      cell = Pixel();
    }
  }
}

}  // namespace ftxui
