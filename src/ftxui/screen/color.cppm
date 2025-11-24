/// @module ftxui.screen:Color
/// @brief Screen 模組中 Color 類別的模組文件

module;

#include <ftxui/screen/color.hpp>

export module ftxui.screen:Color;

/**
 * @namespace ftxui
      * @brief FTXUI ftxui:: 命名空間 */
export namespace ftxui {
    using ftxui::Color;

    /**
     * @namespace literals
     * @brief FTXUI ftxui::literals:: 命名空間
     */
    inline namespace literals {
        using ftxui::literals::operator""_rgb;
    }
}
