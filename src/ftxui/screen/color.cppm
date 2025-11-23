/// @module ftxui.screen:Couleur
/// @brief Fichier module pour la classe Color du module Screen

module;

#include <ftxui/screen/color.hpp>

export module ftxui.screen:Color;

/**
 * @namespace ftxui
 * @brief L'espace de noms FTXUI ftxui::
 */
export namespace ftxui {
    using ftxui::Color;

    /**
     * @namespace literals
     * @brief L'espace de noms FTXUI ftxui::literals::
     */
    inline namespace literals {
        using ftxui::literals::operator""_rgb;
    }
}
