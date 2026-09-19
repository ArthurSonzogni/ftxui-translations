// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.line.
#ifndef FTXUI_DOM_BOX_HELPER_HPP
#define FTXUI_DOM_BOX_HELPER_HPP

#include <vector>
#include "ftxui/dom/requirement.hpp"

namespace ftxui::box_helper {

struct Element {
  // Entrada:
  int min_size = 0;
  int flex_grow = 0;
  int flex_shrink = 0;

  // Salida;
  int size = 0;
};

void Compute(std::vector<Element>* elements, int target_size);
}  // namespace ftxui::box_helper

#endif /* fin de la guarda de inclusión: FTXUI_DOM_BOX_HELPER_HPP */
