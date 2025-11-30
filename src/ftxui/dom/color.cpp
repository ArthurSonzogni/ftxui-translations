// Copyright 2020 Arthur Sonzogni. All rights reserved.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <memory>   // para make_shared
#include <utility>  // para move

#include "ftxui/dom/elements.hpp"  // para Element, Decorator, bgcolor, color
#include "ftxui/dom/node_decorator.hpp"  // para NodeDecorator
#include "ftxui/screen/box.hpp"          // para Box
#include "ftxui/screen/color.hpp"        // para Color
#include "ftxui/screen/screen.hpp"       // para Pixel, Screen

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

/// @brief Establece el color de primer plano de un elemento.
/// @param color El color del elemento de salida.
/// @param child El elemento de entrada.
/// @return El elemento de salida coloreado.
/// @ingroup dom
///
/// ### Ejemplo
///
/// ```cpp
/// Element document = color(Color::Green, text("Success")),
/// ```
Element color(Color color, Element child) {
  return std::make_shared<FgColor>(std::move(child), color);
}

/// @brief Establece el color de fondo de un elemento.
/// @param color El color del elemento de salida.
/// @param child El elemento de entrada.
/// @return El elemento de salida coloreado.
/// @ingroup dom
///
/// ### Ejemplo
///
/// ```cpp
/// Element document = bgcolor(Color::Green, text("Success")),
/// ```
Element bgcolor(Color color, Element child) {
  return std::make_shared<BgColor>(std::move(child), color);
}

/// @brief Decora usando un color de primer plano.
/// @param c El color de primer plano a aplicar.
/// @return El Decorator que aplica el color.
/// @ingroup dom
///
/// ### Ejemplo
///
/// ```cpp
/// Element document = text("red") | color(Color::Red);
/// ```
Decorator color(Color c) {
  return [c](Element child) { return color(c, std::move(child)); };
}

/// @brief Decora usando un color de fondo.
/// @param color El color de fondo a aplicar.
/// @return El Decorator que aplica el color.
/// @ingroup dom
///
/// ### Ejemplo
///
/// ```cpp
/// Element document = text("red") | bgcolor(Color::Red);
/// ```
Decorator bgcolor(Color color) {
  return [color](Element child) { return bgcolor(color, std::move(child)); };
}

}  // namespace ftxui
