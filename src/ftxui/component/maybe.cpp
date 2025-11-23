// Copyright 2021 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#include <functional>  // for function
#include <memory>  // for make_unique, __shared_ptr_access, __shared_ptr_access<>::element_type, shared_ptr
#include <utility>  // for move

#include "ftxui/component/component.hpp"  // for ComponentDecorator, Maybe, Make
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/dom/elements.hpp"              // for Element
#include "ftxui/dom/node.hpp"                  // for Node

namespace ftxui {

/// @brief Décore un composant |child|. Il est affiché uniquement lorsque |show| retourne
/// vrai.
/// @param child le composant à décorer.
/// @param show une fonction retournant si |child| doit être affiché.
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

/// @brief Décore un composant. Il est affiché uniquement lorsque la fonction |show|
/// retourne vrai.
/// @param show une fonction retournant si le composant décoré doit
/// être affiché.
/// @ingroup component
///
/// ### Exemple
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

/// @brief Décore un composant |child|. Il est affiché uniquement lorsque |show| est vrai.
/// @param child le composant à décorer.
/// @param show un booléen. |child| est affiché lorsque |show| est vrai.
/// @ingroup component
///
/// ### Exemple
///
/// ```cpp
/// auto component = Renderer([]{ return text("Hello World!"); });
/// auto maybe_component = Maybe(component, &show);
/// ```
Component Maybe(Component child, const bool* show) {
  return Maybe(std::move(child), [show] { return *show; });
}

/// @brief Décore un composant. Il est affiché uniquement lorsque |show| est vrai.
/// @param show un booléen. |child| est affiché lorsque |show| est vrai.
/// @ingroup component
///
/// ### Exemple
///
/// ```cpp
/// auto component = Renderer([]{ return text("Hello World!"); });
/// auto maybe_component = component | Maybe(&show);
/// ```
ComponentDecorator Maybe(const bool* show) {
  return [show](Component child) { return Maybe(std::move(child), show); };
}

}  // namespace ftxui