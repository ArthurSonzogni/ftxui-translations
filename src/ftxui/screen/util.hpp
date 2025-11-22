// Copyright 2022 Arthur Sonzogni. 保留所有权利。
// 此源代码的使用受 MIT 许可的约束，MIT 许可可在
// LICENSE 文件中找到。
#ifndef FTXUI_SCREEN_UTIL_HPP
#define FTXUI_SCREEN_UTIL_HPP

namespace ftxui::util {

// 类似于 std::clamp，但允许 hi 小于 lo。
template <class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
  return v < lo ? lo : hi < v ? hi : v;
}

}  // namespace ftxui::util

#endif /* include guard 结束: FTXUI_SCREEN_UTIL_HPP */
