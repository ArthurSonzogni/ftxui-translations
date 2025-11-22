/// @module ftxui.screen:Color
/// @brief Screen模块中Color类的模块文件

module;

#include <ftxui/screen/color.hpp>

export module ftxui.screen:Color;

/**
 * @namespace ftxui
 * @brief FTXUI ftxui:: 命名空间
 */
export namespace ftxui {
    using ftxui::Color;

    /**
     * @namespace literals
     * @brief FTXUI ftxui::literals:: 命名空间
     */
    inline namespace literals {
        using ftxui::literals::operator""_rgb;
    }
}
