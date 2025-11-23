// Copyright 2020 Arthur Sonzogni. All rights reserved.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#ifndef FTXUI_DOM_REQUIREMENT_HPP
#define FTXUI_DOM_REQUIREMENT_HPP

#include "ftxui/screen/box.hpp"
#include "ftxui/screen/screen.hpp"

namespace ftxui {
class Node;

/// @brief Requirement est une structure qui définit les exigences de mise en page pour un
/// Nœud dans l'interface utilisateur du terminal.
///
/// Elle spécifie la taille minimale requise pour dessiner entièrement l'élément,
/// @ingroup dom
struct Requirement {
  // La taille requise pour dessiner entièrement l'élément.
  int min_x = 0;
  int min_y = 0;

  // Combien de flexibilité est donnée au composant.
  int flex_grow_x = 0;
  int flex_grow_y = 0;
  int flex_shrink_x = 0;
  int flex_shrink_y = 0;

  // Gestion du focus pour prendre en charge l'élément frame/focus/select.
  struct Focused {
    bool enabled = false;
    Box box;
    Node* node = nullptr;
    Screen::Cursor::Shape cursor_shape = Screen::Cursor::Shape::Hidden;

    // Interne pour les interactions avec les composants.
    bool component_active = false;

    // Retourne si cette exigence doit être préférée à l'autre.
    bool Prefer(const Focused& other) const {
      if (!other.enabled) {
        return false;
      }
      if (!enabled) {
        return true;
      }

      return other.component_active && !component_active;
    }
  };
  Focused focused;
};

}  // namespace ftxui

#endif  // FTXUI_DOM_REQUIREMENT_HPP