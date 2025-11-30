// Copyright 2021 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <functional>  // for function
#include <memory>  // for make_unique, __shared_ptr_access, __shared_ptr_access<>::element_type, shared_ptr
#include <utility>  // for move

#include "ftxui/component/component.hpp"  // for ComponentDecorator, Maybe, Make
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/dom/elements.hpp"              // for Element
#include "ftxui/dom/node.hpp"                  // for Node

namespace ftxui {

/// @brief Decora un componente |child|. Se muestra solo cuando |show| devuelve
/// verdadero.
/// @param child el componente a decorar.
/// @param show una función que devuelve si |child| debe mostrarse.
/// @ingroup component
Component Maybe(Component child, std::function<bool()> show) {
  class Impl : public ComponentBase {
   public:
    explicit Impl(std::function<bool()> show) : show_(std::move(show)) {}

   private:
    Element OnRender() override {
      return show_() ? ComponentBase::OnRender() : std::make_unique<Node>();
    }
    bool Focusable() const override {
      return show_() && ComponentBase::Focusable();
    }
    bool OnEvent(Event event) override {
      return show_() && ComponentBase::OnEvent(event);
    }

    std::function<bool()> show_;
  };

  auto maybe = Make<Impl>(std::move(show));
  maybe->Add(std::move(child));
  return maybe;
}

/// @brief Decora un componente. Se muestra solo cuando la función |show|
/// devuelve verdadero.
/// @param show una función que devuelve si el componente decorado debe
/// mostrarse.
/// @ingroup component
///
/// ### Ejemplo
///
/// ```cpp
/// auto component = Renderer([]{ return text("Hello World!"); });
/// auto maybe_component = component | Maybe([&]{ return counter == 42; });
/// ```
ComponentDecorator Maybe(std::function<bool()> show) {
  return [show = std::move(show)](Component child) mutable {
    return Maybe(std::move(child), std::move(show));
  };
}

/// @brief Decora un componente |child|. Se muestra solo cuando |show| es verdadero.
/// @param child el componente a decorar.
/// @param show un booleano. |child| se muestra cuando |show| es verdadero.
/// @ingroup component
///
/// ### Ejemplo
///
/// ```cpp
/// auto component = Renderer([]{ return text("Hello World!"); });
/// auto maybe_component = Maybe(component, &show);
/// ```
Component Maybe(Component child, const bool* show) {
  return Maybe(std::move(child), [show] { return *show; });
}

/// @brief Decora un componente. Se muestra solo cuando |show| es verdadero.
/// @param show un booleano. |child| se muestra cuando |show| es verdadero.
/// @ingroup component
///
/// ### Ejemplo
///
/// ```cpp
/// auto component = Renderer([]{ return text("Hello World!"); });
/// auto maybe_component = component | Maybe(&show);
/// ```
ComponentDecorator Maybe(const bool* show) {
  return [show](Component child) { return Maybe(std::move(child), show); };
}

}  // namespace ftxui
