// Copyright 2024 Arthur Sonzogni. 保留所有权利。
// 此源代码的使用受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
#include "ftxui/component/task_internal.hpp"

namespace ftxui::task {
bool PendingTask::operator<(const PendingTask& other) const {
  if (!time && !other.time) {
    return false;
  }
  if (!time) {
    return true;
  }
  if (!other.time) {
    return false;
  }
  return time.value() > other.time.value();
}
}  // namespace ftxui::task
