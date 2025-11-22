// 版权所有 2022 Arthur Sonzogni。保留所有权利。
// 本源代码的使用受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
#ifndef FTXUI_COMPONENT_ANIMATION_HPP
#define FTXUI_COMPONENT_ANIMATION_HPP

#include <functional>
#include <variant>
#include "ftxui/component/event.hpp"

namespace ftxui {
class AnimationTask {};
using Closure = std::function<void()>;
using Task = std::variant<Event, Closure, AnimationTask>;
}  // namespace ftxui

#endif  // FTXUI_COMPONENT_ANIMATION_HPP
