/// @module ftxui.screen:Terminal
/// @brief Screen 模組中 Terminal 命名空間的模組檔案

module;

#include <ftxui/screen/terminal.hpp>

export module ftxui.screen:Terminal;

/**
 * @namespace ftxui
 * @brief FTXUI ftxui:: 命名空間
 */
export namespace ftxui {
    using ftxui::Dimensions;

    /**
     * @namespace Terminal
     * @brief FTXUI ftxui::Terminal:: 命名空間
     */
    namespace Terminal {
        using ftxui::Terminal::Size;
        using ftxui::Terminal::SetFallbackSize;
        using ftxui::Terminal::Color;
        using ftxui::Terminal::ColorSupport;
        using ftxui::Terminal::SetColorSupport;
    }
}
