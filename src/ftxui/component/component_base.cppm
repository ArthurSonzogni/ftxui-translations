/// @module ftxui.component:ComponentBase
/// @brief ComponentモジュールのComponentBaseクラスのモジュールファイル

module;

#include <ftxui/component/component_base.hpp>

export module ftxui.component:ComponentBase;

/**
 * @namespace ftxui
 * @brief FTXUI ftxui::名前空間
 */
export namespace ftxui {
    using ftxui::Delegate;
    using ftxui::Focus;
    using ftxui::Event;

    /**
     * @namespace animation
     * @brief FTXUI ftxui::animation::名前空間
     */
    namespace animation {
        using ftxui::animation::Params;
    }

    using ftxui::ComponentBase;
    using ftxui::Component;
    using ftxui::Components;
}
