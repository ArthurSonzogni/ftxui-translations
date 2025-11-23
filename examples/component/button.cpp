// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <memory>  // pour shared_ptr, __shared_ptr_access
#include <string>  // pour operator+, to_string

#include "ftxui/component/captured_mouse.hpp"  // pour ftxui
#include "ftxui/component/component.hpp"  // pour Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // pour ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // pour ScreenInteractive
#include "ftxui/dom/elements.hpp"  // pour separator, gauge, text, Element, operator|, vbox, border

using namespace ftxui;

// Ceci est une fonction utilitaire pour créer un bouton avec un style personnalisé.
// Le style est défini par une fonction lambda qui prend un EntryState et
// retourne un Element.
// Nous utilisons `center` pour centrer le texte à l'intérieur du bouton, puis `border` pour
// ajouter une bordure autour du bouton, et enfin `flex` pour que le bouton remplisse
// l'espace disponible.
ButtonOption Style() {
  auto option = ButtonOption::Animated();
  option.transform = [](const EntryState& s) {
    auto element = text(s.label);
    if (s.focused) {
      element |= bold;
    }
    return element | center | borderEmpty | flex;
  };
  return option;
}

int main() {
  int value = 50;

  // clang-format off
  auto btn_dec_01 = Button("-1", [&] { value -= 1; }, Style());
  auto btn_inc_01 = Button("+1", [&] { value += 1; }, Style());
  auto btn_dec_10 = Button("-10", [&] { value -= 10; }, Style());
  auto btn_inc_10 = Button("+10", [&] { value += 10; }, Style());
  // clang-format on

  // L'arbre des composants. Cela définit comment naviguer en utilisant le clavier.
  // La `row` sélectionnée est partagée pour obtenir une disposition en grille.
  int row = 0;
  auto buttons = Container::Vertical({
      Container::Horizontal({btn_dec_01, btn_inc_01}, &row) | flex,
      Container::Horizontal({btn_dec_10, btn_inc_10}, &row) | flex,
  });

  // Modifie la manière de les afficher à l'écran :
  auto component = Renderer(buttons, [&] {
    return vbox({
               text("value = " + std::to_string(value)),
               separator(),
               buttons->Render() | flex,
           }) |
           flex | border;
  });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(component);
  return 0;
}