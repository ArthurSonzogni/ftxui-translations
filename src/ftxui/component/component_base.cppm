/// @module ftxui.component:ComponentBase
/// @brief Fichier module pour la classe ComponentBase du module Component.

module;

#include <ftxui/component/component_base.hpp>

export module ftxui.component:ComponentBase;

/**
 * @namespace ftxui
 * @brief L'espace de noms FTXUI ftxui::
 */
export namespace ftxui {
    using ftxui::Delegate;
    using ftxui::Focus;
    using ftxui::Event;

    /**
     * @namespace animation
     * @brief L'espace de noms FTXUI ftxui::animation::
     */
    namespace animation {
        using ftxui::animation::Params;
    }

    using ftxui::ComponentBase;
    using ftxui::Component;
    using ftxui::Components;
}