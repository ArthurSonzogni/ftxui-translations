// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <array>    // for array, array<>::value_type
#include <memory>   // for make_shared, allocator
#include <string>   // for basic_string, string
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Element, BorderStyle, LIGHT, separator, DOUBLE, EMPTY, HEAVY, separatorCharacter, separatorDouble, separatorEmpty, separatorHSelector, separatorHeavy, separatorLight, separatorStyled, separatorVSelector
#include "ftxui/dom/node.hpp"      // for Node
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/color.hpp"     // for Color
#include "ftxui/screen/pixel.hpp"     // for Pixel
#include "ftxui/screen/screen.hpp"    // for Pixel, Screen

namespace ftxui {

namespace {
using Charset = std::array<std::string, 2>;  // NOLINT
using Charsets = std::array<Charset, 6>;     // NOLINT
// NOLINTNEXTLINE
const Charsets charsets = {
    Charset{"│", "─"},  // LIGERO
    Charset{"╏", "╍"},  // DISCONTINUO
    Charset{"┃", "━"},  // GRUESO
    Charset{"║", "═"},  // DOBLE
    Charset{"│", "─"},  // REDONDEADO
    Charset{" ", " "},  // VACÍO
};

class Separator : public Node {
 public:
  explicit Separator(std::string value) : value_(std::move(value)) {}

  void ComputeRequirement() override {
    requirement_.min_x = 1;
    requirement_.min_y = 1;
  }

  void Render(Screen& screen) override {
    for (int y = box_.y_min; y <= box_.y_max; ++y) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        Pixel& pixel = screen.PixelAt(x, y);
        pixel.character = value_;
        pixel.automerge = true;
      }
    }
  }

  std::string value_;
};

class SeparatorAuto : public Node {
 public:
  explicit SeparatorAuto(BorderStyle style) : style_(style) {}

  void ComputeRequirement() override {
    requirement_.min_x = 1;
    requirement_.min_y = 1;
  }

  void Render(Screen& screen) override {
    const bool is_column = (box_.x_max == box_.x_min);
    const bool is_line = (box_.y_min == box_.y_max);

    const std::string c =
        charsets[style_][int(is_line && !is_column)];  // NOLINT

    for (int y = box_.y_min; y <= box_.y_max; ++y) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        Pixel& pixel = screen.PixelAt(x, y);
        pixel.character = c;
        pixel.automerge = true;
      }
    }
  }

  BorderStyle style_;
};

class SeparatorWithPixel : public SeparatorAuto {
 public:
  explicit SeparatorWithPixel(Pixel pixel)
      : SeparatorAuto(LIGHT), pixel_(std::move(pixel)) {
    pixel_.automerge = true;
  }
  void Render(Screen& screen) override {
    for (int y = box_.y_min; y <= box_.y_max; ++y) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        screen.PixelAt(x, y) = pixel_;
      }
    }
  }

 private:
  Pixel pixel_;
};
}  // namespace

/// @brief Dibuja una separación vertical u horizontal entre otros dos
/// elementos.
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// Añade una separación visual entre dos elementos.
///
/// ### Ejemplo
///
/// ```cpp
/// // Use 'border' as a function...
/// Element document = vbox({
///   text("up"),
///   separator(),
///   text("down"),
/// });
/// ```
///
/// ### Salida
///
/// ```bash
/// up
/// ────
/// down
/// ```
Element separator() {
  return std::make_shared<SeparatorAuto>(LIGHT);
}

/// @brief Dibuja una separación vertical u horizontal entre otros dos
/// elementos.
/// @param style el estilo del separador.
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// Añade una separación visual entre dos elementos.
///
/// ### Ejemplo
///
/// ```cpp
/// // Use 'border' as a function...
/// Element document = vbox({
///   text("up"),
///   separatorStyled(DOUBLE),
///   text("down"),
/// });
/// ```
///
/// ### Salida
///
/// ```bash
/// up
/// ════
/// down
/// ```
Element separatorStyled(BorderStyle style) {
  return std::make_shared<SeparatorAuto>(style);
}

