// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef TASK_QUEUE_HPP
#define TASK_QUEUE_HPP

#include <mutex>
#include <queue>
#include <variant>

#include "ftxui/component/task_internal.hpp"  // for PendingTask, Task

namespace ftxui::task {

/// 将来実行されるタスクをスケジュールするタスクキュー。タスクは
/// 即座に、または一定期間後に実行されるようスケジュールできます。
/// - タスクはスケジュールされた順序で実行されます。
/// - 複数のタスクが同じ時刻に実行されるようスケジュールされている場合、
///   スケジュールされた順序で実行されます。
/// - タスクが過去に実行されるようスケジュールされている場合、
///   直ちに実行されます。
struct TaskQueue {
  using MaybeTask =
      std::variant<Task, std::chrono::steady_clock::duration, std::monostate>;

  auto Get() -> MaybeTask;
  auto HasImmediateTasks() const -> bool;
  auto PostTask(PendingTask task) -> void;

 private:
  mutable std::mutex mutex_;
  std::queue<PendingTask> immediate_tasks_;
  std::priority_queue<PendingTask> delayed_tasks_;
};

}  // namespace ftxui::task

#endif
