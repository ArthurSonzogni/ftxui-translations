// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_UTIL_HPP
#define FTXUI_SCREEN_UTIL_HPP

namespace ftxui::util {

// 類似於 std::clamp，但允許 hi 小於 lo。
template <class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
  return v < lo ? lo : hi < v ? hi : v;
}

}  // namespace ftxui::util

#endif /* include guard 結束: FTXUI_SCREEN_UTIL_HPP */