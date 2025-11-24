// 版權所有 2024 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱
// LICENSE 檔案。
#include "ftxui/component/task_runner.hpp"

#include <cassert>
#include <thread>

namespace ftxui::task {

static thread_local TaskRunner* current_task_runner = nullptr;  // NOLINT

TaskRunner::TaskRunner() {
  assert(!previous_task_runner_);
  previous_task_runner_ = current_task_runner;
  current_task_runner = this;
}

TaskRunner::~TaskRunner() {
  current_task_runner = previous_task_runner_;
}

// static
auto TaskRunner::Current() -> TaskRunner* {
  assert(current_task_runner);
  return current_task_runner;
}

auto TaskRunner::PostTask(Task task) -> void {
  queue_.PostTask(PendingTask{std::move(task)});
}

auto TaskRunner::PostDelayedTask(Task task,
                                 std::chrono::steady_clock::duration duration)
    -> void {
  queue_.PostTask(PendingTask{std::move(task), duration});
}

/// 執行佇列中的任務。
auto TaskRunner::RunUntilIdle()
    -> std::optional<std::chrono::steady_clock::duration> {
  while (true) {
    auto maybe_task = queue_.Get();
    if (std::holds_alternative<std::monostate>(maybe_task)) {
      // 沒有更多任務可執行，退出迴圈。
      return std::nullopt;
    }

    if (std::holds_alternative<Task>(maybe_task)) {
      executed_tasks_++;
      std::get<Task>(maybe_task)();
      continue;
    }

    if (std::holds_alternative<std::chrono::steady_clock::duration>(
            maybe_task)) {
      return std::get<std::chrono::steady_clock::duration>(maybe_task);
    }
  }
}

auto TaskRunner::Run() -> void {
  while (true) {
    auto duration = RunUntilIdle();
    if (!duration) {
      // 沒有更多任務可執行，退出迴圈。
      return;
    }

    // 休眠直到下一個任務可以執行。
    std::this_thread::sleep_for(duration.value());
  }
}

}  // namespace ftxui::task
