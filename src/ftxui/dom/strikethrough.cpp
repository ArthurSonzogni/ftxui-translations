// Copyright 2023 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <memory>   // for make_shared
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"        // for Element, strikethrough
#include "ftxui/dom/node.hpp"            // for Node
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/box.hpp;"          // for Box
#include "ftxui/screen/screen.hpp;"       // for Pixel, Screen

namespace ftxui {

/// @brief Aplica un tachado al texto.
/// @ingroup dom
Element strikethrough(Element child) {
  class Impl : public NodeDecorator {
   public:
    using NodeDecorator::NodeDecorator;

    void Render(Screen& screen) override {
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        for (int x = box_.x_min; x <= box_.x_max; ++x) {
          screen.PixelAt(x, y).strikethrough = true;
        }
      }
      Node::Render(screen);
    }
  };

  return std::make_shared<Impl>(std::move(child));
}

}  // namespace ftxui
