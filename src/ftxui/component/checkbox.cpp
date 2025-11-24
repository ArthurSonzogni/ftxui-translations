// Copyright 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，該條款可在
// LICENSE 文件中找到。
#include <functional>  // for function
#include <utility>     // for move

#include "ftxui/component/component.hpp"       // for Make, Checkbox
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/component_options.hpp"  // for CheckboxOption, EntryState
#include "ftxui/component/event.hpp"              // for Event, Event::Return
#include "ftxui/component/mouse.hpp"  // for Mouse, Mouse::Left, Mouse::Pressed
#include "ftxui/dom/elements.hpp"  // for operator|, Element, reflect, focus, nothing, select
#include "ftxui/screen/box.hpp"  // for Box
#include "ftxui/util/ref.hpp"    // for Ref, ConstStringRef

namespace ftxui {

namespace {
class CheckboxBase : public ComponentBase, public CheckboxOption {
 public:
  explicit CheckboxBase(CheckboxOption option)
      : CheckboxOption(std::move(option)) {}

 private:
  // 元件實作。
  Element OnRender() override {
    const bool is_focused = Focused();
    const bool is_active = Active();
    auto entry_state = EntryState{
        *label, *checked, is_active, is_focused || hovered_, -1,
    };
    auto element = (transform ? transform : CheckboxOption::Simple().transform)(
        entry_state);
    element |= focus;
    element |= reflect(box_);
    return element;
  }

  bool OnEvent(Event event) override {
    if (!CaptureMouse(event)) {
      return false;
    }

    if (event.is_mouse()) {
      return OnMouseEvent(event);
    }

    hovered_ = false;
    if (event == Event::Character(' ') || event == Event::Return) {
      *checked = !*checked;
      on_change();
      TakeFocus();
      return true;
    }
    return false;
  }

  bool OnMouseEvent(Event event) {
    hovered_ = box_.Contain(event.mouse().x, event.mouse().y);

    if (!CaptureMouse(event)) {
      return false;
    }

    if (!hovered_) {
      return false;
    }

    if (event.mouse().button == Mouse::Left &&
        event.mouse().motion == Mouse::Pressed) {
      *checked = !*checked;
      on_change();
      return true;
    }

    return false;
  }

  bool Focusable() const final { return true; }

  bool hovered_ = false;
  Box box_;
};
}  // namespace

/// @param option 額外的可選參數。
/// @ingroup component
/// @see CheckboxBase
///
/// ### 範例
///
/// ```cpp
/// auto screen = ScreenInteractive::FitComponent();
/// CheckboxOption option;
/// option.label = "製作一個三明治";
/// option.checked = false;
/// Component checkbox = Checkbox(option);
/// screen.Loop(checkbox)
/// ```
///
/// ### 輸出
///
/// ```bash
/// ☐ 製作一個三明治
/// ```
// NOLINTNEXTLINE
Component Checkbox(CheckboxOption option) {
  return Make<CheckboxBase>(std::move(option));
}

/// @brief 繪製可勾選元素。
/// @param label 複選框的標籤。
/// @param checked 複選框是否被選中。
/// @param option 額外的可選參數。
/// @ingroup component
/// @see CheckboxBase
///
/// ### 範例
///
/// ```cpp
/// auto screen = ScreenInteractive::FitComponent();
/// std::string label = "製作一個三明治";
/// bool checked = false;
/// Component checkbox = Checkbox(&label, &checked);
/// screen.Loop(checkbox)
/// ```
///
/// ### 輸出
///
/// ```bash
/// ☐ 製作一個三明治
/// ```
// NOLINTNEXTLINE
Component Checkbox(ConstStringRef label, bool* checked, CheckboxOption option) {
  option.label = std::move(label);
  option.checked = checked;
  return Make<CheckboxBase>(std::move(option));
}

}  // namespace ftxui
