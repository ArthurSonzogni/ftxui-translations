// Copyright 2021 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#ifndef FTXUI_DOM_BOX_HELPER_HPP
#define FTXUI_DOM_BOX_HELPER_HPP

#include <vector>
#include "ftxui/dom/requirement.hpp"

namespace ftxui::box_helper {

struct Element {
  // Entrée :
  int min_size = 0;
  int flex_grow = 0;
  int flex_shrink = 0;

  // Sortie :
  int size = 0;
};

void Compute(std::vector<Element>* elements, int target_size);
}  // namespace ftxui::box_helper

#endif /* Fin de la garde d'inclusion : FTXUI_DOM_BOX_HELPER_HPP */