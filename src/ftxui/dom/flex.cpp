// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <memory>   // for make_shared, __shared_ptr_access
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Element, unpack, filler, flex, flex_grow, flex_shrink, notflex, xflex, xflex_grow, xflex_shrink, yflex, yflex_grow, yflex_shrink
#include "ftxui/dom/node.hpp"      // for Elements, Node
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box

namespace ftxui {

namespace {

using FlexFunction = void (*)(Requirement&);

void function_flex_grow(Requirement& r) {
  r.flex_grow_x = 1;
  r.flex_grow_y = 1;
}

void function_xflex_grow(Requirement& r) {
  r.flex_grow_x = 1;
}

void function_yflex_grow(Requirement& r) {
  r.flex_grow_y = 1;
}

void function_flex_shrink(Requirement& r) {
  r.flex_shrink_x = 1;
  r.flex_shrink_y = 1;
}

void function_xflex_shrink(Requirement& r) {
  r.flex_shrink_x = 1;
}

void function_yflex_shrink(Requirement& r) {
  r.flex_shrink_y = 1;
}

void function_flex(Requirement& r) {
  r.flex_grow_x = 1;
  r.flex_grow_y = 1;
  r.flex_shrink_x = 1;
  r.flex_shrink_y = 1;
}

void function_xflex(Requirement& r) {
  r.flex_grow_x = 1;
  r.flex_shrink_x = 1;
}

void function_yflex(Requirement& r) {
  r.flex_grow_y = 1;
  r.flex_shrink_y = 1;
}

void function_not_flex(Requirement& r) {
  r.flex_grow_x = 0;
  r.flex_grow_y = 0;
  r.flex_shrink_x = 0;
  r.flex_shrink_y = 0;
}

class Flex : public Node {
 public:
  explicit Flex(FlexFunction f) : f_(f) {}
  Flex(FlexFunction f, Element child) : Node(unpack(std::move(child))), f_(f) {}
  void ComputeRequirement() override {
    requirement_.min_x = 0;
    requirement_.min_y = 0;
    if (!children_.empty()) {
      children_[0]->ComputeRequirement();
      requirement_ = children_[0]->requirement();
    }
    f_(requirement_);
  }

  void SetBox(Box box) override {
    Node::SetBox(box);
    if (children_.empty()) {
      return;
    }
    children_[0]->SetBox(box);
  }

  FlexFunction f_;
};

}  // namespace

/// @brief Un elemento que se expandirá proporcionalmente al espacio restante en
/// un contenedor.
/// @ingroup dom
Element filler() {
  return std::make_shared<Flex>(function_flex);
}

/// @brief Hace que un elemento hijo se expanda proporcionalmente al espacio restante en un
/// contenedor.
/// @ingroup dom
///
/// #### Ejemplos:
///
/// ~~~cpp
///   hbox({
///     text("left") | border ,
///     text("middle") | border | flex,
///     text("right") | border,
///   });
/// ~~~
///
/// #### Salida:
///
/// ~~~bash
/// ┌────┐┌─────────────────────────────────────────────────────────┐┌─────┐
/// │left││middle                                                   ││right│
/// └────┘└─────────────────────────────────────────────────────────┘└─────┘
/// ~~~
Element flex(Element child) {
  return std::make_shared<Flex>(function_flex, std::move(child));
}

/// @brief Expandir/Minimizar si es posible/necesario en el eje X.
/// @ingroup dom
Element xflex(Element child) {
  return std::make_shared<Flex>(function_xflex, std::move(child));
}

/// @brief Expandir/Minimizar si es posible/necesario en el eje Y.
/// @ingroup dom
Element yflex(Element child) {
  return std::make_shared<Flex>(function_yflex, std::move(child));
}

/// @brief Expandir si es posible.
/// @ingroup dom
Element flex_grow(Element child) {
  return std::make_shared<Flex>(function_flex_grow, std::move(child));
}

/// @brief Expandir si es posible en el eje X.
/// @ingroup dom
Element xflex_grow(Element child) {
  return std::make_shared<Flex>(function_xflex_grow, std::move(child));
}

/// @brief Expandir si es posible en el eje Y.
/// @ingroup dom
Element yflex_grow(Element child) {
  return std::make_shared<Flex>(function_yflex_grow, std::move(child));
}

/// @brief Minimizar si es necesario.
/// @ingroup dom
Element flex_shrink(Element child) {
  return std::make_shared<Flex>(function_flex_shrink, std::move(child));
}

/// @brief Minimizar si es necesario en el eje X.
/// @ingroup dom
Element xflex_shrink(Element child) {
  return std::make_shared<Flex>(function_xflex_shrink, std::move(child));
}

/// @brief Minimizar si es necesario en el eje Y.
/// @ingroup dom
Element yflex_shrink(Element child) {
  return std::make_shared<Flex>(function_yflex_shrink, std::move(child));
}

/// @brief Hace que el elemento no sea flexible.
/// @ingroup dom
Element notflex(Element child) {
  return std::make_shared<Flex>(function_not_flex, std::move(child));
}

}  // namespace ftxui