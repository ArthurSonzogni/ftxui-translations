// Copyright 2023 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef FTXUI_DOM_LINEAR_GRADIENT_HPP
#define FTXUI_DOM_LINEAR_GRADIENT_HPP

#include <optional>
#include <vector>

#include "ftxui/screen/color.hpp"  // for Colors

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
struct LinearGradient {
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