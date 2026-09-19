// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef TASK_RUNNER_HPP
#define TASK_RUNNER_HPP

#include "ftxui/component/task_internal.hpp"
#include "ftxui/component/task_queue.hpp"

namespace ftxui::task {

class TaskRunner {
 public:
  TaskRunner();
  ~TaskRunner();

  // Devuelve el ejecutor de tareas para el hilo actual.
  static auto Current() -> TaskRunner*;

  /// Programa una tarea para ser ejecutada inmediatamente.
  auto PostTask(Task task) -> void;

  /// Programa una tarea para ser ejecutada después de una cierta duración.
  auto PostDelayedTask(Task task, std::chrono::steady_clock::duration duration)
      -> void;

  /// Ejecuta las tareas en la cola, devuelve el retraso hasta que la siguiente tarea
  /// retrasada pueda ser ejecutada.
  auto RunUntilIdle() -> std::optional<std::chrono::steady_clock::duration>;

  // Ejecuta las tareas en la cola, bloqueando hasta que todas las tareas sean ejecutadas.
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
