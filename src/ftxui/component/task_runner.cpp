// Copyright 2024 Arthur Sonzogni. All rights reserved.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#include "ftxui/component/task_runner.hpp"

#include <cassert>
#include <thread>

namespace ftxui::task {

static thread_local TaskRunner* current_task_runner = nullptr;  // NOLINT

TaskRunner::TaskRunner() {
  assert(!previous_task_runner_);
  previous_task_runner_ = current_task_runner;
  current_task_runner = this;
}

TaskRunner::~TaskRunner() {
  current_task_runner = previous_task_runner_;
}

// static
auto TaskRunner::Current() -> TaskRunner* {
  assert(current_task_runner);
  return current_task_runner;
}

auto TaskRunner::PostTask(Task task) -> void {
  queue_.PostTask(PendingTask{std::move(task)});
}

auto TaskRunner::PostDelayedTask(Task task,
                                 std::chrono::steady_clock::duration duration)
    -> void {
  queue_.PostTask(PendingTask{std::move(task), duration});
}

/// Exécute les tâches dans la file d'attente.
auto TaskRunner::RunUntilIdle()
    -> std::optional<std::chrono::steady_clock::duration> {
  while (true) {
    auto maybe_task = queue_.Get();
    if (std::holds_alternative<std::monostate>(maybe_task)) {
      // Plus de tâches à exécuter, sortir de la boucle.
      return std::nullopt;
    }

    if (std::holds_alternative<Task>(maybe_task)) {
      executed_tasks_++;
      std::get<Task>(maybe_task)();
      continue;
    }

    if (std::holds_alternative<std::chrono::steady_clock::duration>(
            maybe_task)) {
      return std::get<std::chrono::steady_clock::duration>(maybe_task);
    }
  }
}

auto TaskRunner::Run() -> void {
  while (true) {
    auto duration = RunUntilIdle();
    if (!duration) {
      // Plus de tâches à exécuter, sortir de la boucle.
      return;
    }

    // Dormir pendant la durée jusqu'à ce que la prochaine tâche puisse être exécutée.
    std::this_thread::sleep_for(duration.value());
  }
}

}  // namespace ftxui::task