// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Element, filler, operator|, hbox, flex_grow, vbox, xflex_grow, yflex_grow, align_right, center, hcenter, vcenter

namespace ftxui {

/// @brief Centre un élément horizontalement.
/// @param child L'élément décoré.
/// @return L'élément centré.
/// @ingroup dom
Element hcenter(Element child) {
  return hbox(filler(), std::move(child), filler());
}

/// @brief Centre un élément verticalement.
/// @param child L'élément décoré.
/// @return L'élément centré.
/// @ingroup dom
Element vcenter(Element child) {
  return vbox(filler(), std::move(child), filler());
}

/// @brief Centre un élément horizontalement et verticalement.
/// @param child L'élément décoré.
/// @return L'élément centré.
/// @ingroup dom
Element center(Element child) {
  return hcenter(vcenter(std::move(child)));
}

/// @brief Aligne un élément sur le côté droit.
/// @param child L'élément décoré.
/// @return L'élément aligné à droite.
/// @ingroup dom
Element align_right(Element child) {
  return hbox(filler(), std::move(child));
}

}  // namespace ftxui
