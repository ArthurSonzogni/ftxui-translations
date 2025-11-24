/// @module ftxui.screen:Screen
/// @brief Screen 模組中 Screen 類的模組文件

module;

#include <ftxui/screen/screen.hpp>

export module ftxui.screen:Screen;

/**
 * @namespace ftxui
 * @brief FTXUI 的 ftxui:: 命名空間
 */
export namespace ftxui {
    /**
     * @namespace Dimension
     * @brief FTXUI 的 ftxui::Dimension:: 命名空間
     */
    namespace Dimension {
        using ftxui::Dimension::Fixed;
        using ftxui::Dimension::Full;
    }

    using ftxui::Image;
    using ftxui::Screen;
}
