// Copyright 2023 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_LINEAR_GRADIENT_HPP
#define FTXUI_DOM_LINEAR_GRADIENT_HPP

#include <optional>
#include <vector>

#include "ftxui/screen/color.hpp"  // for Colors
#include "ftxui/util/export.hpp"   // for FTXUI_EXPORT

namespace ftxui {

/// @brief Una clase que representa la configuración para el efecto de color de gradiente lineal.
///
/// Ejemplo:
/// ```cpp
/// LinearGradient()
///    .Angle(45)
///    .Stop(Color::Red, 0.0)
///    .Stop(Color::Green, 0.5)
///    .Stop(Color::Blue, 1.0);
/// ```
///
/// También hay constructores abreviados:
/// ```cpp
/// LinearGradient(Color::Red, Color::Blue);
/// LinearGradient(45, Color::Red, Color::Blue);
/// ```
///
/// @ingroup dom
struct FTXUI_EXPORT(DOM) LinearGradient {
  float angle = 0.f;

  /// Un "stop" es un color en una posición específica en el gradiente.
  /// La posición es un valor entre 0.0 y 1.0,
  /// donde 0.0 es el inicio del gradiente
  /// y 1.0 es el final del gradiente.
  struct Stop {
    Color color = Color::Default;
    std::optional<float> position;
  };
  std::vector<Stop> stops;

  // Constructor simple
  LinearGradient();
  LinearGradient(Color begin, Color end);
  LinearGradient(float angle, Color begin, Color end);

  // Modificador usando el patrón "builder".
  LinearGradient& Angle(float angle);
  LinearGradient& Stop(Color color, float position);
  LinearGradient& Stop(Color color);
};

}  // namespace ftxui

#endif  // FTXUI_DOM_LINEAR_GRADIENT_HPP
