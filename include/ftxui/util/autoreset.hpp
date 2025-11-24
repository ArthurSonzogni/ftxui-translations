// Copyright 2020 Arthur Sonzogni. All rights reserved.
// 此原始碼的使用受 MIT 授權條款約束，詳見 LICENSE 檔案。
#ifndef FTXUI_UTIL_AUTORESET_HPP
#define FTXUI_UTIL_AUTORESET_HPP

#include <utility>

namespace ftxui {

/// 將一個值賦給一個變數，當超出作用域時，將其舊值重置。
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

#endif /* 包含防護結束: FTXUI_UTIL_AUTORESET_HPP */