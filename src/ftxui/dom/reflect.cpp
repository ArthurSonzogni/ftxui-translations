// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <memory>   // for make_shared, __shared_ptr_access
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"     // for Element, unpack, Decorator, reflect
#include "ftxui/dom/node.hpp"         // for Node, Elements
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/screen.hpp"    // for Screen

namespace ftxui {
namespace {

// Classe utilitaire.
class Reflect : public Node {
 public:
  Reflect(Element child, Box& box)
      : Node(unpack(std::move(child))), reflected_box_(box) {}

  void ComputeRequirement() final {
    Node::ComputeRequirement();
    requirement_ = children_[0]->requirement();
  }

  void SetBox(Box box) final {
    reflected_box_ = box;
    Node::SetBox(box);
    children_[0]->SetBox(box);
  }

  void Render(Screen& screen) final {
    reflected_box_ = Box::Intersection(screen.stencil, reflected_box_);
    Node::Render(screen);
  }

 private:
  Box& reflected_box_;
};
}  // namespace

Decorator reflect(Box& box) {
  return [&](Element child) -> Element {
    return std::make_shared<Reflect>(std::move(child), box);
  };
}

}  // namespace ftxui
