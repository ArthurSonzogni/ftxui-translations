// Copyright 2024 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
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
  // Intenta ejecutar una tarea inmediatamente.
  if (!immediate_tasks_.empty()) {
    auto task = immediate_tasks_.front();
    immediate_tasks_.pop();
    return task.task;
  }

  // Mueve todas las tareas que se pueden ejecutar a la cola inmediata.
  auto now = std::chrono::steady_clock::now();
  while (!delayed_tasks_.empty() && delayed_tasks_.top().time.value() <= now) {
    immediate_tasks_.push(delayed_tasks_.top());
    delayed_tasks_.pop();
  }

  // Intenta ejecutar una tarea inmediatamente.
  if (!immediate_tasks_.empty()) {
    auto task = immediate_tasks_.front();
    immediate_tasks_.pop();
    return task.task;
  }

  // Si no hay tareas para ejecutar, devuelve el retraso hasta la próxima tarea.
  if (!delayed_tasks_.empty()) {
    return delayed_tasks_.top().time.value() - now;
  }

  // Si no hay tareas para ejecutar, devuelve la duración máxima.
  return std::monostate{};
}

}  // namespace ftxui::task