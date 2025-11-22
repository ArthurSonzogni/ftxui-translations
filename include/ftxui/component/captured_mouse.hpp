// 版权所有 2020 Arthur Sonzogni。保留所有权利。
// 此源代码的使用受 MIT 许可证的约束，该许可证可在
// LICENSE 文件中找到。
#ifndef FTXUI_CAPTURED_MOUSE_HPP
#define FTXUI_CAPTURED_MOUSE_HPP

#include <memory>

namespace ftxui {

class CapturedMouseInterface {
 public:
  CapturedMouseInterface() = default;
  CapturedMouseInterface(const CapturedMouseInterface&) = default;
  CapturedMouseInterface(CapturedMouseInterface&&) = delete;
  CapturedMouseInterface& operator=(const CapturedMouseInterface&) = default;
  CapturedMouseInterface& operator=(CapturedMouseInterface&&) = delete;
  virtual ~CapturedMouseInterface() = default;
};
using CapturedMouse = std::unique_ptr<CapturedMouseInterface>;
}  // namespace ftxui

#endif /* include guard 结束: FTXUI_CAPTURED_MOUSE_HPP */
