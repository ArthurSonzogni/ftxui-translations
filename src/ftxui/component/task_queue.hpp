// 版权所有 2024 Arthur Sonzogni。保留所有权利。
// 本源代码的使用受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
#ifndef TASK_QUEUE_HPP
#define TASK_QUEUE_HPP

#include <queue>
#include <variant>

#include "ftxui/component/task_internal.hpp"  // for PendingTask, Task

namespace ftxui::task {

/// 一个任务队列，用于调度未来执行的任务。任务可以安排立即执行，或在特定持续时间后执行。
/// - 任务按照调度顺序执行。
/// - 如果多个任务安排在同一时间执行，它们将按照调度顺序执行。
/// - 如果任务安排在过去执行，则立即执行。
struct TaskQueue {
  auto PostTask(PendingTask task) -> void;

  using MaybeTask =
      std::variant<Task, std::chrono::steady_clock::duration, std::monostate>;
  auto Get() -> MaybeTask;

  bool HasImmediateTasks() const { return !immediate_tasks_.empty(); }

 private:
  std::queue<PendingTask> immediate_tasks_;
  std::priority_queue<PendingTask> delayed_tasks_;
};

}  // namespace ftxui::task

#endif
