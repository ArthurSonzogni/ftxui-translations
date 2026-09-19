// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/component/loop.hpp"

#include <utility>  // for move

#include "ftxui/component/app.hpp"  // for App, Component

namespace ftxui {

/// @brief Loop 是 Component 與 App 的包裝。
/// 它用於在終端機中執行一個 Component。
/// @see Component, App.
/// @see App::Loop().
/// @see App::Exit().
/// @param[in] screen 要使用的畫面。
/// @param[in] component 要執行的元件。
// NOLINTNEXTLINE
Loop::Loop(App* screen, Component component)
    : screen_(screen), component_(std::move(component)) {
  screen_->PreMain();
}

Loop::~Loop() {
  screen_->PostMain();
}

/// @brief 迴圈是否已結束。
bool Loop::HasQuitted() {
  return screen_->HasQuitted();
}

/// @brief 執行迴圈。讓 `component` 處理所有待處理的任務/事件。
/// 如果前一個影格失效，可能會繪製一個新影格。
/// 在迴圈完成之前返回 true。
void Loop::RunOnce() {
  screen_->RunOnce(component_);
}

/// @brief 等待至少一個事件被處理並執行
/// `Loop::RunOnce()`。
void Loop::RunOnceBlocking() {
  screen_->RunOnceBlocking(component_);
}

/// 執行迴圈，阻塞目前的執行緒，直到迴圈
/// 結束為止。
void Loop::Run() {
  while (!HasQuitted()) {
    RunOnceBlocking();
  }
}

}  // namespace ftxui
