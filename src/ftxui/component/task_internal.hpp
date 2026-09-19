// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef TASK_HPP
#define TASK_HPP

#include <chrono>
#include <functional>
#include <optional>

namespace ftxui::task {

/// 一個 task 代表一個工作單位。
using Task = std::function<void()>;

/// PendingTask 代表一個預計在特定時間或盡快執行的任務。
struct PendingTask {
  // 立即執行的 task：
  PendingTask(Task t) : task(std::move(t)) {}  // NOLINT

  // 延遲一段時間執行的 task
  PendingTask(Task t, std::chrono::steady_clock::duration duration)
      : task(std::move(t)), time(std::chrono::steady_clock::now() + duration) {}

  /// 要執行的 task。
  Task task;

  /// task 應該被執行的時間。如果時間為空，則該 task
  /// 應該盡快執行。
  std::optional<std::chrono::steady_clock::time_point> time;

  /// 根據時間比較兩個 PendingTask。
  /// 如果兩個任務都沒有時間，則視為相等。
  bool operator<(const PendingTask& other) const;
};

}  // namespace ftxui::task

#endif  // TASK_HPP_
