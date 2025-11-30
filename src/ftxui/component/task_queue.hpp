// Copyright 2024 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef TASK_QUEUE_HPP
#define TASK_QUEUE_HPP

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
