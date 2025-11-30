// Copyright 2021 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.line.
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