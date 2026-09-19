// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/component/loop.hpp"

#include <utility>  // for move

#include "ftxui/component/app.hpp"  // for App, Component

namespace ftxui {

/// @brief LoopはComponentとAppを包むラッパーです。
/// 端末でComponentを実行するために使用されます。
/// @see Component, App.
/// @see App::Loop().
/// @see App::Exit().
/// @param[in] screen 使用するスクリーン。
/// @param[in] component 実行するコンポーネント。
// NOLINTNEXTLINE
Loop::Loop(App* screen, Component component)
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

/// ループを実行し、ループが終了するまで現在のスレッドを
/// ブロックします。
void Loop::Run() {
  while (!HasQuitted()) {
    RunOnceBlocking();
  }
}

}  // namespace ftxui
