// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include "ftxui/dom/flexbox_config.hpp"

namespace ftxui {

/// @brief Establece la dirección del flexbox.
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::Direction d) {
  this->direction = d;
  return *this;
}

/// @brief Establece el ajuste (wrap) del flexbox.
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::Wrap w) {
  this->wrap = w;
  return *this;
}

/// @brief Establece la justificación del contenido (justify content) del flexbox.
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::JustifyContent j) {
  this->justify_content = j;
  return *this;
}

/// @brief Establece la alineación de los ítems (align items) del flexbox.
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::AlignItems a) {
  this->align_items = a;
  return *this;
}

/// @brief Establece la alineación del contenido (align content) del flexbox.
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::AlignContent a) {
  this->align_content = a;
  return *this;
}

/// @brief Establece el espacio (gap) del flexbox.
FlexboxConfig& FlexboxConfig::SetGap(int x, int y) {
  this->gap_x = x;
  this->gap_y = y;
  return *this;
}

}  // namespace ftxui
