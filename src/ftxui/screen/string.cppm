/// @module ftxui.screen:String
/// @brief Fichier module pour les fonctions de chaîne du module Screen

module;

#include <ftxui/screen/string.hpp>

export module ftxui.screen:String;

/**
 * @namespace ftxui
 * @brief L'espace de noms FTXUI ftxui::
 */
export namespace ftxui {
    using ftxui::to_string;
    using ftxui::to_wstring;
    using ftxui::string_width;
    using ftxui::Utf8ToGlyphs;
    using ftxui::CellToGlyphIndex;
}
