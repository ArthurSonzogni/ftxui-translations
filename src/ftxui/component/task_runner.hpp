// Copyright 2024 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#ifndef TASK_RUNNER_HPP
#define TASK_RUNNER_HPP

#include "ftxui/component/task_internal.hpp"
#include "ftxui/component/task_queue.hpp"

namespace ftxui::task {

class TaskRunner {
 public:
  TaskRunner();
  ~TaskRunner();

  // Retourne le gestionnaire de tâches pour le thread actuel.
  static auto Current() -> TaskRunner*;

  /// Planifie une tâche à exécuter immédiatement.
  auto PostTask(Task task) -> void;

  /// Planifie une tâche à exécuter après une certaine durée.
  auto PostDelayedTask(Task task,
                       std::chrono::steady_clock::duration duration) -> void;

  /// Exécute les tâches dans la file d'attente, retourne le délai avant que la prochaine tâche différée
  /// puisse être exécutée.
  auto RunUntilIdle() -> std::optional<std::chrono::steady_clock::duration>;

  // Exécute les tâches dans la file d'attente, bloquant jusqu'à ce que toutes les tâches soient exécutées.
  auto Run() -> void;

  bool HasImmediateTasks() const { return queue_.HasImmediateTasks(); }

  size_t ExecutedTasks() const { return executed_tasks_; }

 private:
  TaskRunner* previous_task_runner_ = nullptr;
  TaskQueue queue_;
  size_t executed_tasks_ = 0;
};

}  // namespace ftxui::task

#endif  // TASK_RUNNER_HPP