// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
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
#include "ftxui/screen/pixel.hpp"     // for Pixel
#include "ftxui/screen/screen.hpp"    // for Pixel, Screen

namespace ftxui {

namespace {
using Charset = std::array<std::string, 6>;  // NOLINT
using Charsets = std::array<Charset, 6>;     // NOLINT
// NOLINTNEXTLINE
static Charsets simple_border_charset = {
    Charset{"┌", "┐", "└", "┘", "─", "│"},  // LIGHT
    Charset{"┏", "┓", "┗", "┛", "╍", "╏"},  // DASHED
    Charset{"┏", "┓", "┗", "┛", "━", "┃"},  // HEAVY
    Charset{"╔", "╗", "╚", "╝", "═", "║"},  // DOUBLE
    Charset{"╭", "╮", "╰", "╯", "─", "│"},  // ROUNDED
    Charset{" ", " ", " ", " ", " ", " "},  // EMPTY
};

// Pour référence, voici le jeu de caractères pour une bordure normale :
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
    children_[0]->Render(screen);

    // Draw the border.
    if (box_.x_min >= box_.x_max || box_.y_min >= box_.y_max) {
      return;
    }

    screen.at(box_.x_min, box_.y_min) = charset_[0];  // NOLINT
    screen.at(box_.x_max, box_.y_min) = charset_[1];  // NOLINT
    screen.at(box_.x_min, box_.y_max) = charset_[2];  // NOLINT
    screen.at(box_.x_max, box_.y_max) = charset_[3];  // NOLINT

    for (int x = box_.x_min + 1; x < box_.x_max; ++x) {
      Pixel& p1 = screen.PixelAt(x, box_.y_min);
      Pixel& p2 = screen.PixelAt(x, box_.y_max);
      p1.character = charset_[4];  // NOLINT
      p2.character = charset_[4];  // NOLINT
      p1.automerge = true;
      p2.automerge = true;
    }
    for (int y = box_.y_min + 1; y < box_.y_max; ++y) {
      Pixel& p3 = screen.PixelAt(box_.x_min, y);
      Pixel& p4 = screen.PixelAt(box_.x_max, y);
      p3.character = charset_[5];  // NOLINT
      p4.character = charset_[5];  // NOLINT
      p3.automerge = true;
      p4.automerge = true;
    }

    // Draw title.
    if (children_.size() == 2) {
      children_[1]->Render(screen);
    }

    // Draw the border color.
    if (foreground_color_) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        screen.PixelAt(x, box_.y_min).foreground_color = *foreground_color_;
        screen.PixelAt(x, box_.y_max).foreground_color = *foreground_color_;
      }
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        screen.PixelAt(box_.x_min, y).foreground_color = *foreground_color_;
        screen.PixelAt(box_.x_max, y).foreground_color = *foreground_color_;
      }
    }
  }
};

// Pour référence, voici le jeu de caractères pour une bordure normale :
class BorderPixel : public Node {
 public:
  BorderPixel(Elements children, Pixel pixel)
      : Node(std::move(children)), pixel_(std::move(pixel)) {}

 private:
  Pixel pixel_;

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
    children_[0]->Render(screen);

    // Draw the border.
    if (box_.x_min >= box_.x_max || box_.y_min >= box_.y_max) {
      return;
    }

    screen.PixelAt(box_.x_min, box_.y_min) = pixel_;
    screen.PixelAt(box_.x_max, box_.y_min) = pixel_;
    screen.PixelAt(box_.x_min, box_.y_max) = pixel_;
    screen.PixelAt(box_.x_max, box_.y_max) = pixel_;

    for (int x = box_.x_min + 1; x < box_.x_max; ++x) {
      screen.PixelAt(x, box_.y_min) = pixel_;
      screen.PixelAt(x, box_.y_max) = pixel_;
    }
    for (int y = box_.y_min + 1; y < box_.y_max; ++y) {
      screen.PixelAt(box_.x_min, y) = pixel_;
      screen.PixelAt(box_.x_max, y) = pixel_;
    }
  }
};
}  // namespace

/// @brief Dessine une bordure autour de l'élément.
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
/// Ajoute une bordure autour d'un élément
///
/// ### Exemple
///
/// ```cpp
/// // Utilisation de 'border' comme une fonction...
/// Element document = border(text("The element"));
///
/// // ...Ou comme un 'pipe'.
/// Element document = text("The element") | border;
/// ```
///
/// ### Sortie
///
/// ```bash
/// ┌───────────┐
/// │The element│
/// └───────────┘
/// ```
Element border(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), ROUNDED);
}

/// @brief Identique à border mais avec un Pixel constant autour de l'élément.
/// @ingroup dom
/// @see border
Decorator borderWith(const Pixel& pixel) {
  return [pixel](Element child) {
    return std::make_shared<BorderPixel>(unpack(std::move(child)), pixel);
  };
}

/// @brief Identique à border mais avec différents styles.
/// @ingroup dom
/// @see border
Decorator borderStyled(BorderStyle style) {
  return [style](Element child) {
    return std::make_shared<Border>(unpack(std::move(child)), style);
  };
}

