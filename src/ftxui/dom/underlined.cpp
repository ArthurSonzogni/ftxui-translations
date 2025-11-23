// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <memory>   // pour make_shared
#include <utility>  // pour move

#include "ftxui/dom/elements.hpp"        // pour Element, underlined
#include "ftxui/dom/node.hpp"            // pour Node
#include "ftxui/dom/node_decorator.hpp"  // pour NodeDecorator
#include "ftxui/screen/box.hpp"          // pour Box
#include "ftxui/screen/screen.hpp"       // pour Pixel, Screen

namespace ftxui {

namespace {
class Underlined : public NodeDecorator {
 public:
  using NodeDecorator::NodeDecorator;

  void Render(Screen& screen) override {
    Node::Render(screen);
    for (int y = box_.y_min; y <= box_.y_max; ++y) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        screen.PixelAt(x, y).underlined = true;
      }
    }
  }
};
}  // namespace

/// @brief Souligne l'élément donné.
/// @ingroup dom
Element underlined(Element child) {
  return std::make_shared<Underlined>(std::move(child));
}

}  // namespace ftxui