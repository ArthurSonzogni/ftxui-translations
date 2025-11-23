// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#include "ftxui/dom/flexbox_config.hpp"

namespace ftxui {

/// @brief Définit la direction de la flexbox.
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::Direction d) {
  this->direction = d;
  return *this;
}

/// @brief Définit l'habillage (wrap) de la flexbox.
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::Wrap w) {
  this->wrap = w;
  return *this;
}

/// @brief Définit la justification du contenu (justify content) de la flexbox.
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::JustifyContent j) {
  this->justify_content = j;
  return *this;
}

/// @brief Définit l'alignement des éléments (align items) de la flexbox.
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::AlignItems a) {
  this->align_items = a;
  return *this;
}

/// @brief Définit l'alignement du contenu (align content) de la flexbox.
FlexboxConfig& FlexboxConfig::Set(FlexboxConfig::AlignContent a) {
  this->align_content = a;
  return *this;
}

/// @brief Définit la direction de flex (flex direction) de la flexbox.
FlexboxConfig& FlexboxConfig::SetGap(int x, int y) {
  this->gap_x = x;
  this->gap_y = y;
  return *this;
}

}  // namespace ftxui
