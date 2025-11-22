/// @module ftxui.util:Ref
/// @brief Util 模块中 Ref 类的模块文件

module;

#include <ftxui/util/ref.hpp>

export module ftxui.util:Ref;

/**
 * @namespace ftxui
 * @brief FTXUI ftxui:: 命名空间
 */
export namespace ftxui {
    using ftxui::ConstRef;
    using ftxui::Ref;
    using ftxui::StringRef;
    using ftxui::ConstStringRef;
    using ftxui::ConstStringListRef;
}
