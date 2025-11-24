/// @module ftxui.screen:String
/// @brief Screen 模組字串功能的模組檔案

module;

#include <ftxui/screen/string.hpp>

export module ftxui.screen:String;

/**
 * @namespace ftxui
 * @brief FTXUI 的 ftxui:: 命名空間
 */
export namespace ftxui {
    using ftxui::to_string;
    using ftxui::to_wstring;
    using ftxui::string_width;
    using ftxui::Utf8ToGlyphs;
    using ftxui::CellToGlyphIndex;
}
