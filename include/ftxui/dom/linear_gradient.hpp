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

/// @brief Une classe représentant les paramètres de l'effet de couleur en dégradé linéaire.
///
/// Exemple:
/// ```cpp
/// LinearGradient()
///    .Angle(45)
///    .Stop(Color::Red, 0.0)
///    .Stop(Color::Green, 0.5)
///    .Stop(Color::Blue, 1.0);
/// ```
///
/// Il existe aussi des constructeurs raccourcis:
/// ```cpp
/// LinearGradient(Color::Red, Color::Blue);
/// LinearGradient(45, Color::Red, Color::Blue);
/// ```
///
/// @ingroup dom
struct FTXUI_EXPORT(DOM) LinearGradient {
  float angle = 0.f;

  /// Un "stop" est une couleur à une position spécifique dans le dégradé.
  /// La position est une valeur entre 0.0 et 1.0,
  /// où 0.0 est le début du dégradé
  /// et 1.0 est la fin du dégradé.
  struct Stop {
    Color color = Color::Default;
    std::optional<float> position;
  };
  std::vector<Stop> stops;

  // Constructeur simple
  LinearGradient();
  LinearGradient(Color begin, Color end);
  LinearGradient(float angle, Color begin, Color end);

  // Modificateur utilisant le modèle de conception "builder".
  LinearGradient& Angle(float angle);
  LinearGradient& Stop(Color color, float position);
  LinearGradient& Stop(Color color);
};

}  // namespace ftxui

#endif  // FTXUI_DOM_LINEAR_GRADIENT_HPP
