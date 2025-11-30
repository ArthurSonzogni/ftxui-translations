/// @module ftxui.component:ComponentBase
/// @brief Archivo de módulo para la clase ComponentBase del módulo Component.

module;

#include <ftxui/component/component_base.hpp>

export module ftxui.component:ComponentBase;

/**
 * @namespace ftxui
 * @brief El espacio de nombres ftxui:: de FTXUI
 */
export namespace ftxui {
    using ftxui::Delegate;
    using ftxui::Focus;
    using ftxui::Event;

    /**
     * @namespace animation
     * @brief El espacio de nombres ftxui::animation:: de FTXUI
     */
    namespace animation {
        using ftxui::animation::Params;
    }

    using ftxui::ComponentBase;
    using ftxui::Component;
    using ftxui::Components;
}
