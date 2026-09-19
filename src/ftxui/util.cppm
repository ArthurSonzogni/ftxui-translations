/// @module ftxui.util
/// @brief Archivo de módulo para operaciones de utilidad de FTXUI.

module;

#include <ftxui/util/autoreset.hpp>
#include <ftxui/util/ref.hpp>

export module ftxui.util;

/**
 * @namespace ftxui
 * @brief El espacio de nombres ftxui:: de FTXUI
 */
export namespace ftxui {
    using ftxui::AutoReset;
    using ftxui::ConstRef;
    using ftxui::ConstStringListRef;
    using ftxui::ConstStringRef;
    using ftxui::Ref;
    using ftxui::StringRef;
}
