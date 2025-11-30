/// @module ftxui.screen:String
/// @brief Archivo de módulo para funciones de cadena del módulo Screen

module;

#include <ftxui/screen/string.hpp>

export module ftxui.screen:String;

/**
 * @namespace ftxui
 * @brief El espacio de nombres ftxui:: de FTXUI
 */
export namespace ftxui {
    using ftxui::to_string;
    using ftxui::to_wstring;
    using ftxui::string_width;
    using ftxui::Utf8ToGlyphs;
    using ftxui::CellToGlyphIndex;
}
