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

/// @brief Dimensions 是一個表示終端機大小的結構
/// @ingroup screen
struct FTXUI_EXPORT(SCREEN) Dimensions {
  int dimx;
  int dimy;
};

namespace Terminal {
FTXUI_EXPORT(SCREEN) Dimensions Size();
FTXUI_EXPORT(SCREEN) void SetFallbackSize(const Dimensions& fallbackSize);

/// @brief Color 是一個列舉，表示終端機的色彩支援
/// @ingroup screen
enum Color : uint8_t {
  Palette1,
  Palette16,
  Palette256,
  TrueColor,
};
FTXUI_EXPORT(SCREEN) Color ColorSupport();
FTXUI_EXPORT(SCREEN) void SetColorSupport(Color color);

/// @brief Quirks 是一個結構，代表各種可能需要備援方案的
/// 終端機特定行為。
/// @ingroup screen
class FTXUI_EXPORT(SCREEN) Quirks {
 public:
  Quirks();
  ~Quirks();
  Quirks(const Quirks&);
  Quirks& operator=(const Quirks&);
  Quirks(Quirks&&) noexcept;
  Quirks& operator=(Quirks&&) noexcept;

  /// @brief 終端機字型是否支援 8 個 Unicode 區塊字元。
  bool BlockCharacters() const;
  void SetBlockCharacters(bool v);

  /// @brief 終端機是否能正確處理隱藏游標。
  bool CursorHiding() const;
  void SetCursorHiding(bool v);

  /// @brief 元件是否應該使用 ASCII 字元。
  bool ComponentAscii() const;
  void SetComponentAscii(bool v);

  /// @brief 終端機的顏色支援等級。
  Color ColorSupport() const;
  void SetColorSupport(Color v);

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
FTXUI_EXPORT(SCREEN) Quirks GetQuirks();
FTXUI_EXPORT(SCREEN) void SetQuirks(const Quirks& quirks);

/// @brief TerminalInfo 是一個包含終端機資訊的
/// 結構。
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

/// @brief 根據環境變數與終端機辨識資訊，計算
/// 顏色支援等級。
/// @param term TERM 環境變數。
/// @param colorterm COLORTERM 環境變數。
/// @param term_program TERM_PROGRAM 環境變數。
/// @param terminal_name 終端機名稱（來自 DA2）。
/// @param terminal_emulator_name 終端機模擬器名稱（來自 XTVERSION）。
/// @param capabilities 終端機能力（來自 DA1）。
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
