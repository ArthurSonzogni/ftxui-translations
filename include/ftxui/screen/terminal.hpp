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

/// @brief Dimensions est une structure qui représente la taille du terminal
/// @ingroup screen
struct FTXUI_EXPORT(SCREEN) Dimensions {
  int dimx;
  int dimy;
};

namespace Terminal {
FTXUI_EXPORT(SCREEN) Dimensions Size();
FTXUI_EXPORT(SCREEN) void SetFallbackSize(const Dimensions& fallbackSize);

/// @brief Color est une énumération qui représente le support des couleurs
/// du terminal.
/// @ingroup screen
enum Color : uint8_t {
  Palette1,
  Palette16,
  Palette256,
  TrueColor,
};
FTXUI_EXPORT(SCREEN) Color ColorSupport();
FTXUI_EXPORT(SCREEN) void SetColorSupport(Color color);

/// @brief Quirks est une structure qui représente divers comportements spécifiques
/// au terminal pouvant nécessiter des solutions de repli.
/// @ingroup screen
class FTXUI_EXPORT(SCREEN) Quirks {
 public:
  Quirks();
  ~Quirks();
  Quirks(const Quirks&);
  Quirks& operator=(const Quirks&);
  Quirks(Quirks&&) noexcept;
  Quirks& operator=(Quirks&&) noexcept;

  /// @brief Indique si la police du terminal prend en charge les 8 caractères de bloc Unicode.
  bool BlockCharacters() const;
  void SetBlockCharacters(bool v);

  /// @brief Indique si le terminal gère correctement le masquage du curseur.
  bool CursorHiding() const;
  void SetCursorHiding(bool v);

  /// @brief Indique si le terminal doit utiliser des caractères ASCII pour les composants.
  bool ComponentAscii() const;
  void SetComponentAscii(bool v);

  /// @brief Le niveau de prise en charge des couleurs du terminal.
  Color ColorSupport() const;
  void SetColorSupport(Color v);

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
FTXUI_EXPORT(SCREEN) Quirks GetQuirks();
FTXUI_EXPORT(SCREEN) void SetQuirks(const Quirks& quirks);

/// @brief TerminalInfo est une structure qui contient des informations sur le
/// terminal.
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

/// @brief Calcule la prise en charge des couleurs en fonction des variables d'environnement et de
/// l'identification du terminal.
/// @param term La variable d'environnement TERM.
/// @param colorterm La variable d'environnement COLORTERM.
/// @param term_program La variable d'environnement TERM_PROGRAM.
/// @param terminal_name Le nom du terminal (depuis DA2).
/// @param terminal_emulator_name Le nom de l'émulateur de terminal (depuis XTVERSION).
/// @param capabilities Les capacités du terminal (depuis DA1).
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
