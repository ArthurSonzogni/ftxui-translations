// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <functional>  // for function
#include <utility>     // for move

#include "ftxui/component/component.hpp"  // for Make, CatchEvent, ComponentDecorator
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/event.hpp"           // for Event

namespace ftxui {

class CatchEventBase : public ComponentBase {
 public:
  // 建構函式。
  explicit CatchEventBase(std::function<bool(Event)> on_event)
      : on_event_(std::move(on_event)) {}

  // 元件實作。
  Component ActiveChild() override {
    return Active() ? ComponentBase::ActiveChild() : nullptr;
  }

  bool OnEvent(Event event) override {
    if (on_event_(event)) {
      return true;
    } else {
      return ComponentBase::OnEvent(event);
    }
  }

 protected:
  std::function<bool(Event)> on_event_;
};

/// @brief 傳回一個元件，使用 |on_event| 來攔截事件。此函式
/// 在事件已被處理時必須回傳 true，否則回傳 false。
/// @param child 被包裝的元件。
/// @param on_event 繪製介面的函式。
/// @ingroup component
///
/// ### 範例
///
/// ```cpp
/// auto screen = App::TerminalOutput();
/// auto renderer = Renderer([] {
///   return text("My interface");
/// });
/// auto component = CatchEvent(renderer, [&](Event event) {
///   if (event == Event::Character('q')) {
///     screen.ExitLoopClosure()();
///     return true;
///   }
///   return false;
/// });
/// screen.Loop(component);
/// ```
Component CatchEvent(Component child,
                     std::function<bool(Event event)> on_event) {
  auto out = Make<CatchEventBase>(std::move(on_event));
  out->Add(std::move(child));
  return out;
}

/// @brief 裝飾一個元件，使用 |on_event| 來攔截事件。此函式
/// 在事件已被處理時必須回傳 true，否則回傳 false。
/// @param on_event 繪製介面的函式。
/// @ingroup component
///
/// ### 範例
///
/// ```cpp
/// auto screen = App::TerminalOutput();
/// auto renderer = Renderer([] { return text("Hello world"); });
/// renderer |= CatchEvent([&](Event event) {
///   if (event == Event::Character('q')) {
///     screen.ExitLoopClosure()();
///     return true;
///   }
///   return false;
/// });
/// screen.Loop(renderer);
/// ```
ComponentDecorator CatchEvent(std::function<bool(Event)> on_event) {
  return [on_event = std::move(on_event)](Component child) {
    return CatchEvent(std::move(child), [on_event = on_event](Event event) {
      return on_event(std::move(event));
    });
  };
}

}  // namespace ftxui
