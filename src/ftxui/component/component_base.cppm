/// @module ftxui.component:ComponentBase
/// @brief 元件模組中 ComponentBase 類的模組文件

module;

#include <ftxui/component/component_base.hpp>

export module ftxui.component:ComponentBase;

/**
 * @namespace ftxui
 * @brief FTXUI 的 ftxui:: 命名空間
 */
export namespace ftxui {
    using ftxui::Delegate;
    using ftxui::Focus;
    using ftxui::Event;

    /**
     * @namespace animation
     * @brief FTXUI 的 ftxui::animation:: 命名空間
     */
    namespace animation {
        using ftxui::animation::Params;
    }

    using ftxui::ComponentBase;
    using ftxui::Component;
    using ftxui::Components;
}
