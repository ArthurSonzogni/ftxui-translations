// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>               // for max
#include <array>                   // for array
#include <ftxui/screen/color.hpp>  // for Color
#include <memory>    // for allocator, make_shared, __shared_ptr_access
#include <optional>  // for optional, nullopt
#include <string>    // for basic_string, string
#include <utility>   // for move

#include "ftxui/dom/elements.hpp"  // for unpack, Element, Decorator, BorderStyle, ROUNDED, borderStyled, Elements, DASHED, DOUBLE, EMPTY, HEAVY, LIGHT, border, borderDashed, borderDouble, borderEmpty, borderHeavy, borderLight, borderRounded, borderWith, window
#include "ftxui/dom/node.hpp"      // for Node, Elements
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/cell.hpp"      // for Cell
#include "ftxui/screen/screen.hpp"    // for Cell, Screen

namespace ftxui {

namespace {
using Charset = std::array<std::string, 6>;  // NOLINT
using Charsets = std::array<Charset, 6>;     // NOLINT
// NOLINTNEXTLINE
static Charsets simple_border_charset = {
    Charset{"┌", "┐", "└", "┘", "─", "│"},  // LIGERO
    Charset{"┏", "┓", "┗", "┛", "╍", "╏"},  // DISCONTINUO
    Charset{"┏", "┓", "┗", "┛", "━", "┃"},  // GRUESO
    Charset{"╔", "╗", "╚", "╝", "═", "║"},  // DOBLE
    Charset{"╭", "╮", "╰", "╯", "─", "│"},  // REDONDEADO
    Charset{" ", " ", " ", " ", " ", " "},  // VACÍO
};

// For reference, here is the charset for normal border:
// Para referencia, aquí está el conjunto de caracteres para un borde normal:
class Border : public Node {
 public:
  Border(Elements children,
         BorderStyle style,
         std::optional<Color> foreground_color = std::nullopt)
      : Node(std::move(children)),
        charset_(simple_border_charset[style])  // NOLINT
        ,
        foreground_color_(foreground_color) {}  // NOLINT

  const Charset& charset_;  // NOLINT
  std::optional<Color> foreground_color_;

  void ComputeRequirement() override {
    Node::ComputeRequirement();
    requirement_ = children_[0]->requirement();
    requirement_.min_x += 2;
    requirement_.min_y += 2;
    if (children_.size() == 2) {
      requirement_.min_x =
          std::max(requirement_.min_x, children_[1]->requirement().min_x + 2);
    }
    requirement_.focused.box.x_min++;
    requirement_.focused.box.x_max++;
    requirement_.focused.box.y_min++;
    requirement_.focused.box.y_max++;
  }

  void SetBox(Box box) override {
    Node::SetBox(box);
    if (children_.size() == 2) {
      Box title_box;
      title_box.x_min = box.x_min + 1;
      title_box.x_max = std::min(box.x_max - 1,
                                 box.x_min + children_[1]->requirement().min_x);
      title_box.y_min = box.y_min;
      title_box.y_max = box.y_min;
      children_[1]->SetBox(title_box);
    }
    box.x_min++;
    box.x_max--;
    box.y_min++;
    box.y_max--;
    children_[0]->SetBox(box);
  }

  void Render(Screen& screen) override {
    // Draw content.
    // Dibujar contenido.
    children_[0]->Render(screen);

    // Draw the border.
    // Dibujar el borde.
    if (box_.x_min >= box_.x_max || box_.y_min >= box_.y_max) {
      return;
    }

    screen.at(box_.x_min, box_.y_min) = charset_[0];  // NOLINT
    screen.at(box_.x_max, box_.y_min) = charset_[1];  // NOLINT
    screen.at(box_.x_min, box_.y_max) = charset_[2];  // NOLINT
    screen.at(box_.x_max, box_.y_max) = charset_[3];  // NOLINT

    for (int x = box_.x_min + 1; x < box_.x_max; ++x) {
      Cell& p1 = screen.CellAt(x, box_.y_min);
      Cell& p2 = screen.CellAt(x, box_.y_max);
      p1.character = charset_[4];  // NOLINT
      p2.character = charset_[4];  // NOLINT
      p1.automerge = true;
      p2.automerge = true;
    }
    for (int y = box_.y_min + 1; y < box_.y_max; ++y) {
      Cell& p3 = screen.CellAt(box_.x_min, y);
      Cell& p4 = screen.CellAt(box_.x_max, y);
      p3.character = charset_[5];  // NOLINT
      p4.character = charset_[5];  // NOLINT
      p3.automerge = true;
      p4.automerge = true;
    }

    // Draw title.
    // Dibujar título.
    if (children_.size() == 2) {
      children_[1]->Render(screen);
    }

    // Draw the border color.
    // Dibujar el color del borde.
    if (foreground_color_) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        screen.CellAt(x, box_.y_min).foreground_color = *foreground_color_;
        screen.CellAt(x, box_.y_max).foreground_color = *foreground_color_;
      }
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        screen.CellAt(box_.x_min, y).foreground_color = *foreground_color_;
        screen.CellAt(box_.x_max, y).foreground_color = *foreground_color_;
      }
    }
  }
};

