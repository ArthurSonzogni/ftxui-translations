// Copyright 2023 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_DIRECTION_HPP
#define FTXUI_DOM_DIRECTION_HPP

#include <cstdint>

namespace ftxui {

/// @brief Direction es una enumeración que representa las cuatro cardinales
/// direcciones.
///
/// @ingroup dom
enum class Direction : uint8_t {
  Up = 0,
  Down = 1,
  Left = 2,
  Right = 3,
};

}  // namespace ftxui

#endif /* fin de la guarda de inclusión: FTXUI_DOM_DIRECTION_HPP */