/// @brief Dibuja una separación vertical u horizontal entre otros dos
/// elementos, usando el estilo LIGERO.
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// Añade una separación visual entre dos elementos.
///
/// ### Ejemplo
///
/// ```cpp
/// // Use 'border' as a function...
/// Element document = vbox({
///   text("up"),
///   separatorLight(),
///   text("down"),
/// });
/// ```
///
/// ### Salida
///
/// ```bash
/// up
/// ────
/// down
/// ```
Element separatorLight() {
  return std::make_shared<SeparatorAuto>(LIGHT);
}

/// @brief Dibuja una separación vertical u horizontal entre otros dos
/// elementos, usando el estilo DISCONTINUO.
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// Añade una separación visual entre dos elementos.
///
/// ### Ejemplo
///
/// ```cpp
/// // Use 'border' as a function...
/// Element document = vbox({
///   text("up"),
///   separatorLight(),
///   text("down"),
/// });
/// ```
///
/// ### Salida
///
/// ```bash
/// up
/// ╍╍╍╍
/// down
/// ```
Element separatorDashed() {
  return std::make_shared<SeparatorAuto>(DASHED);
}

/// @brief Dibuja una separación vertical u horizontal entre otros dos
/// elementos, usando el estilo GRUESO.
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// Añade una separación visual entre dos elementos.
///
/// ### Ejemplo
///
/// ```cpp
/// // Use 'border' as a function...
/// Element document = vbox({
///   text("up"),
///   separatorHeavy(),
///   text("down"),
/// });
/// ```
///
/// ### Salida
///
/// ```bash
/// up
/// ━━━━
/// down
/// ```
Element separatorHeavy() {
  return std::make_shared<SeparatorAuto>(HEAVY);
}

/// @brief Dibuja una separación vertical u horizontal entre otros dos
/// elementos, usando el estilo DOBLE.
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// Añade una separación visual entre dos elementos.
///
/// ### Ejemplo
///
/// ```cpp
/// // Use 'border' as a function...
/// Element document = vbox({
///   text("up"),
///   separatorDouble(),
///   text("down"),
/// });
/// ```
///
/// ### Salida
///
/// ```bash
/// up
/// ════
/// down
/// ```
Element separatorDouble() {
  return std::make_shared<SeparatorAuto>(DOUBLE);
}

/// @brief Dibuja una separación vertical u horizontal entre otros dos
/// elementos, usando el estilo VACÍO.
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// Añade una separación visual entre dos elementos.
///
/// ### Ejemplo
///
/// ```cpp
/// // Use 'border' as a function...
/// Element document = vbox({
///   text("up"),
///   separator(),
///   text("down"),
/// });
/// ```
///
/// ### Salida
///
/// ```bash
/// up
///
/// down
/// ```
Element separatorEmpty() {
  return std::make_shared<SeparatorAuto>(EMPTY);
}

/// @brief Dibuja una separación vertical u horizontal entre otros dos
/// elementos.
/// @param value el carácter para rellenar el área del separador.
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorDouble
/// @see separatorHeavy
/// @see separatorEmpty
/// @see separatorRounded
/// @see separatorStyled
/// @see separatorCharacter
///
/// Añade una separación visual entre dos elementos.
///
/// ### Ejemplo
///
/// ```cpp
/// // Use 'border' as a function...
/// Element document = vbox({
///   text("up"),
///   separator(),
///   text("down"),
/// });
/// ```
///
/// ### Salida
///
/// ```bash
/// up
/// ────
/// down
/// ```
Element separatorCharacter(std::string value) {
  return std::make_shared<Separator>(std::move(value));
}

/// @brief Dibuja un separador entre dos elementos, rellenado con un píxel dado.
/// @ingroup dom
/// @see separator
/// @see separatorLight
/// @see separatorDashed
/// @see separatorHeavy
/// @see separatorDouble
/// @see separatorStyled
///
/// ### Ejemplo
///
/// ```cpp
/// Pixel empty;
/// Element document = vbox({
///   text("Up"),
///   separator(empty),
///   text("Down"),
/// })
/// ```
///
/// ### Salida
///
/// ```bash
/// Up
///
/// Down
/// ```
Element separator(Pixel pixel) {
  return std::make_shared<SeparatorWithPixel>(std::move(pixel));
}

