// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar
// en el archivo LICENSE.
#ifndef FTXUI_DOM_NODE_DECORATOR_H_
#define FTXUI_DOM_NODE_DECORATOR_H_

#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Element, unpack
#include "ftxui/dom/node.hpp"      // for Node

namespace ftxui {
struct Box;

// Clase auxiliar.
class NodeDecorator : public Node {
 public:
  explicit NodeDecorator(Element child) : Node(unpack(std::move(child))) {}
  void ComputeRequirement() override;
  void SetBox(Box box) override;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_DOM_NODE_DECORATOR_H_ */