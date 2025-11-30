// Copyright 2024 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef TASK_HPP
#define TASK_HPP

#include <chrono>
#include <functional>
#include <optional>

namespace ftxui::task {

/// Una tarea representa una unidad de trabajo.
using Task = std::function<void()>;

/// Una PendingTask representa una tarea programada para ser ejecutada en un
/// momento específico, o tan pronto como sea posible.
struct PendingTask {
  // Tarea inmediata:
  PendingTask(Task t) : task(std::move(t)) {}  // NOLINT

  // Tarea retrasada con una duración
  PendingTask(Task t, std::chrono::steady_clock::duration duration)
      : task(std::move(t)), time(std::chrono::steady_clock::now() + duration) {}

  /// La tarea a ejecutar.
  Task task;

  /// El momento en que la tarea debe ser ejecutada. Si el momento está vacío, la tarea
  /// debe ser ejecutada tan pronto como sea posible.
  std::optional<std::chrono::steady_clock::time_point> time;

  /// Compara dos PendingTask por su tiempo.
  /// Si ambas tareas no tienen tiempo, se consideran iguales.
  bool operator<(const PendingTask& other) const;
};

}  // namespace ftxui::task

#endif  // TASK_HPP_