// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on trouve dans
// le fichier LICENSE.
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

// Divise la chaîne en ses glyphes. Un glyphe vide est inséré après les glyphes pleine largeur.
std::vector<std::string> Utf8ToGlyphs(const std::string& input);

// Associe chaque cellule dessinée par |input| à son glyphe correspondant. Les glyphes de demi-taille
// prennent une cellule, les glyphes de pleine taille en prennent deux.
std::vector<int> CellToGlyphIndex(const std::string& input);

}  // namespace ftxui

#endif /* Fin de la garde d'inclusion: FTXUI_SCREEN_STRING_HPP */
