// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに準拠しています。
#ifndef FTXUI_UTIL_AUTORESET_HPP
#define FTXUI_UTIL_AUTORESET_HPP

#include <utility>

namespace ftxui {

/// 変数に値を割り当て、スコープを抜けるときに古い値をリセットします。
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

#endif /* end of include guard: FTXUI_UTIL_AUTORESET_HPP */
