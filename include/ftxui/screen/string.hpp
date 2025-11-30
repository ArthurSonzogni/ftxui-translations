// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef FTXUI_SCREEN_STRING_HPP
#define FTXUI_SCREEN_STRING_HPP

#include <string>  // for string, wstring, to_string
#include <vector>  // for vector

namespace ftxui {
std::string to_string(const std::wstring& s);
std::wstring to_wstring(const std::string& s);

template <typename T>
std::wstring to_wstring(T s) {
  return to_wstring(std::to_string(s));
}

int string_width(const std::string&);

// Divide la cadena en sus glifos. Se inserta uno vacío después de los de ancho completo.
std::vector<std::string> Utf8ToGlyphs(const std::string& input);

// Asigna cada celda dibujada por |input| a sus glifos correspondientes. Los glifos de
// tamaño medio ocupan una celda, los glifos de tamaño completo ocupan dos celdas.
std::vector<int> CellToGlyphIndex(const std::string& input);

}  // namespace ftxui

#endif /* end of include guard: FTXUI_SCREEN_STRING_HPP */
