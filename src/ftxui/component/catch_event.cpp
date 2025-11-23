// Copyright 2021 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#include <functional>  // for function
#include <utility>     // for move

#include "ftxui/component/component.hpp"  // for Make, CatchEvent, ComponentDecorator
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/event.hpp"           // for Event

namespace ftxui {

class CatchEventBase : public ComponentBase {
 public:
  // Constructeur.
  explicit CatchEventBase(std::function<bool(Event)> on_event)
      : on_event_(std::move(on_event)) {}

  // Implémentation du composant.
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

/// @brief Retourne un composant, utilisant |on_event| pour intercepter les événements. Cette fonction
/// doit retourner 'true' si l'événement a été géré, 'false' sinon.
/// @param child Le composant enveloppé.
/// @param on_event La fonction gérant les événements.
/// @ingroup component
///
/// ### Example
///
/// ```cpp
/// auto screen = ScreenInteractive::TerminalOutput();
/// auto renderer = Renderer([] {
///   return text("My interface");
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

/// @brief Décore un composant, utilisant |on_event| pour intercepter les événements. Cette fonction
/// doit retourner 'true' si l'événement a été géré, 'false' sinon.
/// @param on_event La fonction gérant les événements.
/// @ingroup component
///
/// ### Example
///
/// ```cpp
/// auto screen = ScreenInteractive::TerminalOutput();
/// auto renderer = Renderer([] { return text("Hello world"); });
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