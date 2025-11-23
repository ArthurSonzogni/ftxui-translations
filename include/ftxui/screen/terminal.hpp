// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être
// trouvée dans le fichier LICENSE.
#ifndef FTXUI_SCREEN_TERMINAL_HPP
#define FTXUI_SCREEN_TERMINAL_HPP

namespace ftxui {

/// @brief Dimensions est une structure qui représente la taille du terminal
/// @ingroup screen
struct Dimensions {
  int dimx;
  int dimy;
};

namespace Terminal {
Dimensions Size();
void SetFallbackSize(const Dimensions& fallbackSize);

/// @brief Color est une énumération qui représente le support des couleurs
/// du terminal.
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
