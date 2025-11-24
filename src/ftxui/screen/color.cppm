/// @module ftxui.screen:Color
/// @brief ScreenモジュールのColorクラスのモジュールファイル

module;

#include <ftxui/screen/color.hpp>

export module ftxui.screen:Color;

/**
 * @namespace ftxui
 * @brief FTXUI ftxui::名前空間
 */
export namespace ftxui {
    using ftxui::Color;

    /**
     * @namespace literals
     * @brief FTXUI ftxui::literals::名前空間
     */
    inline namespace literals {
        using ftxui::literals::operator""_rgb;
    }
}
