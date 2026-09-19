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

  // 回傳目前執行緒的 task runner。
  static auto Current() -> TaskRunner*;

  /// 排程一個立即執行的 task。
  auto PostTask(Task task) -> void;

  /// 排程一個在一段時間後執行的 task。
  auto PostDelayedTask(Task task, std::chrono::steady_clock::duration duration)
      -> void;

  /// 執行佇列中的 task，回傳延遲到下一個延遲 task
  /// 可執行的時間。
  auto RunUntilIdle() -> std::optional<std::chrono::steady_clock::duration>;

  // 執行佇列中的 task，阻塞直到所有 task 都執行完畢。
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
