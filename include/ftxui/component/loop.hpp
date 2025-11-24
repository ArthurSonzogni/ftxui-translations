// Copyright 2022 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスによって管理されます。
#ifndef FTXUI_COMPONENT_LOOP_HPP
#define FTXUI_COMPONENT_LOOP_HPP

#include <memory>  // for shared_ptr

#include "ftxui/component/component_base.hpp"  // for ComponentBase

namespace ftxui {
class ComponentBase;

using Component = std::shared_ptr<ComponentBase>;
class ScreenInteractive;

/// @brief Loopは、コンポーネントのイベントループを管理するクラスです。
///
/// コンポーネントの実行、イベントの処理、および画面の更新を担当します。
///
/// Loopクラスは、ターミナル画面を表すScreenInteractiveオブジェクトと共に使用するように設計されています。
///
/// **例**
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
///  // いずれか
///  loop.Run();  // コンポーネントが終了するまでブロックします。
///
///  // または
///  loop.RunOnce();  // 非ブロッキングで、すぐに戻ります。
///
///  // または
///  loop.RunOnceBlocking();  // 1つのイベントを処理するまでブロックします。
///
///  // またはループで:
///  while (!loop.HasQuitted()) {
///    loop.RunOnce();
///
///    // 別のライブラリのループ関数を実行するなど、他のことを行います。
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

  // このクラスはコピー/移動できません。
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