/// @module ftxui.screen:Screen
/// @brief ScreenモジュールのScreenクラスのためのモジュールファイル

module;

#include <ftxui/screen/screen.hpp>

export module ftxui.screen:Screen;

/**
 * @namespace ftxui
 * @brief FTXUI ftxui::名前空間
 */
export namespace ftxui {
    /**
     * @namespace Dimension
     * @brief FTXUI ftxui::Dimension::名前空間
     */
    namespace Dimension {
        using ftxui::Dimension::Fixed;
        using ftxui::Dimension::Full;
    }

    using ftxui::Image;
    using ftxui::Screen;
}
