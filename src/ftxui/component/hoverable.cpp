// Copyright 2022 Arthur Sonzogni. All rights reserved.
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱 LICENSE 檔案。
#include <functional>  // for function
#include <utility>     // for move

#include "ftxui/component/component.hpp"  // for ComponentDecorator, Hoverable, Make
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/component/mouse.hpp"           // for Mouse
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, reflect, Element
#include "ftxui/screen/box.hpp"    // for Box

namespace ftxui {

namespace {

void Post(std::function<void()> f) {
  if (auto* screen = ScreenInteractive::Active()) {
    screen->Post(std::move(f));
    return;
  }
  f();
}

}  // namespace

/// @brief 包裝一個元件。提供能力以判斷滑鼠是否懸停在其上方。
/// @param component 被包裝的元件。
/// @param hover 反映元件是否被懸停的值。
/// @ingroup component
///
/// ### 範例
///
/// ```cpp
/// auto button = Button("exit", screen.ExitLoopClosure());
/// bool hover = false;
/// auto button_hover = Hoverable(button, &hover);
/// ```
// NOLINTNEXTLINE
Component Hoverable(Component component, bool* hover) {
  class Impl : public ComponentBase {
   public:
    Impl(Component component, bool* hover)
        : component_(std::move(component)), hover_(hover) {
      Add(component_);
    }

   private:
    Element OnRender() override {
      return ComponentBase::OnRender() | reflect(box_);
    }

    bool OnEvent(Event event) override {
      if (event.is_mouse()) {
        *hover_ = box_.Contain(event.mouse().x, event.mouse().y) &&
                  CaptureMouse(event);
      }

      return ComponentBase::OnEvent(event);
    }

    Component component_;
    bool* hover_;
    Box box_;
  };

  return Make<Impl>(component, hover);
}

/// @brief 包裝一個元件。使用回呼函數。
/// @param component 被包裝的元件。
/// @param on_enter 進入時的回呼函數
/// @param on_leave 離開時的回呼函數
/// @ingroup component
///
/// ### 範例
///
/// ```cpp
/// auto button = Button("exit", screen.ExitLoopClosure());
/// bool hover = false;
/// auto button_hover = Hoverable(button, &hover);
/// ```
Component Hoverable(Component component,
                    std::function<void()> on_enter,
                    std::function<void()> on_leave) {
  class Impl : public ComponentBase {
   public:
    Impl(Component component,
         std::function<void()> on_enter,
         std::function<void()> on_leave)
        : component_(std::move(component)),
          on_enter_(std::move(on_enter)),
          on_leave_(std::move(on_leave)) {
      Add(component_);
    }

   private:
    Element OnRender() override {
      return ComponentBase::OnRender() | reflect(box_);
    }

    bool OnEvent(Event event) override {
      if (event.is_mouse()) {
        const bool hover = box_.Contain(event.mouse().x, event.mouse().y) &&
                           CaptureMouse(event);
        if (hover != hover_) {
          Post(hover ? on_enter_ : on_leave_);
        }
        hover_ = hover;
      }

      return ComponentBase::OnEvent(event);
    }

    Component component_;
    Box box_;
    bool hover_ = false;
    std::function<void()> on_enter_;
    std::function<void()> on_leave_;
  };

  return Make<Impl>(std::move(component), std::move(on_enter),
                    std::move(on_leave));
}

/// @brief 包裝一個元件。提供能力以判斷滑鼠是否懸停在其上方。
/// @param hover 反映元件是否被懸停的值。
/// @ingroup component
///
/// ### 範例
///
/// ```cpp
/// bool hover = false;
/// auto button = Button("exit", screen.ExitLoopClosure());
/// button |= Hoverable(&hover);
/// ```
ComponentDecorator Hoverable(bool* hover) {
  return [hover](Component component) {
    return Hoverable(std::move(component), hover);
  };
}

/// @brief 包裝一個元件。提供能力以判斷滑鼠是否懸停在其上方。
/// @param on_enter 當滑鼠懸停在元件上時呼叫。
/// @param on_leave 當滑鼠離開元件時呼叫。
/// @ingroup component
///
/// ### 範例
///
/// ```cpp
/// auto button = Button("exit", screen.ExitLoopClosure());
/// int on_enter_cnt = 0;
/// int on_leave_cnt = 0;
/// button |= Hoverable(
///   [&]{ on_enter_cnt++; },
///   [&]{ on_leave_cnt++; }
/// );
/// ```
// NOLINTNEXTLINE
ComponentDecorator Hoverable(std::function<void()> on_enter,
                             // NOLINTNEXTLINE
                             std::function<void()> on_leave) {
  return [on_enter, on_leave](Component component) {
    return Hoverable(std::move(component), on_enter, on_leave);
  };
}

/// @brief 包裝一個元件。提供能力以判斷滑鼠是否懸停在其上方。
/// @param component 被包裝的元件。
/// @param on_change 當滑鼠進入或離開元件時呼叫。
/// @ingroup component
///
/// ### 範例
///
/// ```cpp
/// auto button = Button("exit", screen.ExitLoopClosure());
/// bool hovered = false;
/// auto button_hoverable = Hoverable(button,
///                                   [&](bool hover) { hovered = hover;});
/// ```
// NOLINTNEXTLINE
Component Hoverable(Component component, std::function<void(bool)> on_change) {
  return Hoverable(
      std::move(component),              //
      [on_change] { on_change(true); },  //
      [on_change] { on_change(false); }  //
  );
}

/// @brief 包裝一個元件。提供能力以判斷滑鼠是否懸停在其上方。
/// @param on_change 當滑鼠進入或離開元件時呼叫。
/// @ingroup component
///
/// ### 範例
///
/// ```cpp
/// auto button = Button("exit", screen.ExitLoopClosure());
/// bool hovered = false;
/// button |= Hoverable([&](bool hover) { hovered = hover;});
/// ```
// NOLINTNEXTLINE
ComponentDecorator Hoverable(std::function<void(bool)> on_change) {
  return [on_change](Component component) {
    return Hoverable(std::move(component), on_change);
  };
}

}  // namespace ftxui