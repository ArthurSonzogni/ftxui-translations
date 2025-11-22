// Copyright 2020 Arthur Sonzogni. All rights reserved.
// 本源代码的使用受 MIT 许可的约束，该许可可在 LICENSE 文件中找到。
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
  // 选择此维度是为了能够显示:
  // https://arthursonzogni.com/FTXUI/examples
  // 当有人有时间实现并需要时，这将会得到改进。
  constexpr int fallback_width = 140;
  constexpr int fallback_height = 43;
#else
  // VT100 中的终端尺寸是 80x24。它至今仍被许多终端模拟器默认使用。
  // 这是一个很好的回退值选择。
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
  // 微软终端没有正确声明自己支持真彩色: https://github.com/microsoft/terminal/issues/1040
  // 作为回退，假设微软终端是没有设置这些变量的终端，并启用真彩色。
  if (TERM.empty() && COLORTERM.empty()) {
    return Terminal::Color::TrueColor;
  }
#endif

  return Terminal::Color::Palette16;
}

}  // namespace

namespace Terminal {

/// @brief 获取终端尺寸。
/// @return 终端尺寸。
/// @ingroup screen
Dimensions Size() {
#if defined(__EMSCRIPTEN__)
  // 选择此维度是为了能够显示:
  // https://arthursonzogni.com/FTXUI/examples
  // 当有人有时间实现并需要时，这将会得到改进。
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
  // 应该检查 ioctl 返回值。某些操作系统不支持 TIOCGWINSZ。
  if (w.ws_col == 0 || w.ws_row == 0 || status < 0) {
    return FallbackSize();
  }
  return Dimensions{w.ws_col, w.ws_row};
#endif
}

/// @brief 在自动检测失败时覆盖终端尺寸
/// @param fallbackSize 回退的终端尺寸
void SetFallbackSize(const Dimensions& fallbackSize) {
  FallbackSize() = fallbackSize;
}

/// @brief 获取终端的颜色支持。
/// @ingroup screen
Color ColorSupport() {
  if (!g_cached) {
    g_cached = true;
    g_cached_supported_color = ComputeColorSupport();
  }
  return g_cached_supported_color;
}

/// @brief 在自动检测失败时覆盖终端颜色支持
/// @ingroup dom
void SetColorSupport(Color color) {
  g_cached = true;
  g_cached_supported_color = color;
}

}  // namespace Terminal
}  // namespace ftxui
