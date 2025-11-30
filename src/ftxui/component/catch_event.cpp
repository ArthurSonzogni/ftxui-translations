// Copyright 2021 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <functional>  // for function
#include <utility>     // for move

#include "ftxui/component/component.hpp"  // for Make, CatchEvent, ComponentDecorator
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/event.hpp"           // for Event

namespace ftxui {

class CatchEventBase : public ComponentBase {
 public:
  // Constructor.
  explicit CatchEventBase(std::function<bool(Event)> on_event)
      : on_event_(std::move(on_event)) {}

  // Implementación del componente.
  bool OnEvent(Event event) override {
    if (on_event_(event)) {
      return true;
    } else {
      return ComponentBase::OnEvent(event);
    }
  }

 protected:
  std::function<bool(Event)> on_event_;
};

/// @brief Retorna un componente, usando |on_event| para capturar eventos. Esta función
/// debe retornar true cuando el evento ha sido manejado, false en caso contrario.
/// @param child El componente envuelto.
/// @param on_event La función que maneja el evento.
/// @ingroup component
///
/// ### Ejemplo
///
/// ```cpp
/// auto screen = ScreenInteractive::TerminalOutput();
/// auto renderer = Renderer([] {
///   return text("Mi interfaz");
/// });
/// auto component = CatchEvent(renderer, [&](Event event) {
///   if (event == Event::Character('q')) {
///     screen.ExitLoopClosure()();
///     return true;
///   }
///   return false;
/// });
/// screen.Loop(component);
/// ```
Component CatchEvent(Component child,
                     std::function<bool(Event event)> on_event) {
  auto out = Make<CatchEventBase>(std::move(on_event));
  out->Add(std::move(child));
  return out;
}

/// @brief Decora un componente, usando |on_event| para capturar eventos. Esta función
/// debe retornar true cuando el evento ha sido manejado, false en caso contrario.
/// @param on_event La función que maneja el evento.
/// @ingroup component
///
/// ### Ejemplo
///
/// ```cpp
/// auto screen = ScreenInteractive::TerminalOutput();
/// auto renderer = Renderer([] { return text("Hola mundo"); });
/// renderer |= CatchEvent([&](Event event) {
///   if (event == Event::Character('q')) {
///     screen.ExitLoopClosure()();
///     return true;
///   }
///   return false;
/// });
/// screen.Loop(renderer);
/// ```
ComponentDecorator CatchEvent(std::function<bool(Event)> on_event) {
  return [on_event = std::move(on_event)](Component child) {
    return CatchEvent(std::move(child), [on_event = on_event](Event event) {
      return on_event(std::move(event));
    });
  };
}

}  // namespace ftxui
