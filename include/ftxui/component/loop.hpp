// 版權所有 2022 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱
// LICENSE 文件。
#ifndef FTXUI_COMPONENT_LOOP_HPP
#define FTXUI_COMPONENT_LOOP_HPP

#include <memory>  // for shared_ptr

#include "ftxui/component/component_base.hpp"  // for ComponentBase

namespace ftxui {
class ComponentBase;

using Component = std::shared_ptr<ComponentBase>;
class ScreenInteractive;

/// @brief Loop 是一個用於管理元件事件循環的類別。
///
/// 它負責執行元件、處理事件，並
/// 更新畫面。
///
/// Loop 類別旨在與 ScreenInteractive 物件一起使用，
/// 該物件代表終端機畫面。
///
/// **範例**
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
///  // 方法一
///  loop.Run();  // 阻塞直到元件退出。
///
///  // 方法二
///  loop.RunOnce();  // 非阻塞，立即返回。
///
///  // 方法三
///  loop.RunOnceBlocking();  // 阻塞直到處理一個事件。
///
///  // 方法四：在循環中：
///  while (!loop.HasQuitted()) {
///    loop.RunOnce();
///
///    // 執行其他操作，例如運行不同的函式庫循環函式。
///  }
/// }
/// ```
///
/// @ingroup 元件
class Loop {
 public:
  Loop(ScreenInteractive* screen, Component component);
  ~Loop();

  bool HasQuitted();
  void RunOnce();
  void RunOnceBlocking();
  void Run();

  // This class is non copyable/movable.
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
