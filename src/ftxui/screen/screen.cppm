/// @module ftxui.screen:Screen
/// @brief Screen 模块的 Screen 类的模块文件

module;

#include <ftxui/screen/screen.hpp>

export module ftxui.screen:Screen;

/**
 * @namespace ftxui
 * @brief FTXUI ftxui:: 命名空间
 */
export namespace ftxui {
    /**
     * @namespace Dimension
     * @brief FTXUI ftxui::Dimension:: 命名空间
     */
    namespace Dimension {
        using ftxui::Dimension::Fixed;
        using ftxui::Dimension::Full;
    }

    using ftxui::Image;
    using ftxui::Screen;
}
