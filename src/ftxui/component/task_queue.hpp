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

/// Une file d'attente de tâches qui planifie les tâches à exécuter ultérieurement. Les tâches peuvent être
/// planifiées pour être exécutées immédiatement, ou après une certaine durée.
/// - Les tâches sont exécutées dans l'ordre où elles ont été planifiées.
/// - Si plusieurs tâches sont planifiées pour être exécutées en même temps, elles sont
///   exécutées dans l'ordre où elles ont été planifiées.
/// - Si une tâche est planifiée pour être exécutée dans le passé, elle est exécutée
///   immédiatement.
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
