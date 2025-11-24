// Copyright 2021 Arthur Sonzogni. All rights reserved.
// このソースコードは、LICENSEファイルにあるMITライセンスに基づいて使用されます。
#include <functional>  // for function
#include <utility>     // for move

#include "ftxui/component/component.hpp"  // for Checkbox, Maybe, Make, Vertical, Collapsible
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/component_options.hpp"  // for CheckboxOption, EntryState
#include "ftxui/dom/elements.hpp"  // for operator|=, text, hbox, Element, bold, inverted
#include "ftxui/util/ref.hpp"  // for Ref, ConstStringRef

namespace ftxui {

/// @brief 折りたたみ可能なコンポーネント。矢印付きのチェックボックスを表示します。アクティブ化されると、子が
/// 表示されます。
/// @param label チェックボックスのラベル。
/// @param child 表示する子。
/// @param show 子が表示されているかどうかを示す状態を保持します。
///
/// ### 例
/// ```cpp
/// auto component = Collapsible("Show details", details);
/// ```
///
/// ### 出力
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
