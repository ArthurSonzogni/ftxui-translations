/// @module ftxui.screen:String
/// @brief Screen 模块字符串函数的模块文件

module;

#include <ftxui/screen/string.hpp>

export module ftxui.screen:String;

/**
 * @namespace ftxui
 * @brief FTXUI ftxui:: 命名空间
 */
export namespace ftxui {
    using ftxui::to_string;
    using ftxui::to_wstring;
    using ftxui::string_width;
    using ftxui::Utf8ToGlyphs;
    using ftxui::CellToGlyphIndex;
}
