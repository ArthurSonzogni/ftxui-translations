// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Utilisation de ce code source soumise à la licence MIT, que l'on peut trouver dans
// le fichier LICENSE.
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
  // Tenter d'exécuter une tâche immédiatement.
  if (!immediate_tasks_.empty()) {
    auto task = immediate_tasks_.front();
    immediate_tasks_.pop();
    return task.task;
  }

  // Déplacer toutes les tâches qui peuvent être exécutées vers la file d'attente immédiate.
  auto now = std::chrono::steady_clock::now();
  while (!delayed_tasks_.empty() && delayed_tasks_.top().time.value() <= now) {
    immediate_tasks_.push(delayed_tasks_.top());
    delayed_tasks_.pop();
  }

  // Tenter d'exécuter une tâche immédiatement.
  if (!immediate_tasks_.empty()) {
    auto task = immediate_tasks_.front();
    immediate_tasks_.pop();
    return task.task;
  }

  // S'il n'y a pas de tâches à exécuter, retourner le délai avant la prochaine tâche.
  if (!delayed_tasks_.empty()) {
    return delayed_tasks_.top().time.value() - now;
  }

  // S'il n'y a pas de tâches à exécuter, retourner la durée maximale.
  return std::monostate{};
}

}  // namespace ftxui::task