/// @brief Identique à border mais avec une couleur de premier plan.
/// @ingroup dom
/// @see border
Decorator borderStyled(Color foreground_color) {
  return [foreground_color](Element child) {
    return std::make_shared<Border>(unpack(std::move(child)), ROUNDED,
                                    foreground_color);
  };
}

/// @brief Identique à border mais avec une couleur de premier plan et un style différent
/// @ingroup dom
/// @see border
Decorator borderStyled(BorderStyle style, Color foreground_color) {
  return [style, foreground_color](Element child) {
    return std::make_shared<Border>(unpack(std::move(child)), style,
                                    foreground_color);
  };
}

/// @brief Dessine une bordure en pointillés autour de l'élément.
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
/// Ajoute une bordure autour d'un élément
///
/// ### Exemple
///
/// ```cpp
/// // Utilisation de 'borderDash' comme une fonction...
/// Element document = borderDashed(text("The element"));
///
/// // ...Ou comme un 'pipe'.
/// Element document = text("The element") | borderDashed;
/// ```
///
/// ### Sortie
///
/// ```bash
/// ┏╍╍╍╍╍╍╍╍╍╍╍╍╍╍┓
/// ╏The element   ╏
/// ┗╍╍╍╍╍╍╍╍╍╍╍╍╍╍┛
/// ```
Element borderDashed(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), DASHED);
}

/// @brief Dessine une bordure fine autour de l'élément.
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
/// Ajoute une bordure autour d'un élément
///
/// ### Exemple
///
/// ```cpp
/// // Utilisation de 'borderLight' comme une fonction...
/// Element document = borderLight(text("The element"));
///
/// // ...Ou comme un 'pipe'.
/// Element document = text("The element") | borderLight;
/// ```
///
/// ### Sortie
///
/// ```bash
/// ┌──────────────┐
/// │The element   │
/// └──────────────┘
/// ```
Element borderLight(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), LIGHT);
}

/// @brief Dessine une bordure épaisse autour de l'élément.
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
/// Ajoute une bordure autour d'un élément
///
/// ### Exemple
///
/// ```cpp
/// // Utilisation de 'borderHeavy' comme une fonction...
/// Element document = borderHeavy(text("The element"));
///
/// // ...Ou comme un 'pipe'.
/// Element document = text("The element") | borderHeavy;
/// ```
///
/// ### Sortie
///
/// ```bash
/// ┏━━━━━━━━━━━━━━┓
/// ┃The element   ┃
/// ┗━━━━━━━━━━━━━━┛
/// ```
Element borderHeavy(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), HEAVY);
}

/// @brief Dessine une double bordure autour de l'élément.
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
/// Ajoute une bordure autour d'un élément
///
/// ### Exemple
///
/// ```cpp
/// // Utilisation de 'borderDouble' comme une fonction...
/// Element document = borderDouble(text("The element"));
///
/// // ...Ou comme un 'pipe'.
/// Element document = text("The element") | borderDouble;
/// ```
///
/// ### Sortie
///
/// ```bash
/// ╔══════════════╗
/// ║The element   ║
/// ╚══════════════╝
/// ```
Element borderDouble(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), DOUBLE);
}

/// @brief Dessine une bordure arrondie autour de l'élément.
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
/// Ajoute une bordure autour d'un élément
///
/// ### Exemple
///
/// ```cpp
/// // Utilisation de 'borderRounded' comme une fonction...
/// Element document = borderRounded(text("The element"));
///
/// // ...Ou comme un 'pipe'.
/// Element document = text("The element") | borderRounded;
/// ```
///
/// ### Sortie
///
/// ```bash
/// ╭──────────────╮
/// │The element   │
/// ╰──────────────╯
/// ```
Element borderRounded(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), ROUNDED);
}

/// @brief Dessine une bordure vide autour de l'élément.
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
/// Ajoute une bordure autour d'un élément
///
/// ### Exemple
///
/// ```cpp
/// // Utilisation de 'borderEmpty' comme une fonction...
/// Element document = borderEmpty(text("The element"));
///
/// // ...Ou comme un 'pipe'.
/// Element document = text("The element") | borderEmpty;
/// ```
///
/// ### Sortie
///
/// ```bash
///
///  The element
///
/// ```
Element borderEmpty(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), EMPTY);
}

/// @brief Dessine une fenêtre avec un titre et une bordure autour de l'élément.
/// @param title Le titre de la fenêtre.
/// @param content L'élément à envelopper.
/// @param border Le style de la bordure. La valeur par défaut est ROUNDED.
/// @ingroup dom
/// @see border
///
/// ### Exemple
///
/// ```cpp
/// Element document = window(text("Title"),
///                           text("content")
///                    );
///
/// // En spécifiant la bordure
/// Element document = window(text("Title"),
///                           text("content"),
///                           ROUNDED
///                    );
/// ```
///
/// ### Sortie
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