/// @brief Dibuja una barra horizontal, con el área entre izquierda/derecha coloreada
/// de forma diferente.
/// @param left el límite izquierdo del área activa.
/// @param right el límite derecho del área activa.
/// @param selected_color el color del área seleccionada.
/// @param unselected_color el color del área no seleccionada.
///
/// ### Ejemplo
///
/// ```cpp
/// Element document = separatorHSelector(2,5, Color::White, Color::Blue);
/// ```
Element separatorHSelector(float left,
                           float right,
                           Color unselected_color,
                           Color selected_color) {
  class Impl : public Node {
   public:
    Impl(float left, float right, Color selected_color, Color unselected_color)
        : left_(left),
          right_(right),
          unselected_color_(unselected_color),
          selected_color_(selected_color) {}
    void ComputeRequirement() override {
      requirement_.min_x = 1;
      requirement_.min_y = 1;
    }

    void Render(Screen& screen) override {
      if (box_.y_max < box_.y_min) {
        return;
      }

      // This are the two location with an empty demi-cell.
      int demi_cell_left = int(left_ * 2.F - 1.F);    // NOLINT
      int demi_cell_right = int(right_ * 2.F + 2.F);  // NOLINT

      const int y = box_.y_min;
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        Pixel& pixel = screen.PixelAt(x, y);

        const int a = (x - box_.x_min) * 2;
        const int b = a + 1;
        const bool a_empty = demi_cell_left == a || demi_cell_right == a;
        const bool b_empty = demi_cell_left == b || demi_cell_right == b;

        if (!a_empty && !b_empty) {
          pixel.character = "─";
          pixel.automerge = true;
        } else {
          pixel.character = a_empty ? "╶" : "╴";  // NOLINT
          pixel.automerge = false;
        }

        if (demi_cell_left <= a && b <= demi_cell_right) {
          pixel.foreground_color = selected_color_;
        } else {
          pixel.foreground_color = unselected_color_;
        }
      }
    }

    float left_;
    float right_;
    Color unselected_color_;
    Color selected_color_;
  };
  return std::make_shared<Impl>(left, right, unselected_color, selected_color);
}

/// @brief Dibuja una barra vertical, con el área entre arriba/abajo coloreada
/// de forma diferente.
/// @param up el límite superior del área activa.
/// @param down el límite inferior del área activa.
/// @param selected_color el color del área seleccionada.
/// @param unselected_color el color del área no seleccionada.
///
/// ### Ejemplo
///
/// ```cpp
/// Element document = separatorHSelector(2,5, Color::White, Color::Blue);
/// ```
Element separatorVSelector(float up,
                           float down,
                           Color unselected_color,
                           Color selected_color) {
  class Impl : public Node {
   public:
    Impl(float up, float down, Color unselected_color, Color selected_color)
        : up_(up),
          down_(down),
          unselected_color_(unselected_color),
          selected_color_(selected_color) {}
    void ComputeRequirement() override {
      requirement_.min_x = 1;
      requirement_.min_y = 1;
    }

    void Render(Screen& screen) override {
      if (box_.x_max < box_.x_min) {
        return;
      }

      // This are the two location with an empty demi-cell.
      const int demi_cell_up = int(up_ * 2 - 1);
      const int demi_cell_down = int(down_ * 2 + 2);

      const int x = box_.x_min;
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        Pixel& pixel = screen.PixelAt(x, y);

        const int a = (y - box_.y_min) * 2;
        const int b = a + 1;
        const bool a_empty = demi_cell_up == a || demi_cell_down == a;
        const bool b_empty = demi_cell_up == b || demi_cell_down == b;

        if (!a_empty && !b_empty) {
          pixel.character = "│";
          pixel.automerge = true;
        } else {
          pixel.character = a_empty ? "╷" : "╵";  // NOLINT
          pixel.automerge = false;
        }

        if (demi_cell_up <= a && b <= demi_cell_down) {
          pixel.foreground_color = selected_color_;
        } else {
          pixel.foreground_color = unselected_color_;
        }
      }
    }

    float up_;
    float down_;
    Color unselected_color_;
    Color selected_color_;
  };
  return std::make_shared<Impl>(up, down, unselected_color, selected_color);
}

}  // namespace ftxui