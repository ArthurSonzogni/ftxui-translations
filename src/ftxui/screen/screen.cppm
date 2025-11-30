/// @module ftxui.screen:Screen
/// @brief Archivo de módulo para la clase Screen del módulo Screen

module;

#include <ftxui/screen/screen.hpp>

export module ftxui.screen:Screen;

/**
 * @namespace ftxui
 * @brief El espacio de nombres ftxui:: de FTXUI
 */
export namespace ftxui {
    /**
     * @namespace Dimension
     * @brief El espacio de nombres ftxui::Dimension:: de FTXUI
     */
    namespace Dimension {
        using ftxui::Dimension::Fixed;
        using ftxui::Dimension::Full;
    }

    using ftxui::Image;
    using ftxui::Screen;
}
