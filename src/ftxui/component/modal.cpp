// Copyright 2022 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <ftxui/component/event.hpp>  // for Event
#include <ftxui/dom/elements.hpp>  // for operator|, Element, center, clear_under, dbox
#include <memory>                  // for __shared_ptr_access, shared_ptr
#include <utility>                 // for move

#include "ftxui/component/component.hpp"  // for Make, Tab, ComponentDecorator, Modal
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase

namespace ftxui {

// Añade una ventana |modal| encima del componente |main|. Se muestra una sobre
// la otra cuando |show_modal| es verdadero.
/// @ingroup component
// NOLINTNEXTLINE
Component Modal(Component main, Component modal, const bool* show_modal) {
  class Impl : public ComponentBase {
   public:
    explicit Impl(Component main, Component modal, const bool* show_modal)
        : main_(std::move(main)),
          modal_(std::move(modal)),
          show_modal_(show_modal) {
      Add(Container::Tab({main_, modal_}, &selector_));
    }

   private:
    Element OnRender() override {
      selector_ = *show_modal_;
      auto document = main_->Render();
      if (*show_modal_) {
        document = dbox({
            document,
            modal_->Render() | clear_under | center,
        });
      }
      return document;
    }

    bool OnEvent(Event event) override {
      selector_ = *show_modal_;
      return ComponentBase::OnEvent(event);
    }

    Component main_;
    Component modal_;
    const bool* show_modal_;
    int selector_ = *show_modal_;
  };
  return Make<Impl>(main, modal, show_modal);
}

// Decora un componente. Añade una ventana |modal| encima de él. Se muestra una sobre
// la otra cuando |show_modal| es verdadero.
/// @ingroup component
// NOLINTNEXTLINE
ComponentDecorator Modal(Component modal, const bool* show_modal) {
  return [modal, show_modal](Component main) {
    return Modal(std::move(main), modal, show_modal);
  };
}

}  // namespace ftxui
