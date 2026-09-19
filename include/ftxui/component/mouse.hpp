// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_COMPONENT_MOUSE_HPP
#define FTXUI_COMPONENT_MOUSE_HPP

#include <cstdint>

#include "ftxui/util/export.hpp"

namespace ftxui {

/// @brief Un evento del ratón. Contiene las coordenadas del ratón, el botón
/// presionado y el modificador (shift, ctrl, meta).
/// @ingroup component
struct FTXUI_EXPORT(COMPONENT) Mouse {
  enum Button : uint8_t {
    Left = 0,
    Middle = 1,
    Right = 2,
    None = 3,
    WheelUp = 4,
    WheelDown = 5,
    WheelLeft = 6,   /// Solo terminales compatibles.
    WheelRight = 7,  /// Solo terminales compatibles.
  };

  enum Motion : uint8_t {
    Released = 0,
    Pressed = 1,
    Moved = 2,
  };

  // Botón
  Button button = Button::None;

  // Movimiento
  Motion motion = Motion::Pressed;

  // Modificadores:
  bool shift = false;
  bool meta = false;
  bool control = false;

  // Coordenadas:
  int x = 0;
  int y = 0;
};

}  // namespace ftxui

#endif /* fin de la guarda de inclusión: FTXUI_COMPONENT_MOUSE_HPP */
