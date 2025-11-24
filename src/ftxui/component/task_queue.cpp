// 版權所有 2024 Arthur Sonzogni。保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱
// LICENSE 文件。
#include "ftxui/component/task_queue.hpp"

namespace ftxui::task {

auto TaskQueue::PostTask(PendingTask task) -> void {
  if (!task.time) {
    immediate_tasks_.push(task);
    return;
  }

  if (task.time.value() < std::chrono::steady_clock::now()) {
    immediate_tasks_.push(task);
    return;
  }

  delayed_tasks_.push(task);
}

auto TaskQueue::Get() -> MaybeTask {
  // 嘗試立即執行任務。
  if (!immediate_tasks_.empty()) {
    auto task = immediate_tasks_.front();
    immediate_tasks_.pop();
    return task.task;
  }

  // 將所有可執行的任務移至即時佇列。
  auto now = std::chrono::steady_clock::now();
  while (!delayed_tasks_.empty() && delayed_tasks_.top().time.value() <= now) {
    immediate_tasks_.push(delayed_tasks_.top());
    delayed_tasks_.pop();
  }

  // 嘗試立即執行任務。
  if (!immediate_tasks_.empty()) {
    auto task = immediate_tasks_.front();
    immediate_tasks_.pop();
    return task.task;
  }

  // 如果沒有任務可執行，則返回下一個任務的延遲時間。
  if (!delayed_tasks_.empty()) {
    return delayed_tasks_.top().time.value() - now;
  }

  // 如果沒有任務可執行，則返回最大持續時間。
  return std::monostate{};
}

}  // namespace ftxui::task