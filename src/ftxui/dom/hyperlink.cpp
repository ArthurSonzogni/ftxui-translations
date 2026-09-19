// Copyright 2023 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <cstdint>  // for uint8_t
#include <memory>   // for make_shared
#include <string>   // for string
#include <string_view>
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"        // for Element, Decorator, hyperlink
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/box.hpp"          // for Box
#include "ftxui/screen/screen.hpp"       // for Screen, Cell

namespace ftxui {

namespace {
class Hyperlink : public NodeDecorator {
 public:
  Hyperlink(Element child, std::string link)
      : NodeDecorator(std::move(child)), link_(std::move(link)) {}

  void Render(Screen& screen) override {
    const uint8_t hyperlink_id = screen.RegisterHyperlink(link_);
    for (int y = box_.y_min; y <= box_.y_max; ++y) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        screen.CellAt(x, y).hyperlink = hyperlink_id;
      }
    }
    NodeDecorator::Render(screen);
  }

  std::string link_;
};
}  // namespace

/// @brief Hace que el área renderizada sea clicable usando un navegador web.
///        El enlace se abrirá cuando el usuario haga clic en él.
///        Esto solo es compatible con un conjunto limitado de emuladores de terminal.
///        Lista: https://github.com/Alhadis/OSC8-Adoption/
/// @param link El enlace
/// @param child El elemento de entrada.
/// @return El elemento de salida con el enlace.
/// @ingroup dom
///
/// ### Ejemplo
///
/// ```cpp
/// Element document =
///   hyperlink("https://github.com/ArthurSonzogni/FTXUI", "link");
/// ```
Element hyperlink(std::string_view link, Element child) {
  return std::make_shared<Hyperlink>(std::move(child), std::string(link));
}

/// @brief Decora usando un hipervínculo.
///        El enlace se abrirá cuando el usuario haga clic en él.
///        Esto solo es compatible con un conjunto limitado de emuladores de terminal.
///        Lista: https://github.com/Alhadis/OSC8-Adoption/
/// @param link El enlace al que se redirigirá a los usuarios.
/// @return El Decorator que aplica el hipervínculo.
/// @ingroup dom
///
/// ### Ejemplo
///
/// ```cpp
/// Element document =
///   text("red") | hyperlink("https://github.com/Arthursonzogni/FTXUI");
/// ```
// NOLINTNEXTLINE
Decorator hyperlink(std::string_view link) {
  return [link = std::string(link)](Element child) {
    return hyperlink(link, std::move(child));
  };
}

}  // namespace ftxui
