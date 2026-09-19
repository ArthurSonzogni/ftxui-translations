// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_UTIL_HPP
#define FTXUI_SCREEN_UTIL_HPP

#include <cstdlib>  // for getenv

namespace ftxui::util {

// 類似於 std::clamp，但允許 hi 小於 lo。
template <class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
  return v < lo ? lo : hi < v ? hi : v;
}

// 傳回環境變數 |name| 的值，若未設定則傳回 ""。
inline const char* GetEnv(const char* name) {
  // Microsoft CRT 將 std::getenv 標記為已棄用。
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
  const char* value = std::getenv(name);  // NOLINT
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif
  return value ? value : "";
}

}  // namespace ftxui::util

#endif /* include guard 結束: FTXUI_SCREEN_UTIL_HPP */
