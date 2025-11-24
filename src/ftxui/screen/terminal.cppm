/// @module ftxui.screen:Terminal
/// @brief ScreenモジュールのTerminal名前空間のモジュールファイル

module;

#include <ftxui/screen/terminal.hpp>

export module ftxui.screen:Terminal;

/**
 * @namespace ftxui
 * @brief FTXUI ftxui::名前空間
 */
export namespace ftxui {
    using ftxui::Dimensions;

    /**
     * @namespace Terminal
     * @brief FTXUI ftxui::Terminal::名前空間
     */
    namespace Terminal {
        using ftxui::Terminal::Size;
        using ftxui::Terminal::SetFallbackSize;
        using ftxui::Terminal::Color;
        using ftxui::Terminal::ColorSupport;
        using ftxui::Terminal::SetColorSupport;
    }
}