// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_TERMINAL_HPP
#define FTXUI_SCREEN_TERMINAL_HPP

#include <cstdint>

#include <memory>
#include <string_view>
#include <vector>

#include "ftxui/util/export.hpp"

namespace ftxui {

/// @brief Dimensionsは、ターミナルのサイズを表す構造体です。
/// @ingroup screen
struct FTXUI_EXPORT(SCREEN) Dimensions {
  int dimx;
  int dimy;
};

namespace Terminal {
FTXUI_EXPORT(SCREEN) Dimensions Size();
FTXUI_EXPORT(SCREEN) void SetFallbackSize(const Dimensions& fallbackSize);

/// @brief Colorは、ターミナルの色サポートを表す列挙型です。
/// @ingroup screen
enum Color : uint8_t {
  Palette1,
  Palette16,
  Palette256,
  TrueColor,
};
FTXUI_EXPORT(SCREEN) Color ColorSupport();
FTXUI_EXPORT(SCREEN) void SetColorSupport(Color color);

/// @brief Quirksは、フォールバックが必要となる可能性のある様々な
/// ターミナル固有の動作を表す構造体です。
/// @ingroup screen
class FTXUI_EXPORT(SCREEN) Quirks {
 public:
  Quirks();
  ~Quirks();
  Quirks(const Quirks&);
  Quirks& operator=(const Quirks&);
  Quirks(Quirks&&) noexcept;
  Quirks& operator=(Quirks&&) noexcept;

  /// @brief ターミナルフォントが8つのUnicodeブロック文字をサポートしているかどうか。
  bool BlockCharacters() const;
  void SetBlockCharacters(bool v);

  /// @brief ターミナルがカーソルの非表示を正しく処理するかどうか。
  bool CursorHiding() const;
  void SetCursorHiding(bool v);

  /// @brief ターミナルがコンポーネントにASCII文字を使用すべきかどうか。
  bool ComponentAscii() const;
  void SetComponentAscii(bool v);

  /// @brief ターミナルの色サポートレベル。
  Color ColorSupport() const;
  void SetColorSupport(Color v);

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
FTXUI_EXPORT(SCREEN) Quirks GetQuirks();
FTXUI_EXPORT(SCREEN) void SetQuirks(const Quirks& quirks);

/// @brief TerminalInfoは、ターミナルに関する情報を含む構造体です。
/// @ingroup screen
class FTXUI_EXPORT(SCREEN) TerminalInfo {
 public:
  TerminalInfo();
  ~TerminalInfo();
  TerminalInfo(const TerminalInfo&) = delete;
  TerminalInfo& operator=(const TerminalInfo&) = delete;
  TerminalInfo(TerminalInfo&&) noexcept;
  TerminalInfo& operator=(TerminalInfo&&) noexcept;

  void SetTerm(std::string_view term);
  void SetColorterm(std::string_view colorterm);
  void SetTermProgram(std::string_view term_program);
  void SetTerminalName(std::string_view terminal_name);
  void SetTerminalEmulatorName(std::string_view terminal_emulator_name);
  void SetCapabilities(std::vector<int> capabilities);

  Color ComputeColorSupport() const;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

/// @brief 環境変数とターミナル識別に基づいて色サポートを計算します。
/// @param term TERM環境変数。
/// @param colorterm COLORTERM環境変数。
/// @param term_program TERM_PROGRAM環境変数。
/// @param terminal_name ターミナル名 (DA2から)。
/// @param terminal_emulator_name ターミナルエミュレータ名 (XTVERSIONから)。
/// @param capabilities ターミナルの能力 (DA1から)。
FTXUI_EXPORT(SCREEN)
Color ComputeColorSupport(std::string_view term,
                          std::string_view colorterm,
                          std::string_view term_program,
                          std::string_view terminal_name,
                          std::string_view terminal_emulator_name,
                          const std::vector<int>& capabilities);

}  // namespace Terminal

}  // namespace ftxui

#endif  // FTXUI_SCREEN_TERMINAL_HPP
