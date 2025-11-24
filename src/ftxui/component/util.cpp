// 版權所有 2022 Arthur Sonzogni。保留所有權利。
// 本原始碼的使用受 MIT 許可證的約束，該許可證可在 LICENSE 文件中找到。
#include <functional>  // for function

#include "ftxui/component/component.hpp"  // for Renderer, ComponentDecorator, ElementDecorator, operator|, operator|=
#include "ftxui/component/component_base.hpp"  // for Component

namespace ftxui {

// NOLINTNEXTLINE
Component operator|(Component component, ComponentDecorator decorator) {
  return decorator(component);  // NOLINT
}

// NOLINTNEXTLINE
Component operator|(Component component, ElementDecorator decorator) {
  return component | Renderer(decorator);  // NOLINT
}

// NOLINTNEXTLINE
Component& operator|=(Component& component, ComponentDecorator decorator) {
  component = component | decorator;  // NOLINT
  return component;
}

// NOLINTNEXTLINE
Component& operator|=(Component& component, ElementDecorator decorator) {
  component = component | decorator;  // NOLINT
  return component;
}

}  // namespace ftxui
