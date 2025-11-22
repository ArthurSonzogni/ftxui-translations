// Copyright 2024 Arthur Sonzogni. All rights reserved.
// 本源代码的使用受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
#ifndef TASK_HPP
#define TASK_HPP

#include <chrono>
#include <functional>
#include <optional>

namespace ftxui::task {

/// 任务表示一个工作单元。
using Task = std::function<void()>;

/// PendingTask 表示一个计划在特定时间或尽快执行的任务。
struct PendingTask {
  // 即时任务:
  PendingTask(Task t) : task(std::move(t)) {}  // NOLINT

  // 延迟任务，带有一个持续时间
  PendingTask(Task t, std::chrono::steady_clock::duration duration)
      : task(std::move(t)), time(std::chrono::steady_clock::now() + duration) {}

  /// 要执行的任务。
  Task task;

  /// 任务应该执行的时间。如果时间为空，则任务应尽快执行。
  std::optional<std::chrono::steady_clock::time_point> time;

  /// 按时间比较两个 PendingTask。
  /// 如果两个任务都没有时间，则它们被认为是相等的。
  bool operator<(const PendingTask& other) const;
};

}  // namespace ftxui::task

#endif  // TASK_HPP_
