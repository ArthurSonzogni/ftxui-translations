// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>  // for std::search
#include <cctype>     // for std::tolower
#include <initializer_list>
#include <string>
#include <string_view>  // for string_view

#include "ftxui/screen/terminal.hpp"
#include "ftxui/screen/util.hpp"  // for util::GetEnv

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
#if defined(__sun) || defined(__illumos__)
#include <sys/termios.h>  // for winsize on illumos
#endif

namespace ftxui {

namespace {

std::unique_ptr<Terminal::Quirks> g_quirks;
bool g_color_support_detected = false;

bool& ColorSupportDetected() {
  return g_color_support_detected;
}

Terminal::Quirks& GetQuirksInternal() {
  if (!g_quirks) {
    g_quirks = std::make_unique<Terminal::Quirks>();
#if defined(_WIN32)
    g_quirks->SetBlockCharacters(false);
    g_quirks->SetCursorHiding(false);
    g_quirks->SetComponentAscii(true);
#endif
  }
  return *g_quirks;
}

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

bool Contains(std::string_view s, std::string_view key) {
  if (key.empty()) {
    return true;
  }
  const auto it = std::search(  // NOLINT
      s.begin(), s.end(), key.begin(), key.end(), [](char a, char b) {
        return std::tolower(static_cast<unsigned char>(a)) ==
               std::tolower(static_cast<unsigned char>(b));
      });
  return it != s.end();
}

bool ContainsAny(std::string_view s,
                 std::initializer_list<std::string_view> keys) {
  for (const std::string_view key : keys) {
    if (Contains(s, key)) {
      return true;
    }
  }
  return false;
}

Terminal::Color ComputeColorSupportInternal() {
  static const std::vector<int> empty_capabilities;
  return Terminal::ComputeColorSupport(
      util::GetEnv("TERM"), util::GetEnv("COLORTERM"),
      util::GetEnv("TERM_PROGRAM"), "unknown", "unknown", empty_capabilities);
}

}  // namespace

namespace Terminal {

struct Quirks::Impl {
  bool block_characters = true;
  bool cursor_hiding = true;
  bool component_ascii = false;
  Color color_support = Palette256;
};

Quirks::Quirks() : impl_(std::make_unique<Impl>()) {}
Quirks::~Quirks() = default;
Quirks::Quirks(const Quirks& other)
    : impl_(std::make_unique<Impl>(*other.impl_)) {}
Quirks& Quirks::operator=(const Quirks& other) {
  if (this != &other) {
    *impl_ = *other.impl_;
  }
  return *this;
}
Quirks::Quirks(Quirks&&) noexcept = default;
Quirks& Quirks::operator=(Quirks&&) noexcept = default;

bool Quirks::BlockCharacters() const {
  return impl_->block_characters;
}
void Quirks::SetBlockCharacters(bool v) {
  impl_->block_characters = v;
}

bool Quirks::CursorHiding() const {
  return impl_->cursor_hiding;
}
void Quirks::SetCursorHiding(bool v) {
  impl_->cursor_hiding = v;
}

bool Quirks::ComponentAscii() const {
  return impl_->component_ascii;
}
void Quirks::SetComponentAscii(bool v) {
  impl_->component_ascii = v;
}

Color Quirks::ColorSupport() const {
  return impl_->color_support;
}
void Quirks::SetColorSupport(Color v) {
  impl_->color_support = v;
}

struct TerminalInfo::Impl {
  std::string term;
  std::string colorterm;
  std::string term_program;
  std::string terminal_name;
  std::string terminal_emulator_name;
  std::vector<int> capabilities;
};

TerminalInfo::TerminalInfo() : impl_(std::make_unique<Impl>()) {}
TerminalInfo::~TerminalInfo() = default;
TerminalInfo::TerminalInfo(TerminalInfo&&) noexcept = default;
TerminalInfo& TerminalInfo::operator=(TerminalInfo&&) noexcept = default;

void TerminalInfo::SetTerm(std::string_view term) {
  impl_->term = term;
}
void TerminalInfo::SetColorterm(std::string_view colorterm) {
  impl_->colorterm = colorterm;
}
void TerminalInfo::SetTermProgram(std::string_view term_program) {
  impl_->term_program = term_program;
}
void TerminalInfo::SetTerminalName(std::string_view terminal_name) {
  impl_->terminal_name = terminal_name;
}
void TerminalInfo::SetTerminalEmulatorName(
    std::string_view terminal_emulator_name) {
  impl_->terminal_emulator_name = terminal_emulator_name;
}
void TerminalInfo::SetCapabilities(std::vector<int> capabilities) {
  impl_->capabilities = std::move(capabilities);
}

/// @brief 根據環境變數與終端機辨識資訊，計算
/// 顏色支援等級。
/// @param term TERM 環境變數。
/// @param colorterm COLORTERM 環境變數。
/// @param term_program TERM_PROGRAM 環境變數。
/// @param terminal_name 終端機名稱（來自 DA2）。
/// @param terminal_emulator_name 終端機模擬器名稱（來自 XTVERSION）。
/// @param capabilities 終端機能力（來自 DA1）。
Color ComputeColorSupport(std::string_view term,
                          std::string_view colorterm,
                          std::string_view term_program,
                          std::string_view terminal_name,
                          std::string_view terminal_emulator_name,
                          const std::vector<int>& capabilities) {
  TerminalInfo info;
  info.SetTerm(term);
  info.SetColorterm(colorterm);
  info.SetTermProgram(term_program);
  info.SetTerminalName(terminal_name);
  info.SetTerminalEmulatorName(terminal_emulator_name);
  info.SetCapabilities(capabilities);
  return info.ComputeColorSupport();
}

Color TerminalInfo::ComputeColorSupport() const {
  // TODO(v8): 從 ComputeColorSupportInternal() 讀取 NO_COLOR 和
  // WT_SESSION，並將它們作為參數傳入，使此函式
  // 維持為輸入的純函式。這需要擴充公開的
  // Terminal::ComputeColorSupport() 簽章，也就是一項會破壞 API 的變更。

  // 0. 使用者偏好設定。參見 https://no-color.org。
  if (util::GetEnv("NO_COLOR")[0] != '\0') {
    return Terminal::Color::Palette1;
  }

  // 1. 平台特定的覆寫。
#if defined(__EMSCRIPTEN__)
  return Terminal::Color::TrueColor;
#endif
#if defined(_WIN32)
  // 檢查我們是否在主控台中執行，以及該主控台是否支援 VT 處理。
  auto stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD out_mode = 0;
  if (GetConsoleMode(stdout_handle, &out_mode)) {
    const int enable_virtual_terminal_processing = 0x0004;
    const int disable_newline_auto_return = 0x0008;
    out_mode |= enable_virtual_terminal_processing;
    out_mode |= disable_newline_auto_return;
    if (!SetConsoleMode(stdout_handle, out_mode)) {
      return Terminal::Color::Palette16;
    }
  }
  return Terminal::Color::TrueColor;
#endif

  // 檢查 WT_SESSION 以判斷是否為 Windows Terminal（例如在 WSL 下執行時）。
  if (util::GetEnv("WT_SESSION")[0] != '\0') {
    return Terminal::Color::TrueColor;
  }

  // 2. term / colorterm 環境變數。
  if (ContainsAny(impl_->colorterm, {"24bit", "truecolor"})) {
    return Terminal::Color::TrueColor;
  }
  if (ContainsAny(impl_->term,
                  {"direct", "truecolor", "kitty", "alacritty", "foot"})) {
    return Terminal::Color::TrueColor;
  }
  if (ContainsAny(impl_->colorterm, {"256"}) ||
      ContainsAny(impl_->term, {"256", "xterm", "screen", "tmux"})) {
    return Terminal::Color::Palette256;
  }

  // 3. term_program
  if (ContainsAny(impl_->term_program, {
                                           "iterm",
                                           "vscode",
                                           "warp",
                                           "ghostty",
                                           "wezterm",
                                       })) {
    return Terminal::Color::TrueColor;
  }
  // Apple 的 Terminal.app (TERM_PROGRAM=Apple_Terminal) 支援 256 色，
  // 但不支援 24 位元色。
  if (Contains(impl_->term_program, "apple_terminal")) {
    return Terminal::Color::Palette256;
  }

  // 4. 終端機識別。
  // 空名稱代表終端機未被識別，等同於
  // “unknown”。
  if (!impl_->terminal_emulator_name.empty() &&
      impl_->terminal_emulator_name != "unknown") {
    return Terminal::Color::TrueColor;
  }
  if (impl_->terminal_name == "xterm") {
    return Terminal::Color::TrueColor;
  }
  for (const int x : impl_->capabilities) {
    // 值 22 是支援 256 色的 SGR 能力。如果終端機
    // 支援它，這強烈表示該終端機支援 256
    // 色。這並非完美的偵測方法，但在缺乏更具體
    // 資訊的情況下，是合理的啟發式方法。
    if (x == 22) {
      return Terminal::Color::Palette256;
    }
  }

  return Terminal::Color::Palette16;
}

/// @brief 獲取終端機大小。
/// @return 終端機大小。
/// @ingroup screen
Dimensions Size() {
#if defined(__EMSCRIPTEN__)
  // 選擇此尺寸是為了能夠顯示：
  // https://arthursonzogni.com/FTXUI/examples
  // 當有人有時間實作並需要時，這將會改進。
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
  if (!ColorSupportDetected()) {
    GetQuirksInternal().SetColorSupport(ComputeColorSupportInternal());
    ColorSupportDetected() = true;
  }
  return GetQuirksInternal().ColorSupport();
}

/// @brief 在自動偵測失敗時覆寫終端機顏色支援
/// @ingroup dom
void SetColorSupport(Color color) {
  GetQuirksInternal().SetColorSupport(color);
  ColorSupportDetected() = true;
}

/// @brief 取得終端機的怪癖行為（quirks）。
/// @ingroup screen
Quirks GetQuirks() {
  if (!ColorSupportDetected()) {
    GetQuirksInternal().SetColorSupport(ComputeColorSupportInternal());
    ColorSupportDetected() = true;
  }
  return GetQuirksInternal();
}

/// @brief 覆寫終端機的怪癖行為（quirks）。
/// @ingroup screen
void SetQuirks(const Quirks& quirks) {
  GetQuirksInternal() = quirks;
  ColorSupportDetected() = true;
}

}  // namespace Terminal
}  // namespace ftxui
