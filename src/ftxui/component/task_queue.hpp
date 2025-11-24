// Copyright 2024 Arthur Sonzogni. All rights reserved.
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱 LICENSE 檔案。
#ifndef TASK_QUEUE_HPP
#define TASK_QUEUE_HPP

#include <queue>
#include <variant>

#include "ftxui/component/task_internal.hpp"  // for PendingTask, Task

namespace ftxui::task {

/// 任務佇列，用於排定未來執行的任務。任務可以排定立即執行，或在指定持續時間後執行。
/// - 任務會依排定的順序執行。
/// - 如果多個任務排定在同一時間執行，它們會依排定的順序執行。
/// - 如果任務排定在過去執行，它會立即執行。
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