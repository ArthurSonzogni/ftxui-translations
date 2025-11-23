// Copyright 2023 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <algorithm>                      // for max, min, sort, copy
#include <cmath>                          // for fmod, cos, sin
#include <cstddef>                        // for size_t
#include <ftxui/dom/linear_gradient.hpp>  // for LinearGradient::Stop, LinearGradient
#include <memory>    // for allocator_traits<>::value_type, make_shared
#include <optional>  // for optional, operator!=, operator<
#include <utility>   // for move
#include <vector>    // for vector

#include "ftxui/dom/elements.hpp"  // for Element, Decorator, bgcolor, color
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/box.hpp"          // for Box
#include "ftxui/screen/color.hpp"   // for Color, Color::Default, Color::Blue
#include "ftxui/screen/screen.hpp"  // for Pixel, Screen

namespace ftxui {
namespace {

struct LinearGradientNormalized {
  float angle = 0.F;
  std::vector<Color> colors;
  std::vector<float> positions;  // Sorted.
};

// Convertit un LinearGradient en une version normalisée.
LinearGradientNormalized Normalize(LinearGradient gradient) {
  // Gère les dégradés de taille 0.
  if (gradient.stops.empty()) {
    return LinearGradientNormalized{
        0.F,
        {Color::Default, Color::Default},
        {0.F, 1.F},
    };
  }

  // Remplit les deux étendues, si non fournies.
  if (!gradient.stops.front().position) {
    gradient.stops.front().position = 0.F;
  }
  if (!gradient.stops.back().position) {
    gradient.stops.back().position = 1.F;
  }

  // Remplit les blancs, en interpolant les positions.
  size_t last_checkpoint = 0;
  for (size_t i = 1; i < gradient.stops.size(); ++i) {
    if (!gradient.stops[i].position) {
      continue;
    }

    if (i - last_checkpoint >= 2) {
      const float min = gradient.stops[i].position.value();  // NOLINT
      const float max =
          gradient.stops[last_checkpoint].position.value();  // NOLINT
      for (size_t j = last_checkpoint + 1; j < i; ++j) {
        gradient.stops[j].position = min + (max - min) *
                                               float(j - last_checkpoint) /
                                               float(i - last_checkpoint);
      }
    }

    last_checkpoint = i;
  }

  // Trie les arrêts par position.
  std::sort(
      gradient.stops.begin(), gradient.stops.end(),
      [](const auto& a, const auto& b) { return a.position < b.position; });

  // Si on ne commence pas à zéro, ajoute un arrêt à zéro.
  if (gradient.stops.front().position != 0) {
    gradient.stops.insert(gradient.stops.begin(),
                          {gradient.stops.front().color, 0.F});
  }
  // Si on ne termine pas à un, ajoute un arrêt à un.
  if (gradient.stops.back().position != 1) {
    gradient.stops.push_back({gradient.stops.back().color, 1.F});
  }

  // Normalise l'angle.
  LinearGradientNormalized normalized;
  const float modulo = 360.F;
  normalized.angle =
      std::fmod(std::fmod(gradient.angle, modulo) + modulo, modulo);
  for (auto& stop : gradient.stops) {
    normalized.colors.push_back(stop.color);
    // NOLINTNEXTLINE
    normalized.positions.push_back(stop.position.value());
  }
  return normalized;
}

Color Interpolate(const LinearGradientNormalized& gradient, float t) {
  // Trouve la bonne couleur dans les arrêts du dégradé.
  size_t i = 1;
  while (true) {
    // Notez que `t` peut être légèrement supérieur à 1.0 en raison de la précision des nombres flottants.
    // C'est pourquoi nous devons gérer le cas où `t` est supérieur à la position du dernier arrêt.
    // Voir https://github.com/ArthurSonzogni/FTXUI/issues/998    if (i >= gradient.positions.size()) {
      const float half = 0.5F;
      return Color::Interpolate(half, gradient.colors.back(),
                                gradient.colors.back());
    }
    if (t <= gradient.positions[i]) {
      break;
    }
    ++i;
  }

  const float t0 = gradient.positions[i - 1];
  const float t1 = gradient.positions[i - 0];
  const float tt = (t - t0) / (t1 - t0);

  const Color& c0 = gradient.colors[i - 1];
  const Color& c1 = gradient.colors[i - 0];
  const Color& cc = Color::Interpolate(tt, c0, c1);

  return cc;
}

class LinearGradientColor : public NodeDecorator {
 public:
  explicit LinearGradientColor(Element child,
                               const LinearGradient& gradient,
                               bool background_color)
      : NodeDecorator(std::move(child)),
        gradient_(Normalize(gradient)),
        background_color_{background_color} {}

