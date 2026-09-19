// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>  // for max
#include <memory>     // for __shared_ptr_access, shared_ptr, make_shared
#include <utility>    // for move

#include "ftxui/dom/elements.hpp"  // for Element, dbox

#include "ftxui/dom/node.hpp"         // for Node, Elements
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box

namespace ftxui {

namespace {
class DBox : public Node {
 public:
  explicit DBox(Elements children) : Node(std::move(children)) {}

  void ComputeRequirement() override {
    requirement_ = Requirement{};
    for (auto& child : children_) {
      child->ComputeRequirement();

      // Extender min_x y min_y para contener a todos los hijos
      requirement_.min_x =
          std::max(requirement_.min_x, child->requirement().min_x);
      requirement_.min_y =
          std::max(requirement_.min_y, child->requirement().min_y);
    }

    // Propagar el requisito de foco.
    // Iteramos en orden inverso porque los hijos se renderizan de primero a
    // último, lo que significa que el último hijo está encima de los demás.
    // Queremos que el hijo superior tenga prioridad para el foco.
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
      auto& child = *it;
      if (requirement_.focused.Prefer(child->requirement().focused)) {
        requirement_.focused = child->requirement().focused;
      }
    }
  }

  void SetBox(Box box) override {
    Node::SetBox(box);

    for (auto& child : children_) {
      child->SetBox(box);
    }
  }
};
}  // namespace

/// @brief Apila varios elementos uno encima de otro.
/// @param children_ Los elementos de entrada.
/// @return El elemento alineado a la derecha.
/// @ingroup dom
Element dbox(Elements children_) {
  return std::make_shared<DBox>(std::move(children_));
}

}  // namespace ftxui
