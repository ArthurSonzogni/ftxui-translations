// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <functional>  // for function
#include <utility>     // for move

#include "ftxui/component/component.hpp"  // for Checkbox, Maybe, Make, Vertical, Collapsible
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/component_options.hpp"  // for CheckboxOption, EntryState
#include "ftxui/dom/elements.hpp"  // for operator|=, text, hbox, Element, bold, inverted
#include "ftxui/util/ref.hpp"  // for Ref, ConstStringRef

namespace ftxui {

/// @brief 可折疊元件。它顯示一個帶有箭頭的核取方塊。一旦啟用，子元件就會顯示。
/// @param label 核取方塊的標籤。
/// @param child 要顯示的子元件。
/// @param show 儲存子元件是否顯示的狀態。
///
/// ### 範例
/// ```cpp
/// auto component = Collapsible("Show details", details);
/// ```
///
/// ### 輸出
/// ```
///
/// ▼ Show details
/// <details component>
/// ```
// NOLINTNEXTLINE
Component Collapsible(ConstStringRef label, Component child, Ref<bool> show) {
  class Impl : public ComponentBase {
   public:
    Impl(ConstStringRef label, Component child, Ref<bool> show) : show_(show) {
      CheckboxOption opt;
      opt.transform = [](EntryState s) {            // NOLINT
        auto prefix = text(s.state ? "▼ " : "▶ ");  // NOLINT
        auto t = text(s.label);
        if (s.active) {
          t |= bold;
        }
        if (s.focused) {
          t |= inverted;
        }
        return hbox({prefix, t});
      };
      Add(Container::Vertical({
          Checkbox(std::move(label), show_.operator->(), opt),
          Maybe(std::move(child), show_.operator->()),
      }));
    }
    Ref<bool> show_;
  };

  return Make<Impl>(std::move(label), std::move(child), show);
}

}  // namespace ftxui