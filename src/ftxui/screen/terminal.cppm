/// @module ftxui.screen:Terminal
/// @brief Fichier de module pour l'espace de noms Terminal du module Screen

module;

#include <ftxui/screen/terminal.hpp>

export module ftxui.screen:Terminal;

/**
 * @namespace ftxui
/// @brief L'espace de noms FTXUI ftxui::
 */
export namespace ftxui {
    using ftxui::Dimensions;

    /**
     * @namespace Terminal
/// @brief L'espace de noms FTXUI ftxui::Terminal::
     */
    namespace Terminal {
        using ftxui::Terminal::Size;
        using ftxui::Terminal::SetFallbackSize;
        using ftxui::Terminal::Color;
        using ftxui::Terminal::ColorSupport;
        using ftxui::Terminal::SetColorSupport;
    }
}
