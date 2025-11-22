// Copyright 2020 Arthur Sonzogni. All rights reserved.
// 本源代码受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
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

/// @brief 访问给定位置单元格中的字符。
/// @param x 沿 x 轴的单元格位置。
/// @param y 沿 y 轴的单元格位置。
std::string& Image::at(int x, int y) {
  return PixelAt(x, y).character;
}

/// @brief 访问给定位置单元格中的字符。
/// @param x 沿 x 轴的单元格位置。
/// @param y 沿 y 轴的单元格位置。
const std::string& Image::at(int x, int y) const {
  return PixelAt(x, y).character;
}

/// @brief 访问给定位置的单元格 (Pixel)。
/// @param x 沿 x 轴的单元格位置。
/// @param y 沿 y 轴的单元格位置。
Pixel& Image::PixelAt(int x, int y) {
  return stencil.Contain(x, y) ? pixels_[y][x] : dev_null_pixel();
}

/// @brief 访问给定位置的单元格 (Pixel)。
/// @param x 沿 x 轴的单元格位置。
/// @param y 沿 y 轴的单元格位置。
const Pixel& Image::PixelAt(int x, int y) const {
  return stencil.Contain(x, y) ? pixels_[y][x] : dev_null_pixel();
}

/// @brief 清除屏幕上的所有像素。
void Image::Clear() {
  for (auto& line : pixels_) {
    for (auto& cell : line) {
      cell = Pixel();
    }
  }
}

}  // namespace ftxui
