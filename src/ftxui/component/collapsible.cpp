// Copyright 2021 Arthur Sonzogni. All rights reserved.
// 本源代码的使用受 MIT 许可的约束，
// 该许可可在 LICENSE 文件中找到。
#include <functional>  // for function
#include <utility>     // for move

#include "ftxui/component/component.hpp"  // for Checkbox, Maybe, Make, Vertical, Collapsible
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/component_options.hpp"  // for CheckboxOption, EntryState
#include "ftxui/dom/elements.hpp"  // for operator|=, text, hbox, Element, bold, inverted
#include "ftxui/util/ref.hpp"  // for Ref, ConstStringRef

namespace ftxui {

/// @brief 一个可折叠的组件。它显示一个带箭头的复选框。一旦激活，子组件就会显示。
/// @param label 复选框的标签。
/// @param child 要显示的子组件。
/// @param show 保持子组件是否显示的状态。
///
/// ### 示例
/// ```cpp
/// auto component = Collapsible("显示详情", details);

/// ```
///
/// ### 输出
/// ```
///
/// ▼ 显示详情
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
