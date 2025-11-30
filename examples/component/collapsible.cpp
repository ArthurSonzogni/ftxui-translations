// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <memory>   // for allocator, make_shared, __shared_ptr_access
#include <utility>  // for move
#include <vector>   // for vector

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Collapsible, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"                  // for text, hbox, Element

using namespace ftxui;

// Toma una lista de componentes, los muestra verticalmente, una columna desplazada a la
// derecha.
Component Inner(std::vector<Component> children) {
  Component vlist = Container::Vertical(std::move(children));
  return Renderer(vlist, [vlist] {
    return hbox({
        text(" "),
        vlist->Render(),
    });
  });
}

Component Empty() {
  return std::make_shared<ComponentBase>();
}

int main() {
  auto component =
      Collapsible("Collapsible 1",
                  Inner({
                      Collapsible("Collapsible 1.1",
                                  Inner({
                                      Collapsible("Collapsible 1.1.1", Empty()),
                                      Collapsible("Collapsible 1.1.2", Empty()),
                                      Collapsible("Collapsible 1.1.3", Empty()),
                                  })),
                      Collapsible("Collapsible 1.2",
                                  Inner({
                                      Collapsible("Collapsible 1.2.1", Empty()),
                                      Collapsible("Collapsible 1.2.2", Empty()),
                                      Collapsible("Collapsible 1.2.3", Empty()),
                                  })),
                      Collapsible("Collapsible 1.3",
                                  Inner({
                                      Collapsible("Collapsible 1.3.1", Empty()),
                                      Collapsible("Collapsible 1.3.2", Empty()),
                                      Collapsible("Collapsible 1.3.3", Empty()),
                                  })),
                  }));

  ScreenInteractive::FitComponent().Loop(component);
}