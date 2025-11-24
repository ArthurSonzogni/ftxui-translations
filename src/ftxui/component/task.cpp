// Copyright 2024 Arthur Sonzogni. 無断複写・転載を禁じます。
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに準拠します。
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