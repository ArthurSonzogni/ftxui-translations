// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスに従います。
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
  // このサイズは、https://arthursonzogni.com/FTXUI/examples を表示できるように任意に選択されました。
  // これは、誰かが実装して必要になったときに改善される必要があります。
  constexpr int fallback_width = 140;
  constexpr int fallback_height = 43;
#else
  // VT100のターミナルサイズは80x24でした。これは今日の多くのターミナルエミュレータでデフォルトとして使用されています。これはフォールバック値として良い選択です。
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
  // Microsoftのターミナルは、TrueColorのサポートを適切に宣言していません: https://github.com/microsoft/terminal/issues/1040
  // フォールバックとして、これらの変数を設定していないターミナルをMicrosoft製であると仮定し、TrueColorを有効にします。
  if (TERM.empty() && COLORTERM.empty()) {
    return Terminal::Color::TrueColor;
  }
#endif

  return Terminal::Color::Palette16;
}

}  // namespace

namespace Terminal {

/// @brief ターミナルサイズを取得します。
/// @return ターミナルサイズ。
/// @ingroup screen
Dimensions Size() {
#if defined(__EMSCRIPTEN__)
  // このサイズは、https://arthursonzogni.com/FTXUI/examples を表示できるように任意に選択されました。
  // これは、誰かが実装して必要になったときに改善される必要があります。
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
  // ioctlの戻り値は確認する必要があります。一部のオペレーティングシステムはTIOCGWINSZをサポートしていません。
  if (w.ws_col == 0 || w.ws_row == 0 || status < 0) {
    return FallbackSize();
  }
  return Dimensions{w.ws_col, w.ws_row};
#endif
}

/// @brief 自動検出が失敗した場合にターミナルサイズを上書きします
/// @param fallbackSize フォールバックするターミナルサイズ
void SetFallbackSize(const Dimensions& fallbackSize) {
  FallbackSize() = fallbackSize;
}

/// @brief ターミナルの色サポートを取得します。
/// @ingroup screen
Color ColorSupport() {
  if (!g_cached) {
    g_cached = true;
    g_cached_supported_color = ComputeColorSupport();
  }
  return g_cached_supported_color;
}

/// @brief 自動検出が失敗した場合にターミナルの色サポートを上書きします
/// @ingroup dom
void SetColorSupport(Color color) {
  g_cached = true;
  g_cached_supported_color = color;
}

}  // namespace Terminal
}  // namespace ftxui
