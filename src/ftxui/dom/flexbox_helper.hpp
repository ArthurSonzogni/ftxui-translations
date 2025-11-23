// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#ifndef FTXUI_DOM_FLEXBOX_HELPER_HPP
#define FTXUI_DOM_FLEXBOX_HELPER_HPP

#include <vector>
#include "ftxui/dom/flexbox_config.hpp"

namespace ftxui::flexbox_helper {

// Un bloc est un rectangle dans le flexbox.
struct Block {
  // Entrée :
  int min_size_x = 0;
  int min_size_y = 0;
  int flex_grow_x = 0;
  int flex_grow_y = 0;
  int flex_shrink_x = 0;
  int flex_shrink_y = 0;

  // Sortie :
  int line{};
  int line_position{};
  int x = 0;
  int y = 0;
  int dim_x = 0;
  int dim_y = 0;
  bool overflow = false;
};

// Une ligne est une rangée de blocs.
struct Line {
  std::vector<Block*> blocks;
  int x = 0;
  int y = 0;
  int dim_x = 0;
  int dim_y = 0;
};

struct Global {
  std::vector<Block> blocks;
  std::vector<Line> lines;
  FlexboxConfig config;
  int size_x;
  int size_y;
};

void Compute(Global& global);

}  // namespace ftxui::flexbox_helper

#endif /* end of include guard: FTXUI_DOM_FLEXBOX_HELPER_HPP*/
