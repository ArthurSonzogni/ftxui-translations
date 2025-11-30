// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <ftxui/dom/direction.hpp>  // for Direction, Direction::Down, Direction::Left, Direction::Right, Direction::Up
#include <memory>                   // for allocator, make_shared
#include <string>                   // for string

#include "ftxui/dom/elements.hpp"  // for Element, gauge, gaugeDirection, gaugeDown, gaugeLeft, gaugeRight, gaugeUp
#include "ftxui/dom/node.hpp"         // for Node
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/screen.hpp"    // for Screen, Pixel

namespace ftxui {

namespace {
// NOLINTNEXTLINE
static const std::string charset_horizontal[11] = {
#if defined(FTXUI_MICROSOFT_TERMINAL_FALLBACK)
    // Las terminales de Microsoft a menudo usan fuentes que no manejan los 8 caracteres unicode
    // para representar el medidor completo. Se utiliza una alternativa con menos caracteres.
    " ", " ", " ", " ", "▌", "▌", "▌", "█", "█", "█",
#else
    " ", " ", "▏", "▎", "▍", "▌", "▋", "▊", "▉", "█",
#endif
    // Un carácter extra en caso de que el fuzzer consiga tener:
    // int(9 * (limite - limite_int)) = 9
    "█"};

// NOLINTNEXTLINE
static const std::string charset_vertical[10] = {
    "█",
    "▇",
    "▆",
    "▅",
    "▄",
    "▃",
    "▂",
    "▁",
    " ",
    // Un carácter extra en caso de que el fuzzer consiga tener:
    // int(8 * (limite - limite_int)) = 8
    " ",
};

class Gauge : public Node {
 public:
  Gauge(float progress, Direction direction)
      : progress_(progress), direction_(direction) {
    // Esto maneja NAN correctamente:
    if (!(progress_ > 0.F)) {
      progress_ = 0.F;
    }
    if (!(progress_ < 1.F)) {
      progress_ = 1.F;
    }
  }

  void ComputeRequirement() override {
    switch (direction_) {
      case Direction::Right:
      case Direction::Left:
        requirement_.flex_grow_x = 1;
        requirement_.flex_grow_y = 0;
        requirement_.flex_shrink_x = 1;
        requirement_.flex_shrink_y = 0;
        break;
      case Direction::Up:
      case Direction::Down:
        requirement_.flex_grow_x = 0;
        requirement_.flex_grow_y = 1;
        requirement_.flex_shrink_x = 0;
        requirement_.flex_shrink_y = 1;
        break;
    }
    requirement_.min_x = 1;
    requirement_.min_y = 1;
  }

  void Render(Screen& screen) override {
    switch (direction_) {
      case Direction::Right:
        RenderHorizontal(screen, /*invert=*/false);
        break;
      case Direction::Up:
        RenderVertical(screen, /*invert=*/false);
        break;
      case Direction::Left:
        RenderHorizontal(screen, /*invert=*/true);
        break;
      case Direction::Down:
        RenderVertical(screen, /*invert=*/true);
        break;
    }
  }

  void RenderHorizontal(Screen& screen, bool invert) {
    const int y = box_.y_min;
    if (y > box_.y_max) {
      return;
    }

    // Draw the progress bar horizontally.
    {
      const float progress = invert ? 1.F - progress_ : progress_;
      const auto limit =
          float(box_.x_min) + progress * float(box_.x_max - box_.x_min + 1);
      const int limit_int = static_cast<int>(limit);
      int x = box_.x_min;
      while (x < limit_int) {
        screen.at(x++, y) = charset_horizontal[9];  // NOLINT
      }
      // NOLINTNEXTLINE
      screen.at(x++, y) = charset_horizontal[int(9 * (limit - limit_int))];
      while (x <= box_.x_max) {
        screen.at(x++, y) = charset_horizontal[0];
      }
    }

    if (invert) {
      for (int x = box_.x_min; x <= box_.x_max; x++) {
        screen.PixelAt(x, y).inverted ^= true;
      }
    }
  }

