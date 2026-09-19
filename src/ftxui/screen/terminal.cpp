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

/// @brief 環境変数とターミナル識別に基づいて色サポートを計算します。
/// @param term TERM環境変数。
/// @param colorterm COLORTERM環境変数。
/// @param term_program TERM_PROGRAM環境変数。
/// @param terminal_name ターミナル名 (DA2から)。
/// @param terminal_emulator_name ターミナルエミュレータ名 (XTVERSIONから)。
/// @param capabilities ターミナルの能力 (DA1から)。
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
  // TODO(v8): ComputeColorSupportInternal() から NO_COLOR と WT_SESSION を読み込み、
  // パラメータとして渡すことで、この関数が入力に対する純粋な関数のままであるようにする。
  // これには、公開の Terminal::ComputeColorSupport() のシグネチャを拡張する必要があり、
  // つまり API を破壊する変更となる。

  // 0. ユーザー設定。 https://no-color.org を参照。
  if (util::GetEnv("NO_COLOR")[0] != '\0') {
    return Terminal::Color::Palette1;
  }

  // 1. プラットフォーム固有のオーバーライド。
#if defined(__EMSCRIPTEN__)
  return Terminal::Color::TrueColor;
#endif
#if defined(_WIN32)
  // コンソールで実行されているかどうか、およびそのコンソールが VT 処理をサポートしているかどうかを確認します。
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

  // (例えば WSL 上で実行している場合の) Windows Terminal を検出するために WT_SESSION を確認します。
  if (util::GetEnv("WT_SESSION")[0] != '\0') {
    return Terminal::Color::TrueColor;
  }

  // 2. term / colorterm 環境変数。
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
  // Apple の Terminal.app (TERM_PROGRAM=Apple_Terminal) は256色をサポートしますが、
  // 24ビットカラーはサポートしません。
  if (Contains(impl_->term_program, "apple_terminal")) {
    return Terminal::Color::Palette256;
  }

  // 4. ターミナルの識別。
  // 名前が空の場合、ターミナルが識別されなかったことを意味し、
  // "unknown" と同じ扱いになります。
  if (!impl_->terminal_emulator_name.empty() &&
      impl_->terminal_emulator_name != "unknown") {
    return Terminal::Color::TrueColor;
  }
  if (impl_->terminal_name == "xterm") {
    return Terminal::Color::TrueColor;
  }
  for (const int x : impl_->capabilities) {
    // 値 22 は256色に対応する SGR 機能です。ターミナルがこれを
    // サポートしている場合、そのターミナルが256色をサポートしている強い
    // 兆候となります。これは完全な検出方法ではありませんが、より具体的な
    // 情報がない場合の妥当なヒューリスティックです。
    if (x == 22) {
      return Terminal::Color::Palette256;
    }
  }

  return Terminal::Color::Palette16;
}

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
  // The ioctl return value result should be checked. Some operating systems
  // don't support TIOCGWINSZ.
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
  if (!ColorSupportDetected()) {
    GetQuirksInternal().SetColorSupport(ComputeColorSupportInternal());
    ColorSupportDetected() = true;
  }
  return GetQuirksInternal().ColorSupport();
}

/// @brief 自動検出が失敗した場合にターミナルの色サポートを上書きします
/// @ingroup dom
void SetColorSupport(Color color) {
  GetQuirksInternal().SetColorSupport(color);
  ColorSupportDetected() = true;
}

/// @brief ターミナルのクイークを取得します。
/// @ingroup screen
Quirks GetQuirks() {
  if (!ColorSupportDetected()) {
    GetQuirksInternal().SetColorSupport(ComputeColorSupportInternal());
    ColorSupportDetected() = true;
  }
  return GetQuirksInternal();
}

/// @brief ターミナルのクイークを上書きします。
/// @ingroup screen
void SetQuirks(const Quirks& quirks) {
  GetQuirksInternal() = quirks;
  ColorSupportDetected() = true;
}

}  // namespace Terminal
}  // namespace ftxui