// For reference, here is the charset for normal border:
// Para referencia, aquí está el conjunto de caracteres para un borde normal:
class BorderCell : public Node {
 public:
  BorderCell(Elements children, Cell pixel)
      : Node(std::move(children)), pixel_(std::move(pixel)) {}

 private:
  Cell pixel_;

  void ComputeRequirement() override {
    Node::ComputeRequirement();
    requirement_ = children_[0]->requirement();
    requirement_.min_x += 2;
    requirement_.min_y += 2;
    if (children_.size() == 2) {
      requirement_.min_x =
          std::max(requirement_.min_x, children_[1]->requirement().min_x + 2);
    }

    requirement_.focused.box.Shift(1, 1);
  }

  void SetBox(Box box) override {
    Node::SetBox(box);
    if (children_.size() == 2) {
      Box title_box;
      title_box.x_min = box.x_min + 1;
      title_box.x_max = box.x_max - 1;
      title_box.y_min = box.y_min;
      title_box.y_max = box.y_min;
      children_[1]->SetBox(title_box);
    }
    box.x_min++;
    box.x_max--;
    box.y_min++;
    box.y_max--;
    children_[0]->SetBox(box);
  }

  void Render(Screen& screen) override {
    // Draw content.
    // Dibujar contenido.
    children_[0]->Render(screen);

    // Draw the border.
    // Dibujar el borde.
    if (box_.x_min >= box_.x_max || box_.y_min >= box_.y_max) {
      return;
    }

    screen.CellAt(box_.x_min, box_.y_min) = pixel_;
    screen.CellAt(box_.x_max, box_.y_min) = pixel_;
    screen.CellAt(box_.x_min, box_.y_max) = pixel_;
    screen.CellAt(box_.x_max, box_.y_max) = pixel_;

    for (int x = box_.x_min + 1; x < box_.x_max; ++x) {
      screen.CellAt(x, box_.y_min) = pixel_;
      screen.CellAt(x, box_.y_max) = pixel_;
    }
    for (int y = box_.y_min + 1; y < box_.y_max; ++y) {
      screen.CellAt(box_.x_min, y) = pixel_;
      screen.CellAt(box_.x_max, y) = pixel_;
    }
  }
};
}  // namespace

/// @brief Dibuja un borde alrededor del elemento.
/// @ingroup dom
/// @see border
/// @see borderLight
/// @see borderDashed
/// @see borderDouble
/// @see borderHeavy
/// @see borderEmpty
/// @see borderRounded
/// @see borderStyled
/// @see borderWith
///
/// Agrega un borde alrededor de un elemento
///
/// ### Ejemplo
///
/// ```cpp
/// // Usa 'border' como una función...
/// Element document = border(text("The element"));
///
/// // ...O como una 'tubería' (pipe).
/// Element document = text("The element") | border;
/// ```
///
/// ### Salida
///
/// ```bash
/// ┌───────────┐
/// │The element│
/// └───────────┘
/// ```
Element border(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), ROUNDED);
}

/// @brief Igual que border pero con un Cell constante alrededor del elemento.
/// @ingroup dom
/// @see border
Decorator borderWith(const Cell& pixel) {
  return [pixel](Element child) {
    return std::make_shared<BorderCell>(unpack(std::move(child)), pixel);
  };
}

/// @brief Igual que border pero con estilos diferentes.
/// @ingroup dom
/// @see border
Decorator borderStyled(BorderStyle style) {
  return [style](Element child) {
    return std::make_shared<Border>(unpack(std::move(child)), style);
  };
}

/// @brief Igual que border pero con un color de primer plano.
/// @ingroup dom
/// @see border
Decorator borderStyled(Color foreground_color) {
  return [foreground_color](Element child) {
    return std::make_shared<Border>(unpack(std::move(child)), ROUNDED,
                                    foreground_color);
  };
}

/// @brief Igual que border pero con un color de primer plano y un estilo diferente
/// @ingroup dom
/// @see border
Decorator borderStyled(BorderStyle style, Color foreground_color) {
  return [style, foreground_color](Element child) {
    return std::make_shared<Border>(unpack(std::move(child)), style,
                                    foreground_color);
  };
}

/// @brief Dibuja un borde discontinuo alrededor del elemento.
/// @ingroup dom
/// @see border
/// @see borderLight
/// @see borderDashed
/// @see borderDouble
/// @see borderHeavy
/// @see borderRounded
/// @see borderEmpty
/// @see borderStyled
/// @see borderWith
///
/// Agrega un borde alrededor de un elemento
///
/// ### Ejemplo
///
/// ```cpp
/// // Usa 'borderDash' como una función...
/// Element document = borderDash(text("The element"));
///
/// // ...O como una 'tubería' (pipe).
/// Element document = text("The element") | borderDAsh;
/// ```
///
/// ### Salida
///
/// ```bash
/// ┏╍╍╍╍╍╍╍╍╍╍╍╍╍╍┓
/// ╏The element   ╏
/// ┗╍╍╍╍╍╍╍╍╍╍╍╍╍╍┛
/// ```
Element borderDashed(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), DASHED);
}

