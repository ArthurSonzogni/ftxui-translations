// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權約束，詳情請參閱
// LICENSE 文件。
#ifndef FTXUI_SCREEN_TERMINAL_HPP
#define FTXUI_SCREEN_TERMINAL_HPP

namespace ftxui {

/// @brief Dimensions 是一個表示終端機大小的結構
/// @ingroup screen
struct Dimensions {
  int dimx;
  int dimy;
};

namespace Terminal {
Dimensions Size();
void SetFallbackSize(const Dimensions& fallbackSize);

/// @brief Color 是一個列舉，表示終端機的色彩支援
/// @ingroup screen
enum Color {
  Palette1,
  Palette16,
  Palette256,
  TrueColor,
};
Color ColorSupport();
void SetColorSupport(Color color);

}  // namespace Terminal

}  // namespace ftxui

#endif  // FTXUI_SCREEN_TERMINAL_HPP