// Copyright 2022 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSE ファイルにあるMITライセンスに従います。
#include "ftxui/component/loop.hpp"

#include <utility>  // for move

#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive, Component

namespace ftxui {

/// @brief LoopはComponentとScreenInteractiveのラッパーです。
/// これはターミナルでコンポーネントを実行するために使用されます。
/// @see Component, ScreenInteractive.
/// @see ScreenInteractive::Loop().
/// @see ScreenInteractive::ExitLoop().
/// @param[in] screen 使用するスクリーン。
/// @param[in] component 実行するコンポーネント。
// NOLINTNEXTLINE
Loop::Loop(ScreenInteractive* screen, Component component)
    : screen_(screen), component_(std::move(component)) {
  screen_->PreMain();
}

Loop::~Loop() {
  screen_->PostMain();
}

/// @brief ループが終了したかどうか。
bool Loop::HasQuitted() {
  return screen_->HasQuitted();
}

/// @brief ループを実行します。`component`に保留中のすべてのタスク/イベントを処理させます。
/// 前のフレームが無効になった場合、新しいフレームが描画される可能性があります。
/// ループが完了するまでtrueを返します。
void Loop::RunOnce() {
  screen_->RunOnce(component_);
}

/// @brief 少なくとも1つのイベントが処理されるのを待ち、`Loop::RunOnce()`を実行します。
void Loop::RunOnceBlocking() {
  screen_->RunOnceBlocking(component_);
}

/// ループが終了するまで、現在のスレッドをブロックしてループを実行します。
void Loop::Run() {
  while (!HasQuitted()) {
    RunOnceBlocking();
  }
}

}  // namespace ftxui