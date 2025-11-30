/// @module ftxui.screen:Color
/// @brief Archivo de módulo para la clase Color del módulo Screen

module;

#include <ftxui/screen/color.hpp>

export module ftxui.screen:Color;

/**
 * @namespace ftxui
 * @brief El espacio de nombres ftxui:: de FTXUI
 */
export namespace ftxui {
    using ftxui::Color;

    /**
     * @namespace literals
     * @brief El espacio de nombres ftxui::literals:: de FTXUI
     */
    inline namespace literals {
        using ftxui::literals::operator""_rgb;
    }
}
