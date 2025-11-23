/// @module ftxui.screen:Screen
/// @brief Fichier module pour la classe Screen du module Screen

module;

#include <ftxui/screen/screen.hpp>

export module ftxui.screen:Screen;

/**
 * @namespace ftxui
 * @brief L'espace de noms FTXUI ftxui::
 */
export namespace ftxui {
    /**
     * @namespace Dimension
     * @brief L'espace de noms FTXUI ftxui::Dimension::
     */
    namespace Dimension {
        using ftxui::Dimension::Fixed;
        using ftxui::Dimension::Full;
    }

    using ftxui::Image;
    using ftxui::Screen;
}
