// Copyright 2024 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
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
