// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <memory>   // for make_shared
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Element, Decorator, bgcolor, color
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/box.hpp"          // for Box
#include "ftxui/screen/color.hpp"        // for Color
#include "ftxui/screen/screen.hpp"       // for Pixel, Screen

namespace ftxui {

namespace {
class BgColor : public NodeDecorator {
 public:
  BgColor(Element child, Color color)
      : NodeDecorator(std::move(child)), color_(color) {}

  void Render(Screen& screen) override {
    if (color_.IsOpaque()) {
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        for (int x = box_.x_min; x <= box_.x_max; ++x) {
          screen.PixelAt(x, y).background_color = color_;
        }
      }
    } else {
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        for (int x = box_.x_min; x <= box_.x_max; ++x) {
          Color& color = screen.PixelAt(x, y).background_color;
          color = Color::Blend(color, color_);
        }
      }
    }
    NodeDecorator::Render(screen);
  }

  Color color_;
};

class FgColor : public NodeDecorator {
 public:
  FgColor(Element child, Color color)
      : NodeDecorator(std::move(child)), color_(color) {}

  void Render(Screen& screen) override {
    if (color_.IsOpaque()) {
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        for (int x = box_.x_min; x <= box_.x_max; ++x) {
          screen.PixelAt(x, y).foreground_color = color_;
        }
      }
    } else {
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        for (int x = box_.x_min; x <= box_.x_max; ++x) {
          Color& color = screen.PixelAt(x, y).foreground_color;
          color = Color::Blend(color, color_);
        }
      }
    }
    NodeDecorator::Render(screen);
  }

  Color color_;
};

}  // namespace

/// @brief Définit la couleur de premier plan d'un élément.
/// @param color La couleur de l'élément de sortie.
/// @param child L'élément d'entrée.
/// @return L'élément de sortie coloré.
/// @ingroup dom
///
/// ### Exemple
///
/// ```cpp
/// Element document = color(Color::Green, text("Success")),
/// ```
Element color(Color color, Element child) {
  return std::make_shared<FgColor>(std::move(child), color);
}

/// @brief Définit la couleur d'arrière-plan d'un élément.
/// @param color La couleur de l'élément de sortie.
/// @param child L'élément d'entrée.
/// @return L'élément de sortie coloré.
/// @ingroup dom
///
/// ### Exemple
///
/// ```cpp
/// Element document = bgcolor(Color::Green, text("Success")),
/// ```
Element bgcolor(Color color, Element child) {
  return std::make_shared<BgColor>(std::move(child), color);
}

/// @brief Décore en utilisant une couleur de premier plan.
/// @param c La couleur de premier plan à appliquer.
/// @return Le Decorator appliquant la couleur.
/// @ingroup dom
///
/// ### Exemple
///
/// ```cpp
/// Element document = text("red") | color(Color::Red);
/// ```
Decorator color(Color c) {
  return [c](Element child) { return color(c, std::move(child)); };
}

/// @brief Décore en utilisant une couleur d'arrière-plan.
/// @param color La couleur d'arrière-plan à appliquer.
/// @return Le Decorator appliquant la couleur.
/// @ingroup dom
///
/// ### Exemple
///
/// ```cpp
/// Element document = text("red") | bgcolor(Color::Red);
/// ```
Decorator bgcolor(Color color) {
  return [color](Element child) { return bgcolor(color, std::move(child)); };
}

}  // namespace ftxui
