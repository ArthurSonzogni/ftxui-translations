// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_STRING_HPP
#define FTXUI_SCREEN_STRING_HPP

#include <string>       // for string, wstring, to_string
#include <string_view>  // for string_view
#include <vector>       // for vector

#include "ftxui/util/export.hpp"

namespace ftxui {
FTXUI_EXPORT(SCREEN) std::string to_string(std::wstring_view s);
FTXUI_EXPORT(SCREEN) std::wstring to_wstring(std::string_view s);

template <typename T>
std::wstring to_wstring(T s) {
  return to_wstring(std::string_view(std::to_string(s)));
}
inline std::wstring to_wstring(const std::string& s) {
  return to_wstring(std::string_view(s));
}
template <>
inline std::wstring to_wstring(const char* s) {
  return to_wstring(std::string_view(s));
}

FTXUI_EXPORT(SCREEN) int string_width(std::string_view);

// Divide la cadena en sus glifos. Se inserta uno vacío después de los de ancho completo.
FTXUI_EXPORT(SCREEN)
std::vector<std::string> Utf8ToGlyphs(std::string_view input);

// Asigna cada celda dibujada por |input| a sus glifos correspondientes. Los glifos de
// tamaño medio ocupan una celda, los glifos de tamaño completo ocupan dos celdas.
FTXUI_EXPORT(SCREEN)
std::vector<int> CellToGlyphIndex(std::string_view input);

}  // namespace ftxui

#endif /* end of include guard: FTXUI_SCREEN_STRING_HPP */
