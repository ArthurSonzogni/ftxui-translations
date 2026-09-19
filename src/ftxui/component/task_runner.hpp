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

  // 現在のスレッドのタスクランナーを返す。
  static auto Current() -> TaskRunner*;

  /// タスクを即座に実行するようスケジュールします。
  auto PostTask(Task task) -> void;

  /// タスクを一定期間後に実行するようスケジュールします。
  auto PostDelayedTask(Task task, std::chrono::steady_clock::duration duration)
      -> void;

  /// キュー内のタスクを実行し、次の遅延タスクが実行可能になるまでの
  /// 遅延を返します。
  auto RunUntilIdle() -> std::optional<std::chrono::steady_clock::duration>;

  // キュー内のタスクを実行し、すべてのタスクが実行されるまでブロックする。
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
