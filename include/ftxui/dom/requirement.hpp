// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef FTXUI_DOM_REQUIREMENT_HPP
#define FTXUI_DOM_REQUIREMENT_HPP

#include "ftxui/screen/box.hpp"
#include "ftxui/screen/screen.hpp"

namespace ftxui {
class Node;

/// @brief Requirement es una estructura que define los requisitos de diseño para un
/// Nodo en la interfaz de usuario del terminal.
///
/// Especifica el tamaño mínimo requerido para dibujar completamente el elemento,
/// @ingroup dom
struct Requirement {
  // El tamaño requerido para dibujar completamente el elemento.
  int min_x = 0;
  int min_y = 0;

  // Cuánta flexibilidad se le da al componente.
  int flex_grow_x = 0;
  int flex_grow_y = 0;
  int flex_shrink_x = 0;
  int flex_shrink_y = 0;

  // Gestión del foco para soportar el elemento frame/focus/select.
  struct Focused {
    bool enabled = false;
    Box box;
    Node* node = nullptr;
    Screen::Cursor::Shape cursor_shape = Screen::Cursor::Shape::Hidden;

    // Interno para interacciones con componentes.
    bool component_active = false;

    // Devuelve si este requisito debe ser preferido sobre el otro.
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
