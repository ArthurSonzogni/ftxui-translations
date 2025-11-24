/// @module ftxui.component:Component
/// @brief 元件模組的元件類別模組檔案

module;

#include <ftxui/component/component.hpp>

export module ftxui.component:Component;

/**
 * @namespace ftxui
 * @brief FTXUI ftxui:: 命名空間
 */
export namespace ftxui {
    using ftxui::ButtonOption;
    using ftxui::CheckboxOption;
    using ftxui::Event;
    using ftxui::InputOption;
    using ftxui::MenuOption;
    using ftxui::RadioboxOption;
    using ftxui::MenuEntryOption;

    using ftxui::Make;

    using ftxui::ComponentDecorator;
    using ftxui::ElementDecorator;

    using ftxui::operator|;
    using ftxui::operator|=;

    /**
     * @namespace Container
     * @brief FTXUI ftxui::Container:: 命名空間
     */
    namespace Container {
        using ftxui::Container::Vertical;
        using ftxui::Container::Horizontal;
        using ftxui::Container::Tab;
        using ftxui::Container::Stacked;
    }
    
    using ftxui::Button;
    using ftxui::Checkbox;
    using ftxui::Input;
    using ftxui::Menu;
    using ftxui::MenuEntry;
    using ftxui::Radiobox;
    using ftxui::Dropdown;
    using ftxui::Toggle;
    using ftxui::Slider;
    using ftxui::ResizableSplit;
    using ftxui::ResizableSplitLeft;
    using ftxui::ResizableSplitRight;
    using ftxui::ResizableSplitTop;
    using ftxui::ResizableSplitBottom;
    using ftxui::Renderer;
    using ftxui::CatchEvent;
    using ftxui::Maybe;
    using ftxui::Modal;
    using ftxui::Collapsible;
    using ftxui::Hoverable;
    using ftxui::Window;
}