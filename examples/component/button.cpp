// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <memory>  // for shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string

#include "ftxui/component/app.hpp"             // for App
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/dom/elements.hpp"  // for separator, gauge, text, Element, operator|, vbox, border

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

  // Modifier la façon de les afficher à l'écran :
  auto component = Renderer(buttons, [&] {
    return vbox({
               text("value = " + std::to_string(value)),
               separator(),
               buttons->Render() | flex,
           }) |
           flex | border;
  });

  auto screen = App::FitComponent();
  screen.Loop(component);
  return 0;
}
