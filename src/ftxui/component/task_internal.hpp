// Copyright 2024 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに準拠します。
#ifndef TASK_HPP
#define TASK_HPP

#include <chrono>
#include <functional>
#include <optional>

namespace ftxui::task {

/// タスクは作業の単位を表します。
using Task = std::function<void()>;

/// PendingTaskは、特定の時刻に、または可能な限り早く実行されるようにスケジュールされたタスクを表します。
struct PendingTask {
  // 即時タスク:
  PendingTask(Task t) : task(std::move(t)) {}  // NOLINT

  // 期間を伴う遅延タスク
  PendingTask(Task t, std::chrono::steady_clock::duration duration)
      : task(std::move(t)), time(std::chrono::steady_clock::now() + duration) {}

  /// 実行されるタスク。
  Task task;

  /// タスクが実行されるべき時刻。時刻が空の場合、タスクは可能な限り早く実行されるべきです。
  std::optional<std::chrono::steady_clock::time_point> time;

  /// 2つのPendingTaskを時刻で比較します。
  /// 両方のタスクに時刻がない場合、それらは等しいと見なされます。
  bool operator<(const PendingTask& other) const;
};

}  // namespace ftxui::task

#endif  // TASK_HPP_