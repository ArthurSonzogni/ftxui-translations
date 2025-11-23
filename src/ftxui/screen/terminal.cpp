// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui se trouve
// dans le fichier LICENSE.
#include <cstdlib>  // pour getenv
#include <string>   // pour string, allocator

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
  // Cette dimension a été choisie arbitrairement pour pouvoir afficher :
  // https://arthursonzogni.com/FTXUI/examples
  // Cela devra être amélioré lorsque quelqu'un aura le temps de l'implémenter et en aura besoin.
  constexpr int fallback_height = 43;
#else
  // La taille du terminal en VT100 était de 80x24. Elle est encore utilisée de nos jours par
  // défaut dans de nombreux émulateurs de terminal. C'est un bon choix pour une valeur de
  // repli.
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
  // Les terminaux Microsoft ne se déclarent pas correctement comme supportant les vraies
  // couleurs : https://github.com/microsoft/terminal/issues/1040
  // En guise de solution de repli, on suppose que les terminaux Microsoft sont ceux qui ne
  // définissent pas ces variables, et on active les vraies couleurs.
  if (TERM.empty() && COLORTERM.empty()) {
    return Terminal::Color::TrueColor;
  }
#endif

  return Terminal::Color::Palette16;
}

}  // namespace

namespace Terminal {

/// @brief Obtenir la taille du terminal.
/// @return La taille du terminal.
/// @ingroup screen
Dimensions Size() {
#if defined(__EMSCRIPTEN__)
  // Cette dimension a été choisie arbitrairement pour pouvoir afficher :
  // https://arthursonzogni.com/FTXUI/examples
  // Cela devra être amélioré lorsque quelqu'un aura le temps de l'implémenter et en aura besoin.
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
  // Le résultat de la valeur de retour ioctl doit être vérifié. Certains systèmes d'exploitation
  // ne supportent pas TIOCGWINSZ.
  if (w.ws_col == 0 || w.ws_row == 0 || status < 0) {
    return FallbackSize();
  }
  return Dimensions{w.ws_col, w.ws_row};
#endif
}

/// @brief Outrepasser la taille du terminal en cas d'échec de la détection automatique.
/// @param fallbackSize Dimensions du terminal à utiliser en cas de repli.
void SetFallbackSize(const Dimensions& fallbackSize) {
  FallbackSize() = fallbackSize;
}

/// @brief Obtenir le support couleur du terminal.
/// @ingroup screen
Color ColorSupport() {
  if (!g_cached) {
    g_cached = true;
    g_cached_supported_color = ComputeColorSupport();
  }
  return g_cached_supported_color;
}

/// @brief Outrepasser le support couleur du terminal en cas d'échec de la détection automatique.
/// @ingroup dom
void SetColorSupport(Color color) {
  g_cached = true;
  g_cached_supported_color = color;
}

}  // namespace Terminal
}  // namespace ftxui
