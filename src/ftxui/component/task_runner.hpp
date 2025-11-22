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

  // 返回当前线程的任务运行器。
  static auto Current() -> TaskRunner*;

  /// 安排一个任务立即执行。
  auto PostTask(Task task) -> void;

  /// 安排一个任务在指定持续时间后执行。
  auto PostDelayedTask(Task task,
                       std::chrono::steady_clock::duration duration) -> void;

  /// 运行队列中的任务，返回下一个延迟任务可以执行的延迟时间。
  auto RunUntilIdle() -> std::optional<std::chrono::steady_clock::duration>;

  // 运行队列中的任务，阻塞直到所有任务执行完毕。
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
