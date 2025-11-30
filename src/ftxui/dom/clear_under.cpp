// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <memory>   // for make_shared
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"        // for Element, clear_under
#include "ftxui/dom/node.hpp"            // for Node
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/box.hpp"          // for Box
#include "ftxui/screen/screen.hpp"       // for Pixel, Screen

namespace ftxui {

namespace {
using ftxui::Screen;

class ClearUnder : public NodeDecorator {
 public:
  using NodeDecorator::NodeDecorator;

  void Render(Screen& screen) override {
    for (int y = box_.y_min; y <= box_.y_max; ++y) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        screen.PixelAt(x, y) = Pixel();
        screen.PixelAt(x, y).character = " ";  // Considerar el píxel escrito.
      }
    }
    Node::Render(screen);
  }
};
}  // namespace

/// @brief Antes de dibujar |child|, limpia los píxeles de abajo. Esto es útil en
///        combinación con dbox.
/// @see ftxui::dbox
/// @ingroup dom
Element clear_under(Element element) {
  return std::make_shared<ClearUnder>(std::move(element));
}

}  // namespace ftxui