/// @brief Dibuja un borde ligero alrededor del elemento.
/// @ingroup dom
/// @see border
/// @see borderLight
/// @see borderDashed
/// @see borderDouble
/// @see borderHeavy
/// @see borderRounded
/// @see borderEmpty
/// @see borderStyled
/// @see borderWith
///
/// Agrega un borde alrededor de un elemento
///
/// ### Ejemplo
///
/// ```cpp
/// // Usa 'borderLight' como una función...
/// Element document = borderLight(text("The element"));
///
/// // ...O como una 'tubería' (pipe).
/// Element document = text("The element") | borderLight;
/// ```
///
/// ### Salida
///
/// ```bash
/// ┌──────────────┐
/// │The element   │
/// └──────────────┘
/// ```
Element borderLight(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), LIGHT);
}

/// @brief Dibuja un borde grueso alrededor del elemento.
/// @ingroup dom
/// @see border
/// @see borderLight
/// @see borderDashed
/// @see borderDouble
/// @see borderHeavy
/// @see borderRounded
/// @see borderEmpty
/// @see borderStyled
/// @see borderWith
///
/// Agrega un borde alrededor de un elemento
///
/// ### Ejemplo
///
/// ```cpp
/// // Usa 'borderHeavy' como una función...
/// Element document = borderHeavy(text("The element"));
///
/// // ...O como una 'tubería' (pipe).
/// Element document = text("The element") | borderHeavy;
/// ```
///
/// ### Salida
///
/// ```bash
/// ┏━━━━━━━━━━━━━━┓
/// ┃The element   ┃
/// ┗━━━━━━━━━━━━━━┛
/// ```
Element borderHeavy(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), HEAVY);
}

/// @brief Dibuja un borde doble alrededor del elemento.
/// @ingroup dom
/// @see border
/// @see borderLight
/// @see borderDashed
/// @see borderDouble
/// @see borderHeavy
/// @see borderRounded
/// @see borderEmpty
/// @see borderStyled
/// @see borderWith
///
/// Agrega un borde alrededor de un elemento
///
/// ### Ejemplo
///
/// ```cpp
/// // Usa 'borderDouble' como una función...
/// Element document = borderDouble(text("The element"));
///
/// // ...O como una 'tubería' (pipe).
/// Element document = text("The element") | borderDouble;
/// ```
///
/// ### Salida
///
/// ```bash
/// ╔══════════════╗
/// ║The element   ║
/// ╚══════════════╝
/// ```
Element borderDouble(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), DOUBLE);
}

/// @brief Dibuja un borde redondeado alrededor del elemento.
/// @ingroup dom
/// @see border
/// @see borderLight
/// @see borderDashed
/// @see borderDouble
/// @see borderHeavy
/// @see borderRounded
/// @see borderEmpty
/// @see borderStyled
/// @see borderWith
///
/// Agrega un borde alrededor de un elemento
///
/// ### Ejemplo
///
/// ```cpp
/// // Usa 'borderRounded' como una función...
/// Element document = borderRounded(text("The element"));
///
/// // ...O como una 'tubería' (pipe).
/// Element document = text("The element") | borderRounded;
/// ```
///
/// ### Salida
///
/// ```bash
/// ╭──────────────╮
/// │The element   │
/// ╰──────────────╯
/// ```
Element borderRounded(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), ROUNDED);
}

/// @brief Dibuja un borde vacío alrededor del elemento.
/// @ingroup dom
/// @see border
/// @see borderLight
/// @see borderDashed
/// @see borderDouble
/// @see borderHeavy
/// @see borderRounded
/// @see borderEmpty
/// @see borderStyled
/// @see borderWith
///
/// Agrega un borde alrededor de un elemento
///
/// ### Ejemplo
///
/// ```cpp
/// // Usa 'borderRounded' como una función...
/// Element document = borderRounded(text("The element"));
///
/// // ...O como una 'tubería' (pipe).
/// Element document = text("The element") | borderRounded;
/// ```
///
/// ### Salida
///
/// ```bash
///
///  The element
///
/// ```
Element borderEmpty(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), EMPTY);
}

/// @brief Dibuja una ventana con un título y un borde alrededor del elemento.
/// @param title El título de la ventana.
/// @param content El elemento a envolver.
/// @param border El estilo del borde. Por defecto es ROUNDED.
/// @ingroup dom
/// @see border
///
/// ### Ejemplo
///
/// ```cpp
/// Element document = window(text("Title"),
///                           text("content")
///                    );
///
/// // Especificando el borde
/// Element document = window(text("Title"),
///                           text("content"),
///                           ROUNDED
///                    );
/// ```
///
/// ### Salida
///
/// ```bash
/// ┌Title──┐
/// │content│
/// └───────┘
/// ```
Element window(Element title, Element content, BorderStyle border) {
  return std::make_shared<Border>(unpack(std::move(content), std::move(title)),
                                  border);
}
}  // namespace ftxui
