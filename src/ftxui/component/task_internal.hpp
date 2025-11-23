// Copyright 2024 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#ifndef TASK_HPP
#define TASK_HPP

#include <chrono>
#include <functional>
#include <optional>

namespace ftxui::task {

/// Une tâche représente une unité de travail.
using Task = std::function<void()>;

/// Une PendingTask représente une tâche qui doit être exécutée à un
/// moment précis, ou dès que possible.
struct PendingTask {
  // Tâche immédiate :
  PendingTask(Task t) : task(std::move(t)) {}  // NOLINT

  // Tâche retardée avec une durée
  PendingTask(Task t, std::chrono::steady_clock::duration duration)
      : task(std::move(t)), time(std::chrono::steady_clock::now() + duration) {}

  /// La tâche à exécuter.
  Task task;

  /// L'heure à laquelle la tâche doit être exécutée. Si l'heure est vide, la tâche
  /// doit être exécutée dès que possible.  std::optional<std::chrono::steady_clock::time_point> time;

  /// Compare deux PendingTasks par leur heure.
  /// Si les deux tâches n'ont pas d'heure, elles sont considérées comme égales.  bool operator<(const PendingTask& other) const;
};

}  // namespace ftxui::task

#endif  // TASK_HPP_
