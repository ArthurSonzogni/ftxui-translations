// 版權所有 2024 Arthur Sonzogni。保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，該條款可在
// LICENSE 檔案中找到。
#ifndef TASK_RUNNER_HPP
#define TASK_RUNNER_HPP

#include "ftxui/component/task_internal.hpp"
#include "ftxui/component/task_queue.hpp"

namespace ftxui::task {

class TaskRunner {
 public:
  TaskRunner();
  ~TaskRunner();

  // 返回當前線程的任務執行器。
  static auto Current() -> TaskRunner*;

  /// 安排一個任務立即執行。
  auto PostTask(Task task) -> void;

  /// 安排一個任務在特定延遲後執行。
  auto PostDelayedTask(Task task,
                       std::chrono::steady_clock::duration duration) -> void;

  /// 運行佇列中的任務，返回下一個延遲任務可以執行的延遲時間。
  auto RunUntilIdle() -> std::optional<std::chrono::steady_clock::duration>;

  // 運行佇列中的任務，阻塞直到所有任務執行完畢。
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