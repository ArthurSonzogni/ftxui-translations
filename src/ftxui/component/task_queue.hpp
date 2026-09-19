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

/// Una cola de tareas que programa tareas para ser ejecutadas en el futuro. Las tareas pueden ser
/// programadas para ser ejecutadas inmediatamente, o después de una cierta duración.
/// - Las tareas se ejecutan en el orden en que fueron programadas.
/// - Si múltiples tareas son programadas para ser ejecutadas al mismo tiempo, se ejecutan
///   en el orden en que fueron programadas.
/// - Si una tarea es programada para ser ejecutada en el pasado, se ejecuta
///   inmediatamente.
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
