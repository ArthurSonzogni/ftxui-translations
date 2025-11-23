// Copyright 2024 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#ifndef TASK_QUEUE_HPP
#define TASK_QUEUE_HPP

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