  void RenderVertical(Screen& screen, bool invert) {
    const int x = box_.x_min;
    if (x > box_.x_max) {
      return;
    }

    // Draw the progress bar vertically:
    {
      const float progress = invert ? progress_ : 1.F - progress_;
      const float limit =
          float(box_.y_min) + progress * float(box_.y_max - box_.y_min + 1);
      const int limit_int = static_cast<int>(limit);
      int y = box_.y_min;
      while (y < limit_int) {
        screen.at(x, y++) = charset_vertical[8];  // NOLINT
      }
      // NOLINTNEXTLINE
      screen.at(x, y++) = charset_vertical[int(8 * (limit - limit_int))];
      while (y <= box_.y_max) {
        screen.at(x, y++) = charset_vertical[0];
      }
    }

    if (invert) {
      for (int y = box_.y_min; y <= box_.y_max; y++) {
        screen.PixelAt(x, y).inverted ^= true;
      }
    }
  }

 private:
  float progress_;
  Direction direction_;
};

}  // namespace

/// @brief Dibuja una barra de progreso de alta definición que avanza en la dirección especificada.
/// @param progress La proporción del área a rellenar. Pertenece a [0,1].
/// @param direction Dirección de progresión de la barra de progreso.
/// @ingroup dom
Element gaugeDirection(float progress, Direction direction) {
  return std::make_shared<Gauge>(progress, direction);
}

/// @brief Dibuja una barra de progreso de alta definición que avanza de izquierda a derecha.
/// @param progress La proporción del área a rellenar. Pertenece a [0,1].
/// @ingroup dom
///
/// ### Ejemplo
///
/// Un medidor. Puede ser usado para representar una barra de progreso.
/// ~~~cpp
/// border(gaugeRight(0.5))
/// ~~~
///
/// #### Output
///
/// ~~~bash
/// ┌──────────────────────────────────────────────────────────────────────────┐
/// │█████████████████████████████████████                                     │
/// └──────────────────────────────────────────────────────────────────────────┘
/// ~~~
Element gaugeRight(float progress) {
  return gaugeDirection(progress, Direction::Right);
}

/// @brief Dibuja una barra de progreso de alta definición que avanza de derecha a izquierda.
/// @param progress La proporción del área a rellenar. Pertenece a [0,1].
/// @ingroup dom
///
/// ### Ejemplo
///
/// Un medidor. Puede ser usado para representar una barra de progreso.
/// ~~~cpp
/// border(gaugeLeft(0.5))
/// ~~~
///
/// #### Output
///
/// ~~~bash
/// ┌──────────────────────────────────────────────────────────────────────────┐
/// │                                     █████████████████████████████████████│
/// └──────────────────────────────────────────────────────────────────────────┘
/// ~~~
Element gaugeLeft(float progress) {
  return gaugeDirection(progress, Direction::Left);
}

/// @brief Dibuja una barra de progreso de alta definición que avanza de abajo hacia arriba.
/// @param progress La proporción del área a rellenar. Pertenece a [0,1].
/// @ingroup dom
///
/// ### Ejemplo
///
/// Un medidor. Puede ser usado para representar una barra de progreso.
/// ~~~cpp
/// border(gaugeUp(0.5))
/// ~~~
///
/// #### Output
///
/// ~~~bash
///  ┌─┐
///  │ │
///  │ │
///  │ │
///  │ │
///  │█│
///  │█│
///  │█│
///  │█│
///  └─┘
/// ~~~
Element gaugeUp(float progress) {
  return gaugeDirection(progress, Direction::Up);
}

/// @brief Dibuja una barra de progreso de alta definición que avanza de arriba hacia abajo.
/// @param progress La proporción del área a rellenar. Pertenece a [0,1].
/// @ingroup dom
///
/// ### Ejemplo
///
/// Un medidor. Puede ser usado para representar una barra de progreso.
/// ~~~cpp
/// border(gaugeDown(0.5))
/// ~~~
///
/// #### Output
///
/// ~~~bash
///  ┌─┐
///  │█│
///  │█│
///  │█│
///  │█│
///  │ │
///  │ │
///  │ │
///  │ │
///  └─┘
/// ~~~
Element gaugeDown(float progress) {
  return gaugeDirection(progress, Direction::Down);
}

/// @brief Dibuja una barra de progreso de alta definición.
/// @param progress La proporción del área a rellenar. Pertenece a [0,1].
/// @ingroup dom
///
/// ### Ejemplo
///
/// Un medidor. Puede ser usado para representar una barra de progreso.
/// ~~~cpp
/// border(gauge(0.5))
/// ~~~
///
/// #### Output
///
/// ~~~bash
/// ┌──────────────────────────────────────────────────────────────────────────┐
/// │█████████████████████████████████████                                     │
/// └──────────────────────────────────────────────────────────────────────────┘
/// ~~~
Element gauge(float progress) {
  return gaugeRight(progress);
}

}  // namespace ftxui
