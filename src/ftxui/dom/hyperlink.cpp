// Copyright 2023 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#include <cstdint>  // for uint8_t
#include <memory>   // for make_shared
#include <string>   // for string
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"        // for Element, Decorator, hyperlink
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/box.hpp"          // for Box
#include "ftxui/screen/screen.hpp"       // for Screen, Pixel

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
        screen.PixelAt(x, y).hyperlink = hyperlink_id;
      }
    }
    NodeDecorator::Render(screen);
  }

  std::string link_;
};
/// @brief Rend la zone affichée cliquable à l'aide d'un navigateur web.
///        Le lien sera ouvert lorsque l'utilisateur cliquera dessus.
///        Ceci n'est pris en charge que par un ensemble limité d'émulateurs de terminal.
///        Liste: https://github.com/Alhadis/OSC8-Adoption/
/// @param link Le lien
/// @param child L'élément d'entrée.
/// @return L'élément de sortie avec le lien.
/// @ingroup dom
///
/// ### Exemple
///
/// ```cpp
/// Element document =
///   hyperlink("https://github.com/ArthurSonzogni/FTXUI", "link");
/// ```
Element hyperlink(std::string link, Element child) {
  return std::make_shared<Hyperlink>(std::move(child), std::move(link));
}

/// @brief Décore avec un hyperlien.
///        Le lien sera ouvert lorsque l'utilisateur cliquera dessus.
///        Ceci n'est pris en charge que par un ensemble limité d'émulateurs de terminal.
///        Liste: https://github.com/Alhadis/OSC8-Adoption/
/// @param link Le lien vers lequel rediriger les utilisateurs.
/// @return Le Décorateur appliquant l'hyperlien.
/// @ingroup dom
///
/// ### Exemple
///
/// ```cpp
/// Element document =
///   text("red") | hyperlink("https://github.com/Arthursonzogni/FTXUI");
/// ```
// NOLINTNEXTLINE
Decorator hyperlink(std::string link) {
  return [link](Element child) { return hyperlink(link, std::move(child)); };
}

}  // namespace ftxui
