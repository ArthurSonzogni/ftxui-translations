// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef FTXUI_DOM_FLEXBOX_HELPER_HPP
#define FTXUI_DOM_FLEXBOX_HELPER_HPP

#include <vector>
#include "ftxui/dom/flexbox_config.hpp"

namespace ftxui::flexbox_helper {

// Un bloque es un rectángulo en el flexbox.
struct Block {
  // Entrada:
  int min_size_x = 0;
  int min_size_y = 0;
  int flex_grow_x = 0;
  int flex_grow_y = 0;
  int flex_shrink_x = 0;
  int flex_shrink_y = 0;

  // Salida:
  int line{};
  int line_position{};
  int x = 0;
  int y = 0;
  int dim_x = 0;
  int dim_y = 0;
  bool overflow = false;
};

// Una línea es una fila de bloques.
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

#endif /* fin de la guarda de inclusión: FTXUI_DOM_FLEXBOX_HELPER_HPP*/