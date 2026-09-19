// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <ftxui/component/event.hpp>
#include <functional>  // for function
#include <string>      // for string

#include <utility>
#include "ftxui/component/component.hpp"  // for Maybe, Checkbox, Make, Radiobox, Vertical, Dropdown
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/component_options.hpp"  // for CheckboxOption, EntryState
#include "ftxui/dom/elements.hpp"  // for operator|, Element, border, filler, operator|=, separator, size, text, vbox, frame, vscroll_indicator, hbox, HEIGHT, LESS_THAN, bold, inverted
#include "ftxui/screen/util.hpp"   // for clamp
#include "ftxui/util/ref.hpp"      // for ConstStringListRef

namespace ftxui {

/// @brief 下拉式選單。
/// @ingroup component
/// @param entries 要顯示的項目清單。
/// @param selected 選定項目的索引。
Component Dropdown(ConstStringListRef entries, int* selected) {
  DropdownOption option;
  option.radiobox.entries = std::move(entries);
  option.radiobox.selected = selected;
  return Dropdown(option);
}

/// @brief 下拉式選單。
/// @ingroup component
/// @param option 下拉式選單的選項。
// NOLINTNEXTLINE
Component Dropdown(DropdownOption option) {
  class Impl : public ComponentBase, public DropdownOption {
   public:
    explicit Impl(DropdownOption option) : DropdownOption(std::move(option)) {
      FillDefault();
      checkbox_ = Checkbox(checkbox);
      radiobox_ = Radiobox(radiobox);

      container_ = Container::Vertical({
          checkbox_,
          Maybe(radiobox_, checkbox.checked),
      });
      Add(container_);
    }

    Element OnRender() override {
      selected_ =
          util::clamp(radiobox.selected(), 0, int(radiobox.entries.size()) - 1);
      selected_ = util::clamp(selected_(), 0, int(radiobox.entries.size()) - 1);

      if (selected_() >= 0 && selected_() < int(radiobox.entries.size())) {
        title_ = radiobox.entries[selected_()];
      }

      // 當另一個元件取得焦點時，關閉下拉選單。這可能
      // 在此下拉選單沒有收到任何事件的情況下發生，例如當使用者
      // 點擊兄弟下拉選單時。將內部焦點移回
      // 核取方塊，而不從其他元件搶走焦點。
      if (open_() && !Focused()) {
        container_->SetActiveChild(checkbox_);
        *open_ = false;
      }

      return transform(*open_, checkbox_->Render(), radiobox_->Render());
    }

    // 選取核取方塊時，在核取方塊與單選方塊之間切換焦點。
    bool OnEvent(ftxui::Event event) override {
      const bool open_old = open_();
      const int selected_old = selected_();
      bool handled = ComponentBase::OnEvent(event);

      // 當下拉選單開啟時，將焦點轉移到單選方塊。
      if (!open_old && open_()) {
        radiobox_->TakeFocus();
      }

      // 當使用者選擇一個項目時自動關閉下拉式選單，即使該項目與前一個相同。      if (open_old && open_()) {
      if (open_old && open_()) {
        const bool should_close =
            (selected_() != selected_old) ||     //
            (event == Event::Return) ||          //
            (event == Event::Character(' ')) ||  //
            (event == Event::Escape) ||          //
            (event.is_mouse() && event.mouse().button == Mouse::Left &&
             event.mouse().motion == Mouse::Pressed);

        if (should_close) {
          checkbox_->TakeFocus();
          *open_ = false;
          handled = true;
        }
      }

      return handled;
    }

    void FillDefault() {
      open_ = checkbox.checked;
      selected_ = radiobox.selected;
      checkbox.checked = &*open_;
      radiobox.selected = &*selected_;
      checkbox.label = &title_;

      if (!checkbox.transform) {
        checkbox.transform = [](const EntryState& s) {
          auto prefix = text(s.state ? "↓ " : "→ ");  // NOLINT
          auto t = text(s.label);
          if (s.active) {
            t |= bold;
          }
          if (s.focused) {
            t |= inverted;
          }
          return hbox({prefix, t});
        };
      }

      if (!transform) {
        transform = [](bool is_open, Element checkbox_element,
                       Element radiobox_element) {
          if (is_open) {
            const int max_height = 12;
            return vbox({
                       std::move(checkbox_element),
                       separator(),
                       std::move(radiobox_element) | vscroll_indicator | frame |
                           size(HEIGHT, LESS_THAN, max_height),
                   }) |
                   border;
          }
          return vbox({std::move(checkbox_element), filler()}) | border;
        };
      }
    }

   private:
    Ref<bool> open_;
    Ref<int> selected_;
    Component container_;
    Component checkbox_;
    Component radiobox_;
    std::string title_;
  };

  return Make<Impl>(option);
}

}  // namespace ftxui
