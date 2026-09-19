// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_UTIL_AUTORESET_HPP
#define FTXUI_UTIL_AUTORESET_HPP

#include <utility>

namespace ftxui {

/// Asigna un valor a una variable, restaurando su valor antiguo al salir del ámbito.
template <typename T>
class AutoReset {
 public:
  AutoReset(T* variable, T new_value)
      : variable_(variable), previous_value_(std::move(*variable)) {
    *variable_ = std::move(new_value);
  }
  AutoReset(const AutoReset&) = delete;
  AutoReset(AutoReset&&) = delete;
  AutoReset& operator=(const AutoReset&) = delete;
  AutoReset& operator=(AutoReset&&) = delete;
  ~AutoReset() { *variable_ = std::move(previous_value_); }

 private:
  T* variable_;
  T previous_value_;
};

}  // namespace ftxui

#endif /* fin de la guarda de inclusión: FTXUI_UTIL_AUTORESET_HPP */
