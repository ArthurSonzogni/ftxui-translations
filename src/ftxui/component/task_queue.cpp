// Copyright 2024 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに従います。
// LICENSEファイルにあるMITライセンスに従います。
#include "ftxui/component/task_queue.hpp"

namespace ftxui::task {

auto TaskQueue::PostTask(PendingTask task) -> void {
  if (!task.time) {
    immediate_tasks_.push(task);
    return;
  }

  if (task.time.value() < std::chrono::steady_clock::now()) {
    immediate_tasks_.push(task);
    return;
  }

  delayed_tasks_.push(task);
}

auto TaskQueue::Get() -> MaybeTask {
  // タスクを即座に実行しようとします。
  if (!immediate_tasks_.empty()) {
    auto task = immediate_tasks_.front();
    immediate_tasks_.pop();
    return task.task;
  }

  // 実行可能なすべてのタスクを即時キューに移動します。
  auto now = std::chrono::steady_clock::now();
  while (!delayed_tasks_.empty() && delayed_tasks_.top().time.value() <= now) {
    immediate_tasks_.push(delayed_tasks_.top());
    delayed_tasks_.pop();
  }

  // タスクを即座に実行しようとします。
  if (!immediate_tasks_.empty()) {
    auto task = immediate_tasks_.front();
    immediate_tasks_.pop();
    return task.task;
  }

  // 実行するタスクがない場合、次のタスクまでの遅延を返します。
  if (!delayed_tasks_.empty()) {
    return delayed_tasks_.top().time.value() - now;
  }

  // 実行するタスクがない場合、最大期間を返します。
  return std::monostate{};
}

}  // namespace ftxui::task