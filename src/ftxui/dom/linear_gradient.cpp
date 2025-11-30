// Copyright 2023 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
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

// Convierte un LinearGradient a una versión normalizada.
LinearGradientNormalized Normalize(LinearGradient gradient) {
  // Maneja gradientes de tamaño 0.
  if (gradient.stops.empty()) {
    return LinearGradientNormalized{
        0.F,
        {Color::Default, Color::Default},
        {0.F, 1.F},
    };
  }

  // Rellena los dos extremos, si no se proporcionan.
  if (!gradient.stops.front().position) {
    gradient.stops.front().position = 0.F;
  }
  if (!gradient.stops.back().position) {
    gradient.stops.back().position = 1.F;
  }

  // Rellena los espacios en blanco, interpolando posiciones.
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

  // Ordena las paradas por posición.
  std::sort(
      gradient.stops.begin(), gradient.stops.end(),
      [](const auto& a, const auto& b) { return a.position < b.position; });

  // Si no comenzamos con cero, añade una parada en cero.
  if (gradient.stops.front().position != 0) {
    gradient.stops.insert(gradient.stops.begin(),
                          {gradient.stops.front().color, 0.F});
  }
  // Si no terminamos con uno, añade una parada en uno.
  if (gradient.stops.back().position != 1) {
    gradient.stops.push_back({gradient.stops.back().color, 1.F});
  }

  // Normaliza el ángulo.
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
  // Encuentra el color correcto en las paradas del gradiente.
  size_t i = 1;
  while (true) {
    // Ten en cuenta que `t` podría ser ligeramente mayor que 1.0 debido a la precisión
    // de punto flotante. Por eso necesitamos manejar el caso en que `t` sea mayor
    // que la posición de la última parada.
    // Consulta https://github.com/ArthurSonzogni/FTXUI/issues/998
    if (i >= gradient.positions.size()) {
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

    // Project every corner to get the extent of the gradient.
    const float p1 = float(box_.x_min) * dx + float(box_.y_min) * dy;
    const float p2 = float(box_.x_min) * dx + float(box_.y_max) * dy;
    const float p3 = float(box_.x_max) * dx + float(box_.y_min) * dy;
    const float p4 = float(box_.x_max) * dx + float(box_.y_max) * dy;
    const float min = std::min({p1, p2, p3, p4});
    const float max = std::max({p1, p2, p3, p4});

    // Renormalize the projection to [0, 1] using the extent and projective
    // geometry.
    const float dX = dx / (max - min);
    const float dY = dy / (max - min);
    const float dZ = -min / (max - min);

    // Project every pixel to get the color.
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

/// @brief Construye el gradiente "vacío". Esto a menudo es seguido por llamadas a
/// LinearGradient::Angle() y LinearGradient::Stop().
/// Ejemplo:
/// ```cpp
///  auto gradient =
///   LinearGradient()
///    .Angle(45)
///    .Stop(Color::Red, 0.0)
///    .Stop(Color::Green, 0.5)
///    .Stop(Color::Blue, 1.0);;
/// ```
LinearGradient::LinearGradient() = default;

/// @brief Construye un gradiente con dos colores.
/// @param begin El color al principio del gradiente.
/// @param end El color al final del gradiente.
LinearGradient::LinearGradient(Color begin, Color end)
    : LinearGradient(0, begin, end) {}

/// @brief Construye un gradiente con dos colores y un ángulo.
/// @param a El ángulo del gradiente.
/// @param begin El color al principio del gradiente.
/// @param end El color al final del gradiente.
LinearGradient::LinearGradient(float a, Color begin, Color end) : angle(a) {
  stops.push_back({begin, {}});
  stops.push_back({end, {}});
}

/// @brief Establece el ángulo del gradiente.
/// @param a El ángulo del gradiente.
/// @return El gradiente.
LinearGradient& LinearGradient::Angle(float a) {
  angle = a;
  return *this;
}

/// @brief Añade una parada de color al gradiente.
/// @param c El color de la parada.
/// @param p La posición de la parada.
LinearGradient& LinearGradient::Stop(Color c, float p) {
  stops.push_back({c, p});
  return *this;
}

/// @brief Añade una parada de color al gradiente.
/// @param c El color de la parada.
/// @return El gradiente.
/// @note La posición de la parada se interpola a partir de las paradas cercanas.
LinearGradient& LinearGradient::Stop(Color c) {
  stops.push_back({c, {}});
  return *this;
}

/// @brief Establece el color de primer plano de un elemento con efecto de gradiente lineal.
/// @param gradient El efecto de gradiente que se aplicará al elemento de salida.
/// @param child El elemento de entrada.
/// @return El elemento de salida coloreado.
/// @ingroup dom
///
/// ### Ejemplo
///
/// ```cpp
/// color(LinearGradient{0, {Color::Red, Color::Blue}}, text("Hello"))
/// ```
Element color(const LinearGradient& gradient, Element child) {
  return std::make_shared<LinearGradientColor>(std::move(child), gradient,
                                               /*background_color*/ false);
}

/// @brief Establece el color de fondo de un elemento con efecto de gradiente lineal.
/// @param gradient El efecto de gradiente que se aplicará al elemento de salida.
/// @param child El elemento de entrada.
/// @return El elemento de salida coloreado.
/// @ingroup dom
///
/// ### Ejemplo
///
/// ```cpp
/// bgcolor(LinearGradient{0, {Color::Red, Color::Blue}}, text("Hello"))
/// ```
Element bgcolor(const LinearGradient& gradient, Element child) {
  return std::make_shared<LinearGradientColor>(std::move(child), gradient,
                                               /*background_color*/ true);
}

/// @brief Decora usando un efecto de gradiente lineal en el color de primer plano.
/// @param gradient El efecto de gradiente que se aplicará al elemento de salida.
/// @return El Decorator que aplica el color.
/// @ingroup dom
///
/// ### Ejemplo
///
/// ```cpp
/// text("Hello") | color(LinearGradient{0, {Color::Red, Color::Blue}})
/// ```
Decorator color(const LinearGradient& gradient) {
  return
      [gradient](Element child) { return color(gradient, std::move(child)); };
}

/// @brief Decora usando un efecto de gradiente lineal en el color de fondo.
/// @param gradient El efecto de gradiente que se aplicará al elemento de salida.
/// @return El Decorator que aplica el color.
/// @ingroup dom
///
/// ### Ejemplo
///
/// ```cpp
/// text("Hello") | color(LinearGradient{0, {Color::Red, Color::Blue}})
/// ```
Decorator bgcolor(const LinearGradient& gradient) {
  return
      [gradient](Element child) { return bgcolor(gradient, std::move(child)); };
}

}  // namespace ftxui
