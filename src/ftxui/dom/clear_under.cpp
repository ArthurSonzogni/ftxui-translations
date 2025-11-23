// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver
// dans le fichier LICENSE.
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
        screen.PixelAt(x, y).character = " ";  // Consider the pixel written.
      }
    }
    Node::Render(screen);
  }
};
}  // namespace

/// @brief Avant de dessiner |child|, effacez les pixels en dessous. Ceci est utile en
///        combinaison avec dbox.
/// @see ftxui::dbox
/// @ingroup dom
Element clear_under(Element element) {
  return std::make_shared<ClearUnder>(std::move(element));
}

}  // namespace ftxui
