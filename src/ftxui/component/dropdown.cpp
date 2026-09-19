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

/// @brief ドロップダウンメニュー。
/// @ingroup component
/// @param entries 表示するエントリのリスト。
/// @param selected 選択されたエントリのインデックス。
Component Dropdown(ConstStringListRef entries, int* selected) {
  DropdownOption option;
  option.radiobox.entries = std::move(entries);
  option.radiobox.selected = selected;
  return Dropdown(option);
}

/// @brief ドロップダウンメニュー。
/// @ingroup component
/// @param option ドロップダウンのオプション。
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

      // 別のコンポーネントがフォーカスを取得したとき、ドロップダウンを
      // 閉じます。これは、このドロップダウンが何のイベントも受け取らずに
      // 発生する可能性があります。例えば、ユーザーが兄弟の
      // ドロップダウンをクリックした場合です。他のコンポーネントから
      // フォーカスを奪わずに、内部フォーカスをチェックボックスに戻します。
      if (open_() && !Focused()) {
        container_->SetActiveChild(checkbox_);
        *open_ = false;
      }

      return transform(*open_, checkbox_->Render(), radiobox_->Render());
    }

    // 選択する際に、チェックボックスとラジオボックスの間でフォーカスを
// 切り替えます。
    bool OnEvent(ftxui::Event event) override {
      const bool open_old = open_();
      const int selected_old = selected_();
      bool handled = ComponentBase::OnEvent(event);

      // ドロップダウンが開かれたとき、ラジオボックスにフォーカスを転送します。
      if (!open_old && open_()) {
        radiobox_->TakeFocus();
      }

      // ユーザーが項目を選択したとき、それが前と同じ項目であっても
      // ドロップダウンを自動的に閉じます。
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
