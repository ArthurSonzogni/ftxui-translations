// Copyright 2022 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、LICENSEファイルに記載されているMITライセンスに準拠します。
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
