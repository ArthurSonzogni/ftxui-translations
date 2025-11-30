// Copyright 2022 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef FTXUI_SCREEN_UTIL_HPP
#define FTXUI_SCREEN_UTIL_HPP

namespace ftxui::util {

// Similar a std::clamp, pero permite que hi sea menor que lo.
template <class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
  return v < lo ? lo : hi < v ? hi : v;
}

}  // namespace ftxui::util

#endif /* fin de la guarda de inclusión: FTXUI_SCREEN_UTIL_HPP */
