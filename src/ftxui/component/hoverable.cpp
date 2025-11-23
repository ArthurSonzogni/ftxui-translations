// Copyright 2022 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <functional>  // for function
#include <utility>     // for move

#include "ftxui/component/component.hpp"  // for ComponentDecorator, Hoverable, Make
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/component/mouse.hpp"           // for Mouse
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, reflect, Element
#include "ftxui/screen/box.hpp"    // for Box

namespace ftxui {

namespace {

void Post(std::function<void()> f) {
  if (auto* screen = ScreenInteractive::Active()) {
    screen->Post(std::move(f));
    return;
  }
  f();
}

}  // namespace

/// @brief Encapsule un composant. Donne la possibilité de savoir s'il est survolé par la
/// souris.
/// @param component Le composant encapsulé.
/// @param hover La valeur pour refléter si le composant est survolé ou non.
/// @ingroup component
///
/// ### Exemple
///
/// ```cpp
/// auto button = Button("exit", screen.ExitLoopClosure());
/// bool hover = false;
/// auto button_hover = Hoverable(button, &hover);
/// ```
// NOLINTNEXTLINE
Component Hoverable(Component component, bool* hover) {
  class Impl : public ComponentBase {
   public:
    Impl(Component component, bool* hover)
        : component_(std::move(component)), hover_(hover) {
      Add(component_);
    }

   private:
    Element OnRender() override {
      return ComponentBase::OnRender() | reflect(box_);
    }

    bool OnEvent(Event event) override {
      if (event.is_mouse()) {
        *hover_ = box_.Contain(event.mouse().x, event.mouse().y) &&
                  CaptureMouse(event);
      }

      return ComponentBase::OnEvent(event);
    }

    Component component_;
    bool* hover_;
    Box box_;
  };

  return Make<Impl>(component, hover);
}

/// @brief Encapsule un composant. Utilise des callbacks.
/// @param component Le composant encapsulé.
/// @param on_enter Callback OnEnter
/// @param on_leave Callback OnLeave
/// @ingroup component
///
/// ### Exemple
///
/// ```cpp
/// auto button = Button("exit", screen.ExitLoopClosure());
/// bool hover = false;
/// auto button_hover = Hoverable(button, &hover);
/// ```
Component Hoverable(Component component,
                    std::function<void()> on_enter,
                    std::function<void()> on_leave) {
  class Impl : public ComponentBase {
   public:
    Impl(Component component,
         std::function<void()> on_enter,
         std::function<void()> on_leave)
        : component_(std::move(component)),
          on_enter_(std::move(on_enter)),
          on_leave_(std::move(on_leave)) {
      Add(component_);
    }

   private:
    Element OnRender() override {
      return ComponentBase::OnRender() | reflect(box_);
    }

    bool OnEvent(Event event) override {
      if (event.is_mouse()) {
        const bool hover = box_.Contain(event.mouse().x, event.mouse().y) &&
                           CaptureMouse(event);
        if (hover != hover_) {
          Post(hover ? on_enter_ : on_leave_);
        }
        hover_ = hover;
      }

      return ComponentBase::OnEvent(event);
    }

    Component component_;
    Box box_;
    bool hover_ = false;
    std::function<void()> on_enter_;
    std::function<void()> on_leave_;
  };

  return Make<Impl>(std::move(component), std::move(on_enter),
                    std::move(on_leave));
}

/// @brief Encapsule un composant. Donne la possibilité de savoir s'il est survolé par la
/// souris.
/// @param hover La valeur pour refléter si le composant est survolé ou non.
/// @ingroup component
///
/// ### Exemple
///
/// ```cpp
/// bool hover = false;
/// auto button = Button("exit", screen.ExitLoopClosure());
/// button |= Hoverable(&hover);
/// ```
ComponentDecorator Hoverable(bool* hover) {
  return [hover](Component component) {
    return Hoverable(std::move(component), hover);
  };
}

/// @brief Encapsule un composant. Donne la possibilité de savoir s'il est survolé par la
/// souris.
/// @param on_enter est appelé lorsque la souris survole le composant.
/// @param on_leave est appelé lorsque la souris quitte le composant.
/// @ingroup component
///
/// ### Exemple
///
/// ```cpp
/// auto button = Button("exit", screen.ExitLoopClosure());
/// int on_enter_cnt = 0;
/// int on_leave_cnt = 0;
/// button |= Hoverable(
///   [&]{ on_enter_cnt++; },
///   [&]{ on_leave_cnt++; }
/// );
/// ```
// NOLINTNEXTLINE
ComponentDecorator Hoverable(std::function<void()> on_enter,
                             // NOLINTNEXTLINE
                             std::function<void()> on_leave) {
  return [on_enter, on_leave](Component component) {
    return Hoverable(std::move(component), on_enter, on_leave);
  };
}

/// @brief Encapsule un composant. Donne la possibilité de savoir s'il est survolé par la
/// souris.
/// @param component le composant encapsulé.
/// @param on_change est appelé lorsque la souris entre ou quitte le composant.
/// @ingroup component
///
/// ### Exemple
///
/// ```cpp
/// auto button = Button("exit", screen.ExitLoopClosure());
/// bool hovered = false;
/// auto button_hoverable = Hoverable(button,
///                                   [&](bool hover) { hovered = hover;});
/// ```
// NOLINTNEXTLINE
Component Hoverable(Component component, std::function<void(bool)> on_change) {
  return Hoverable(
      std::move(component),              //
      [on_change] { on_change(true); },  //
      [on_change] { on_change(false); }  //
  );
}

/// @brief Encapsule un composant. Donne la possibilité de savoir s'il est survolé par la
/// souris.
/// @param on_change est appelé lorsque la souris entre ou quitte le composant.
/// @ingroup component
///
/// ### Exemple
///
/// ```cpp
/// auto button = Button("exit", screen.ExitLoopClosure());
/// bool hovered = false;
/// button |= Hoverable([&](bool hover) { hovered = hover;});
/// ```
// NOLINTNEXTLINE
ComponentDecorator Hoverable(std::function<void(bool)> on_change) {
  return [on_change](Component component) {
    return Hoverable(std::move(component), on_change);
  };
}

}  // namespace ftxui