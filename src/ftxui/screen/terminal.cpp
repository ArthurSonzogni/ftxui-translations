// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <cstdlib>  // for getenv
#include <string>   // for string, allocator

#include "ftxui/screen/terminal.hpp"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#else
#include <sys/ioctl.h>  // for winsize, ioctl, TIOCGWINSZ
#include <unistd.h>     // for STDOUT_FILENO
#endif

namespace ftxui {

namespace {

bool g_cached = false;                     // NOLINT
Terminal::Color g_cached_supported_color;  // NOLINT

Dimensions& FallbackSize() {
#if defined(__EMSCRIPTEN__)
  // 選擇此尺寸是為了能夠顯示：
  // https://arthursonzogni.com/FTXUI/examples
  // 當有人有時間實作並需要時，這將會改進。
  constexpr int fallback_width = 140;
  constexpr int fallback_height = 43;
#else
  // VT100 中的終端機大小為 80x24。它至今仍被許多終端機模擬器預設使用。這是一個很好的後備值。
  constexpr int fallback_width = 80;
  constexpr int fallback_height = 24;
#endif
  static Dimensions g_fallback_size{
      fallback_width,
      fallback_height,
  };
  return g_fallback_size;
}

const char* Safe(const char* c) {
  return (c != nullptr) ? c : "";
}

bool Contains(const std::string& s, const char* key) {
  return s.find(key) != std::string::npos;
}

Terminal::Color ComputeColorSupport() {
#if defined(__EMSCRIPTEN__)
  return Terminal::Color::TrueColor;
#endif

  std::string COLORTERM = Safe(std::getenv("COLORTERM"));  // NOLINT
  if (Contains(COLORTERM, "24bit") || Contains(COLORTERM, "truecolor")) {
    return Terminal::Color::TrueColor;
  }

  std::string TERM = Safe(std::getenv("TERM"));  // NOLINT
  if (Contains(COLORTERM, "256") || Contains(TERM, "256")) {
    return Terminal::Color::Palette256;
  }

#if defined(FTXUI_MICROSOFT_TERMINAL_FALLBACK)
  // 微軟終端機沒有正確宣告自己支援真彩色：https://github.com/microsoft/terminal/issues/1040
  // 作為後備方案，假設微軟終端機是那些沒有設定這些變量的終端機，並啟用真彩色。
  if (TERM.empty() && COLORTERM.empty()) {
    return Terminal::Color::TrueColor;
  }
#endif

  return Terminal::Color::Palette16;
}

}  // namespace

namespace Terminal {

/// @brief 獲取終端機大小。
/// @return 終端機大小。
/// @ingroup screen
Dimensions Size() {
#if defined(__EMSCRIPTEN__)
  // This dimension was chosen arbitrarily to be able to display:
  // https://arthursonzogni.com/FTXUI/examples
  // This will have to be improved when someone has time to implement and need
  // it.
  return FallbackSize();
#elif defined(_WIN32)
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
    return Dimensions{csbi.srWindow.Right - csbi.srWindow.Left + 1,
                      csbi.srWindow.Bottom - csbi.srWindow.Top + 1};
  }

  return FallbackSize();
#else
  winsize w{};
  const int status = ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);  // NOLINT
  // The ioctl return value result should be checked. Some operating systems
  // don't support TIOCGWINSZ.
  if (w.ws_col == 0 || w.ws_row == 0 || status < 0) {
    return FallbackSize();
  }
  return Dimensions{w.ws_col, w.ws_row};
#endif
}

/// @brief 在自動偵測失敗時覆寫終端機大小
/// @param fallbackSize 要回退到的終端機尺寸
void SetFallbackSize(const Dimensions& fallbackSize) {
  FallbackSize() = fallbackSize;
}

/// @brief 獲取終端機的顏色支援。
/// @ingroup screen
Color ColorSupport() {
  if (!g_cached) {
    g_cached = true;
    g_cached_supported_color = ComputeColorSupport();
  }
  return g_cached_supported_color;
}

/// @brief 在自動偵測失敗時覆寫終端機顏色支援
/// @ingroup dom
void SetColorSupport(Color color) {
  g_cached = true;
  g_cached_supported_color = color;
}

}  // namespace Terminal
}  // namespace ftxui
