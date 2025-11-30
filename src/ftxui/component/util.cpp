// Copyright 2022 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
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
