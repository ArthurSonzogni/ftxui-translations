/// @module ftxui.screen:Terminal
/// @brief Archivo de módulo para el espacio de nombres Terminal del módulo Screen

module;

#include <ftxui/screen/terminal.hpp>

export module ftxui.screen:Terminal;

/**
 * @namespace ftxui
 * @brief El espacio de nombres ftxui:: de FTXUI
 */
export namespace ftxui {
    using ftxui::Dimensions;

    /**
     * @namespace Terminal
     * @brief El espacio de nombres ftxui::Terminal:: de FTXUI
     */
    namespace Terminal {
        using ftxui::Terminal::Size;
        using ftxui::Terminal::SetFallbackSize;
        using ftxui::Terminal::Color;
        using ftxui::Terminal::ColorSupport;
        using ftxui::Terminal::SetColorSupport;
    }
}