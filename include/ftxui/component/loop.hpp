// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_COMPONENT_LOOP_HPP
#define FTXUI_COMPONENT_LOOP_HPP

#include <memory>  // 用于 shared_ptr

/// #include "ftxui/component/component_base.hpp"  // 用于 ComponentBase

namespace ftxui {
class ComponentBase;

using Component = std::shared_ptr<ComponentBase>;
class ScreenInteractive;

/// @brief Loop 是一个管理组件事件循环的类。
///
/// 它负责运行组件、处理事件和更新屏幕。
///
/// Loop 类设计用于 ScreenInteractive 对象，
/// 该对象表示终端屏幕。
///
/// **示例**
/// ```cpp
/// #include <ftxui/component/component.hpp>
/// #include <ftxui/component/screen_interactive.hpp>
/// #include <ftxui/component/loop.hpp>
///
/// int main() {
///  auto screen = ftxui::ScreenInteractive::TerminalOutput();
///  auto component = ftxui::Button("Click me", [] { ... });
///
///  ftxui::Loop loop(screen.get(), component);
///
///  // Either
///  loop.Run();  // Blocking until the component quits.
///
///  // Or
///  loop.RunOnce();  // Non-blocking, returns immediately.
///
///  // Or
///  loop.RunOnceBlocking();  // Blocking until handling one event.
///
///  // Or in a loop:
///  while (!loop.HasQuitted()) {
///    loop.RunOnce();
///
///    // Do something else like running a different library loop function.
///  }
/// }
/// ```
///
/// @ingroup component
class Loop {
 public:
  Loop(ScreenInteractive* screen, Component component);
  ~Loop();

  bool HasQuitted();
  void RunOnce();
  void RunOnceBlocking();
  void Run();

  // 此类不可复制/移动。
  Loop(const Loop&) = default;
  Loop(Loop&&) = delete;
  Loop& operator=(Loop&&) = delete;
  Loop(const ScreenInteractive&) = delete;
  Loop& operator=(const Loop&) = delete;

 private:
  ScreenInteractive* screen_;
  Component component_;
};

}  // namespace ftxui

#endif  // FTXUI_COMPONENT_LOOP_HPP
