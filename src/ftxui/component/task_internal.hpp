// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef TASK_HPP
#define TASK_HPP

#include <chrono>
#include <functional>
#include <optional>

namespace ftxui::task {

/// A task represents a unit of work.
using Task = std::function<void()>;

/// PendingTask 代表一個預計在特定時間或盡快執行的任務。
struct PendingTask {
  // Immediate task:
  PendingTask(Task t) : task(std::move(t)) {}  // NOLINT

  // Delayed task with a duration
  PendingTask(Task t, std::chrono::steady_clock::duration duration)
      : task(std::move(t)), time(std::chrono::steady_clock::now() + duration) {}

  /// The task to be executed.
  Task task;

  /// The time when the task should be executed. If the time is empty, the task
  /// should be executed as soon as possible.
  std::optional<std::chrono::steady_clock::time_point> time;

  /// 根據時間比較兩個 PendingTask。
  /// 如果兩個任務都沒有時間，則視為相等。
  bool operator<(const PendingTask& other) const;
};

}  // namespace ftxui::task

#endif  // TASK_HPP_
