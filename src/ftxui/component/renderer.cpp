// Copyright 2021 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée
// dans le fichier LICENSE.
#include <functional>  // for function
#include <utility>     // for move

#include "ftxui/component/component.hpp"       // for Make, Renderer
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/component/mouse.hpp"           // for Mouse
#include "ftxui/dom/elements.hpp"  // for Element, operator|, reflect
#include "ftxui/screen/box.hpp"    // for Box

namespace ftxui {

/// @brief Renvoie un composant, utilisant |render| pour afficher son interface.
/// @param render La fonction dessinant l'interface.
/// @ingroup component
///
/// ### Exemple
///
/// ```cpp
/// auto screen = ScreenInteractive::TerminalOutput();
/// auto renderer = Renderer([] {
///   return text("My interface");
/// });
/// screen.Loop(renderer);
/// ```
Component Renderer(std::function<Element()> render) {
  class Impl : public ComponentBase {
   public:
    explicit Impl(std::function<Element()> render)
        : render_(std::move(render)) {}
    Element OnRender() override { return render_(); }
    std::function<Element()> render_;
  };

  return Make<Impl>(std::move(render));
}

/// @brief Renvoie un nouveau composant, similaire à |child|, mais utilisant |render| comme événement
/// Component::Render().
/// @param child Le composant auquel transmettre les événements.
/// @param render La fonction dessinant l'interface.
/// @ingroup component
///
/// ### Exemple
///
/// ```cpp
/// auto screen = ScreenInteractive::TerminalOutput();
/// std::string label = "Click to quit";
/// auto button = Button(&label, screen.ExitLoopClosure());
/// auto renderer = Renderer(button, [&] {
///   return hbox({
///     text("A button:"),
///     button->Render(),
///   });
/// });
/// screen.Loop(renderer);
/// ```
Component Renderer(Component child, std::function<Element()> render) {
  Component renderer = Renderer(std::move(render));
  renderer->Add(std::move(child));
  return renderer;
}

/// @brief Renvoie un composant focusable, utilisant |render| pour afficher son interface.
/// @param render La fonction dessinant l'interface, prenant un booléen indiquant
/// si le composant est focus ou non.
/// @ingroup component
///
/// ### Exemple
///
/// ```cpp
/// auto screen = ScreenInteractive::TerminalOutput();
/// auto renderer = Renderer([] (bool focused) {
///   if (focused)
///     return text("My interface") | inverted;
///   else
///     return text("My interface");
/// });
/// screen.Loop(renderer);
/// ```
Component Renderer(std::function<Element(bool)> render) {
  class Impl : public ComponentBase {
   public:
    explicit Impl(std::function<Element(bool)> render)
        : render_(std::move(render)) {}

   private:
    Element OnRender() override { return render_(Focused()) | reflect(box_); }
    bool Focusable() const override { return true; }
    bool OnEvent(Event event) override {
      if (event.is_mouse() && box_.Contain(event.mouse().x, event.mouse().y)) {
        if (!CaptureMouse(event)) {
          return false;
        }

        TakeFocus();
      }

      return false;
    }
    Box box_;

    std::function<Element(bool)> render_;
  };
  return Make<Impl>(std::move(render));
}

/// @brief Décore un composant, en décorant ce qu'il rend.
/// @param decorator la fonction modifiant l'élément qu'il rend.
/// @ingroup component
///
/// ### Exemple
///
/// ```cpp
/// auto screen = ScreenInteractive::TerminalOutput();
/// auto renderer =
///     Renderer([] { return text("Hello");)
///  | Renderer(bold)
///  | Renderer(inverted);
/// screen.Loop(renderer);
/// ```
ComponentDecorator Renderer(ElementDecorator decorator) {  // NOLINT
  return [decorator](Component component) {                // NOLINT
    return Renderer(component, [component, decorator] {
      return component->Render() | decorator;
    });
  };
}

}  // namespace ftxui
