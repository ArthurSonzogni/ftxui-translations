// Copyright 2024 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <functional>  // for function
#include <memory>      // for make_shared
#include <utility>     // for move

#include "ftxui/dom/elements.hpp"  // for Element, Decorator, bgcolor, color
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/color.hpp"        // for Color
#include "ftxui/screen/pixel.hpp"        // for Pixel
#include "ftxui/screen/screen.hpp"       // for Screen

namespace ftxui {

namespace {

class SelectionStyleReset : public NodeDecorator {
 public:
  explicit SelectionStyleReset(Element child)
      : NodeDecorator(std::move(child)) {}

  void Render(Screen& screen) final {
    auto old_style = screen.GetSelectionStyle();
    screen.SetSelectionStyle([](Pixel&) {});
    NodeDecorator::Render(screen);
    screen.SetSelectionStyle(old_style);
  }
};

class SelectionStyle : public NodeDecorator {
 public:
  SelectionStyle(Element child, const std::function<void(Pixel&)>& style)
      : NodeDecorator(std::move(child)), style_(style) {}

  void Render(Screen& screen) final {
    auto old_style = screen.GetSelectionStyle();
    auto new_style = [&, old_style](Pixel& pixel) {
      old_style(pixel);
      style_(pixel);
    };
    screen.SetSelectionStyle(new_style);
    NodeDecorator::Render(screen);
    screen.SetSelectionStyle(old_style);
  }

  std::function<void(Pixel&)> style_;
};

}  // namespace

/// @brief Restablece el estilo de selección de un elemento.
/// @param child El elemento de entrada.
/// @return El elemento de salida con el estilo de selección restablecido.
Element selectionStyleReset(Element child) {
  return std::make_shared<SelectionStyleReset>(std::move(child));
}

/// @brief Establece el color de fondo de un elemento cuando está seleccionado.
/// Tenga en cuenta que el estilo se aplica además del estilo existente.
Decorator selectionBackgroundColor(Color foreground) {
  return selectionStyle([foreground](Pixel& pixel) {  //
    pixel.background_color = foreground;
  });
}

/// @brief Establece el color de primer plano de un elemento cuando está seleccionado.
/// Tenga en cuenta que el estilo se aplica además del estilo existente.
Decorator selectionForegroundColor(Color foreground) {
  return selectionStyle([foreground](Pixel& pixel) {  //
    pixel.foreground_color = foreground;
  });
}

/// @brief Establece el color de un elemento cuando está seleccionado.
/// @param foreground El color a aplicar.
/// Tenga en cuenta que el estilo se aplica además del estilo existente.
Decorator selectionColor(Color foreground) {
  return selectionForegroundColor(foreground);
}

/// @brief Establece el estilo de un elemento cuando está seleccionado.
/// @param style El estilo a aplicar.
/// Tenga en cuenta que el estilo se aplica además del estilo existente.
// NOLINTNEXTLINE
Decorator selectionStyle(std::function<void(Pixel&)> style) {
  return [style](Element child) -> Element {
    return std::make_shared<SelectionStyle>(std::move(child), style);
  };
}

}  // namespace ftxui