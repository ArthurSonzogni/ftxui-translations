// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Utilisation de ce code source soumise à la licence MIT, que l'on peut trouver dans le fichier
// LICENSE.
#include <memory>   // for allocator, make_shared, __shared_ptr_access
#include <utility>  // for move
#include <vector>   // for vector

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Collapsible, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"                  // for text, hbox, Element

using namespace ftxui;

// Prend une liste de composants, les affiche verticalement, chaque colonne décalée vers la
// droite.
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
      Collapsible("Réductible 1",
                  Inner({
                      Collapsible("Réductible 1.1",
                                  Inner({
                                      Collapsible("Réductible 1.1.1", Empty()),
                                      Collapsible("Réductible 1.1.2", Empty()),
                                      Collapsible("Réductible 1.1.3", Empty()),
                                  })),
                      Collapsible("Réductible 1.2",
                                  Inner({
                                      Collapsible("Réductible 1.2.1", Empty()),
                                      Collapsible("Réductible 1.2.2", Empty()),
                                      Collapsible("Réductible 1.2.3", Empty()),
                                  })),
                      Collapsible("Réductible 1.3",
                                  Inner({
                                      Collapsible("Réductible 1.3.1", Empty()),
                                      Collapsible("Réductible 1.3.2", Empty()),
                                      Collapsible("Réductible 1.3.3", Empty()),
                                  })),
                  }));

  ScreenInteractive::FitComponent().Loop(component);
}