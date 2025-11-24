// Copyright 2024 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權約束，該授權可在 LICENSE 檔案中找到。
#ifndef TASK_HPP
#define TASK_HPP

#include <chrono>
#include <functional>
#include <optional>

namespace ftxui::task {

/// 任務代表一個工作單元。
using Task = std::function<void()>;

/// PendingTask 代表一個預計在特定時間或盡快執行的任務。
struct PendingTask {
  // 立即任務：
  PendingTask(Task t) : task(std::move(t)) {}  // NOLINT

  // 帶有持續時間的延遲任務
  PendingTask(Task t, std::chrono::steady_clock::duration duration)
      : task(std::move(t)), time(std::chrono::steady_clock::now() + duration) {}

  /// 要執行的任務。
  Task task;

  /// 任務應該執行的時間。如果時間為空，則任務應盡快執行。
  std::optional<std::chrono::steady_clock::time_point> time;

  /// 根據時間比較兩個 PendingTask。
  /// 如果兩個任務都沒有時間，則視為相等。
  bool operator<(const PendingTask& other) const;
};

}  // namespace ftxui::task

#endif  // TASK_HPP_