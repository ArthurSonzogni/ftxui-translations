// Copyright 2022 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#ifndef FTXUI_SCREEN_UTIL_HPP
#define FTXUI_SCREEN_UTIL_HPP

namespace ftxui::util {

// Similaire à std::clamp, mais permet à 'hi' d'être inférieur à 'lo'.
template <class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
  return v < lo ? lo : hi < v ? hi : v;
}

}  // namespace ftxui::util

#endif /* fin de la protection d'inclusion : FTXUI_SCREEN_UTIL_HPP */
