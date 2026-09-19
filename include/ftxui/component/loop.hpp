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

/// @brief Loopは、コンポーネントのイベントループを管理するクラスです。
///
/// コンポーネントの実行、イベントの処理、画面の更新を担当します。
///
/// Loopクラスは、ターミナル画面を表すAppオブジェクトと
/// 一緒に使用するように設計されています。
///
/// **例**
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
///  // または
///  loop.Run();  // コンポーネントが終了するまでブロックします。
///
///  // または
///  loop.RunOnce();  // ノンブロッキングで、即座に戻ります。
///
///  // または
///  loop.RunOnceBlocking();  // 1つのイベントを処理するまでブロックします。
///
///  // またはループ内で:
///  while (!loop.HasQuitted()) {
///    loop.RunOnce();
///
///    // 別のライブラリのループ関数を実行するなど、他の処理を行います。
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

  // このクラスはコピー/ムーブ不可です。
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
