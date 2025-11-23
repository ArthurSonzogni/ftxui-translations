// Copyright 2021 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#ifndef FTXUI_DOM_DEPRECATED_HPP
#define FTXUI_DOM_DEPRECATED_HPP

#include <ftxui/dom/node.hpp>
#include <string>

namespace ftxui {
Element text(std::wstring text);
Element vtext(std::wstring text);
Elements paragraph(std::wstring text);
}  // namespace ftxui

#endif  // FTXUI_DOM_DEPRECATED_HPP