 private:
  void Render(Screen& screen) override {
    const float degtorad = 0.01745329251F;
    const float dx = std::cos(gradient_.angle * degtorad);
    const float dy = std::sin(gradient_.angle * degtorad);

    // Projeter chaque coin pour obtenir l'étendue du dégradé.
    const float p1 = float(box_.x_min) * dx + float(box_.y_min) * dy;
    const float p2 = float(box_.x_min) * dx + float(box_.y_max) * dy;
    const float p3 = float(box_.x_max) * dx + float(box_.y_min) * dy;
    const float p4 = float(box_.x_max) * dx + float(box_.y_max) * dy;
    const float min = std::min({p1, p2, p3, p4});
    const float max = std::max({p1, p2, p3, p4});

    // Renormaliser la projection à [0, 1] en utilisant l'étendue et la géométrie projective.    const float dX = dx / (max - min);
    const float dY = dy / (max - min);
    const float dZ = -min / (max - min);

    // Projeter chaque pixel pour obtenir la couleur.
    if (background_color_) {
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        for (int x = box_.x_min; x <= box_.x_max; ++x) {
          const float t = float(x) * dX + float(y) * dY + dZ;
          screen.PixelAt(x, y).background_color = Interpolate(gradient_, t);
        }
      }
    } else {
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        for (int x = box_.x_min; x <= box_.x_max; ++x) {
          const float t = float(x) * dX + float(y) * dY + dZ;
          screen.PixelAt(x, y).foreground_color = Interpolate(gradient_, t);
        }
      }
    }

    NodeDecorator::Render(screen);
  }

  LinearGradientNormalized gradient_;
  bool background_color_;
};

}  // namespace

/// @brief Construit le dégradé "vide". Ceci est souvent suivi d'appels à
/// LinearGradient::Angle() et LinearGradient::Stop().
/// Exemple:
/// ```cpp
///  auto gradient =
///   LinearGradient()
///    .Angle(45)
///    .Stop(Color::Red, 0.0)
///    .Stop(Color::Green, 0.5)
///    .Stop(Color::Blue, 1.0);;
/// ```
LinearGradient::LinearGradient() = default;

/// @brief Construit un dégradé avec deux couleurs.
/// @param begin La couleur au début du dégradé.
/// @param end La couleur à la fin du dégradé.
LinearGradient::LinearGradient(Color begin, Color end)
    : LinearGradient(0, begin, end) {}

/// @brief Construit un dégradé avec deux couleurs et un angle.
/// @param a L'angle du dégradé.
/// @param begin La couleur au début du dégradé.
/// @param end La couleur à la fin du dégradé.
LinearGradient::LinearGradient(float a, Color begin, Color end) : angle(a) {
  stops.push_back({begin, {}});
  stops.push_back({end, {}});
}

/// @brief Définit l'angle du dégradé.
/// @param a L'angle du dégradé.
/// @return Le dégradé.
LinearGradient& LinearGradient::Angle(float a) {
  angle = a;
  return *this;
}

/// @brief Ajoute un arrêt de couleur au dégradé.
/// @param c La couleur de l'arrêt.
/// @param p La position de l'arrêt.
LinearGradient& LinearGradient::Stop(Color c, float p) {
  stops.push_back({c, p});
  return *this;
}

/// @brief Ajoute un arrêt de couleur au dégradé.
/// @param c La couleur de l'arrêt.
/// @return Le dégradé.
/// @note La position de l'arrêt est interpolée à partir des arrêts voisins.
LinearGradient& LinearGradient::Stop(Color c) {
  stops.push_back({c, {}});
  return *this;
}

/// @brief Définit la couleur de premier plan d'un élément avec un effet de dégradé linéaire.
/// @param gradient L'effet de dégradé à appliquer sur l'élément de sortie.
/// @param child L'élément d'entrée.
/// @return L'élément de sortie coloré.
/// @ingroup dom
///
/// ### Exemple
///
/// ```cpp
/// color(LinearGradient{0, {Color::Red, Color::Blue}}, text("Hello"))
/// ```
Element color(const LinearGradient& gradient, Element child) {
  return std::make_shared<LinearGradientColor>(std::move(child), gradient,
                                               /*background_color*/ false);
}

/// @brief Définit la couleur de fond d'un élément avec un effet de dégradé linéaire.
/// @param gradient L'effet de dégradé à appliquer sur l'élément de sortie.
/// @param child L'élément d'entrée.
/// @return L'élément de sortie coloré.
/// @ingroup dom
///
/// ### Exemple
///
/// ```cpp
/// bgcolor(LinearGradient{0, {Color::Red, Color::Blue}}, text("Hello"))
/// ```
Element bgcolor(const LinearGradient& gradient, Element child) {
  return std::make_shared<LinearGradientColor>(std::move(child), gradient,
                                               /*background_color*/ true);
}

/// @brief Décore en utilisant un effet de dégradé linéaire sur la couleur de premier plan.
/// @param gradient L'effet de dégradé à appliquer sur l'élément de sortie.
/// @return Le Decorator appliquant la couleur.
/// @ingroup dom
///
/// ### Exemple
///
/// ```cpp
/// text("Hello") | color(LinearGradient{0, {Color::Red, Color::Blue}})
/// ```
Decorator color(const LinearGradient& gradient) {
  return
      [gradient](Element child) { return color(gradient, std::move(child)); };
}

/// @brief Décore en utilisant un effet de dégradé linéaire sur la couleur de fond.
/// @param gradient L'effet de dégradé à appliquer sur l'élément de sortie.
/// @return Le Decorator appliquant la couleur.
/// @ingroup dom
///
/// ### Exemple
///
/// ```cpp
/// text("Hello") | color(LinearGradient{0, {Color::Red, Color::Blue}})
/// ```
Decorator bgcolor(const LinearGradient& gradient) {
  return
      [gradient](Element child) { return bgcolor(gradient, std::move(child)); };
}

}  // namespace ftxui
