// Copyright 2022 Arthur Sonzogni. 版权所有。
// 本源代码受 MIT 许可证约束，可在 LICENSE 文件中找到。
#include "ftxui/component/loop.hpp"

#include <utility>  // for move

#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive, Component

namespace ftxui {

/// @brief Loop 是 Component 和 ScreenInteractive 的包装器。
/// 它用于在终端中运行 Component。
/// @see Component, ScreenInteractive.
/// @see ScreenInteractive::Loop().
/// @see ScreenInteractive::ExitLoop().
/// @param[in] screen 要使用的屏幕。
/// @param[in] component 要运行的组件。
// NOLINTNEXTLINE
Loop::Loop(ScreenInteractive* screen, Component component)
    : screen_(screen), component_(std::move(component)) {
  screen_->PreMain();
}

Loop::~Loop() {
  screen_->PostMain();
}

/// @brief 循环是否已退出。
bool Loop::HasQuitted() {
  return screen_->HasQuitted();
}

/// @brief 执行循环。使 `component` 处理所有待处理的任务/事件。
/// 如果前一帧无效，可能会绘制新帧。
/// 在循环完成之前返回 true。
void Loop::RunOnce() {
  screen_->RunOnce(component_);
}

/// @brief 等待至少一个事件被处理并执行 `Loop::RunOnce()`。
void Loop::RunOnceBlocking() {
  screen_->RunOnceBlocking(component_);
}

/// 执行循环，阻塞当前线程，直到循环退出。
void Loop::Run() {
  while (!HasQuitted()) {
    RunOnceBlocking();
  }
}

}  // namespace ftxui
