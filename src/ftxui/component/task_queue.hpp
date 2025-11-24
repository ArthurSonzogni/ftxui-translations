// Copyright 2024 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに準拠します。
#ifndef TASK_QUEUE_HPP
#define TASK_QUEUE_HPP

#include <queue>
#include <variant>

#include "ftxui/component/task_internal.hpp"  // for PendingTask, Task

namespace ftxui::task {

/// 将来の実行のためにタスクをスケジュールするタスクキュー。タスクは、即座に、または特定の期間後に実行されるようにスケジュールできます。
/// - タスクはスケジュールされた順序で実行されます。
/// - 複数のタスクが同時に実行されるようにスケジュールされている場合、それらはスケジュールされた順序で実行されます。
/// - タスクが過去に実行されるようにスケジュールされている場合、それは即座に実行されます。
struct TaskQueue {
  auto PostTask(PendingTask task) -> void;

  using MaybeTask =
      std::variant<Task, std::chrono::steady_clock::duration, std::monostate>;
  auto Get() -> MaybeTask;

  bool HasImmediateTasks() const { return !immediate_tasks_.empty(); }

 private:
  std::queue<PendingTask> immediate_tasks_;
  std::priority_queue<PendingTask> delayed_tasks_;
};

}  // namespace ftxui::task

#endif
