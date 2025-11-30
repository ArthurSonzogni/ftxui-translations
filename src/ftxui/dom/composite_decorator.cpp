// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Element, filler, operator|, hbox, flex_grow, vbox, xflex_grow, yflex_grow, align_right, center, hcenter, vcenter

namespace ftxui {

/// @brief Centra un elemento horizontalmente.
/// @param child El elemento decorado.
/// @return El elemento centrado.
/// @ingroup dom
Element hcenter(Element child) {
  return hbox(filler(), std::move(child), filler());
}

/// @brief Centra un elemento verticalmente.
/// @param child El elemento decorado.
/// @return El elemento centrado.
/// @ingroup dom
Element vcenter(Element child) {
  return vbox(filler(), std::move(child), filler());
}

/// @brief Centra un elemento horizontal y verticalmente.
/// @param child El elemento decorado.
/// @return El elemento centrado.
/// @ingroup dom
Element center(Element child) {
  return hcenter(vcenter(std::move(child)));
}

/// @brief Alinea un elemento a la derecha.
/// @param child El elemento decorado.
/// @return El elemento alineado a la derecha.
/// @ingroup dom
Element align_right(Element child) {
  return hbox(filler(), std::move(child));
}

}  // namespace ftxui
