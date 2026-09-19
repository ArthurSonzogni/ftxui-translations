// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_COMPONENT_MOUSE_HPP
#define FTXUI_COMPONENT_MOUSE_HPP

#include <cstdint>

#include "ftxui/util/export.hpp"

namespace ftxui {

/// @brief Un événement de souris. Il contient les coordonnées de la souris, le bouton
/// pressé et le modificateur (shift, ctrl, méta).
/// @ingroup component
struct FTXUI_EXPORT(COMPONENT) Mouse {
  enum Button : uint8_t {
    Left = 0,
    Middle = 1,
    Right = 2,
    None = 3,
    WheelUp = 4,
    WheelDown = 5,
    WheelLeft = 6,   /// Supporté par le terminal uniquement.
    WheelRight = 7,  /// Supporté par le terminal uniquement.
  };

  enum Motion : uint8_t {
    Released = 0,
    Pressed = 1,
    Moved = 2,
  };

  // Button
  Button button = Button::None;

  // Motion
  Motion motion = Motion::Pressed;

  // Modifiers:
  bool shift = false;
  bool meta = false;
  bool control = false;

  // Coordinates:
  int x = 0;
  int y = 0;
};

}  // namespace ftxui

#endif /* fin de la garde d'inclusion: FTXUI_COMPONENT_MOUSE_HPP */
