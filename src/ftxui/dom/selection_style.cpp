// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <functional>  // for function
#include <memory>      // for make_shared
#include <utility>     // for move

#include "ftxui/dom/elements.hpp"  // for Element, Decorator, bgcolor, color
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/cell.hpp"         // for Cell
#include "ftxui/screen/color.hpp"        // for Color
#include "ftxui/screen/screen.hpp"       // for Screen

namespace ftxui {

namespace {

class SelectionStyleReset : public NodeDecorator {
 public:
  explicit SelectionStyleReset(Element child)
      : NodeDecorator(std::move(child)) {}

  void Render(Screen& screen) final {
    auto old_style = screen.GetSelectionStyle();
    screen.SetSelectionStyle([](Cell&) {});
    NodeDecorator::Render(screen);
    screen.SetSelectionStyle(old_style);
  }
};

class SelectionStyle : public NodeDecorator {
 public:
  SelectionStyle(Element child, const std::function<void(Cell&)>& style)
      : NodeDecorator(std::move(child)), style_(style) {}

  void Render(Screen& screen) final {
    auto old_style = screen.GetSelectionStyle();
    auto new_style = [&, old_style](Cell& pixel) {
      old_style(pixel);
      style_(pixel);
    };
    screen.SetSelectionStyle(new_style);
    NodeDecorator::Render(screen);
    screen.SetSelectionStyle(old_style);
  }

  std::function<void(Cell&)> style_;
};

}  // namespace

/// @brief Réinitialise le style de sélection d'un élément.
/// @param child L'élément d'entrée.
/// @return L'élément de sortie avec le style de sélection réinitialisé.
Element selectionStyleReset(Element child) {
  return std::make_shared<SelectionStyleReset>(std::move(child));
}

/// @brief Définit la couleur d'arrière-plan d'un élément lorsqu'il est sélectionné.
/// Notez que le style est appliqué en plus du style existant.
Decorator selectionBackgroundColor(Color foreground) {
  return selectionStyle([foreground](Cell& pixel) {  //
    pixel.background_color = foreground;
  });
}

/// @brief Définit la couleur de premier plan d'un élément lorsqu'il est sélectionné.
/// Notez que le style est appliqué en plus du style existant.
Decorator selectionForegroundColor(Color foreground) {
  return selectionStyle([foreground](Cell& pixel) {  //
    pixel.foreground_color = foreground;
  });
}

/// @brief Définit la couleur d'un élément lorsqu'il est sélectionné.
/// @param foreground La couleur à appliquer.
/// Notez que le style est appliqué en plus du style existant.
Decorator selectionColor(Color foreground) {
  return selectionForegroundColor(foreground);
}

/// @brief Définit le style d'un élément lorsqu'il est sélectionné.
/// @param style Le style à appliquer.
/// Notez que le style est appliqué en plus du style existant.
// NOLINTNEXTLINE
Decorator selectionStyle(std::function<void(Cell&)> style) {
  return [style](Element child) -> Element {
    return std::make_shared<SelectionStyle>(std::move(child), style);
  };
}

}  // namespace ftxui
