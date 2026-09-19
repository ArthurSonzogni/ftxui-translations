// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_COMPONENT_LOOP_HPP
#define FTXUI_COMPONENT_LOOP_HPP

#include <memory>  // for shared_ptr

#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/util/export.hpp"               // for FTXUI_EXPORT

namespace ftxui {
class ComponentBase;

using Component = std::shared_ptr<ComponentBase>;
class App;

/// @brief Loop 是一個管理元件事件迴圈的類別。
///
/// 它負責執行元件、處理事件，以及
/// 更新畫面。
///
/// Loop 類別設計用來與代表終端機畫面的 App 物件
/// 一起使用。
///
/// **範例**
/// ```cpp
/// #include <ftxui/component/component.hpp>
/// #include <ftxui/component/app.hpp>
/// #include <ftxui/component/loop.hpp>
///
/// int main() {
///  auto screen = ftxui::App::TerminalOutput();
///  auto component = ftxui::Button("Click me", [] { ... });
///
///  ftxui::Loop loop(screen.get(), component);
///
///  // 或者
///  loop.Run();  // 阻塞直到元件結束。
///
///  // 或者
///  loop.RunOnce();  // 非阻塞，立即返回。
///
///  // 或者
///  loop.RunOnceBlocking();  // 阻塞直到處理一個事件為止。
///
///  // 或者在迴圈中：
///  while (!loop.HasQuitted()) {
///    loop.RunOnce();
///
///    // 做一些其他事情，例如執行不同函式庫的迴圈函式。
///  }
/// }
/// ```
///
/// @ingroup component
class FTXUI_EXPORT(COMPONENT) Loop {
 public:
  Loop(App* screen, Component component);
  ~Loop();

  bool HasQuitted();
  void RunOnce();
  void RunOnceBlocking();
  void Run();

  // 此類別不可複製/移動。
  Loop& operator=(Loop&&) = delete;
  Loop& operator=(const Loop&) = delete;
  Loop(Loop&&) = delete;
  Loop(const Loop&) = delete;

 private:
  App* screen_;
  Component component_;
};

}  // namespace ftxui

#endif  // FTXUI_COMPONENT_LOOP_HPP
