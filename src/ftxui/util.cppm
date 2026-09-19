/// @module ftxui.util
/// @brief FTXUIユーティリティ操作用のモジュールファイル。

module;

#include <ftxui/util/autoreset.hpp>
#include <ftxui/util/ref.hpp>

export module ftxui.util;

/**
 * @namespace ftxui
 * @brief FTXUI ftxui::名前空間
 */
export namespace ftxui {
    using ftxui::AutoReset;
    using ftxui::ConstRef;
    using ftxui::ConstStringListRef;
    using ftxui::ConstStringRef;
    using ftxui::Ref;
    using ftxui::StringRef;
}
