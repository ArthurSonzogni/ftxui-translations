/// @module ftxui.screen:Terminal
/// @brief Screen模块中Terminal命名空间的模块文件

module;

#include <ftxui/screen/terminal.hpp>

export module ftxui.screen:Terminal;

/**
 * @namespace ftxui
 * @brief FTXUI ftxui:: 命名空间
 */
export namespace ftxui {
    using ftxui::Dimensions;

    /**
     * @namespace Terminal
     * @brief FTXUI ftxui::Terminal:: 命名空间
     */
    namespace Terminal {
        using ftxui::Terminal::Size;
        using ftxui::Terminal::SetFallbackSize;
        using ftxui::Terminal::Color;
        using ftxui::Terminal::ColorSupport;
        using ftxui::Terminal::SetColorSupport;
    }
}
