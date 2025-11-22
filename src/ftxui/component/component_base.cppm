/// @module ftxui.component:ComponentBase
/// @brief Component 模块的 ComponentBase 类的模块文件

module;

#include <ftxui/component/component_base.hpp>

export module ftxui.component:ComponentBase;

    /**
     * @namespace ftxui
     * @brief FTXUI ftxui:: 命名空间
     */export namespace ftxui {
    using ftxui::Delegate;
    using ftxui::Focus;
    using ftxui::Event;

    /**
     * @namespace animation
     * @brief FTXUI ftxui::animation:: 命名空间
     */
    namespace animation {
        using ftxui::animation::Params;
    }

    using ftxui::ComponentBase;
    using ftxui::Component;
    using ftxui::Components;
}
