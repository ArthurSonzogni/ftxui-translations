/// @module ftxui.screen:String
/// @brief スクリーンモジュールの文字列機能のモジュールファイル

module;

#include <ftxui/screen/string.hpp>

export module ftxui.screen:String;

/**
 * @namespace ftxui
 * @brief FTXUI ftxui:: 名前空間
 */
export namespace ftxui {
    using ftxui::to_string;
    using ftxui::to_wstring;
    using ftxui::string_width;
    using ftxui::Utf8ToGlyphs;
    using ftxui::CellToGlyphIndex;
}
