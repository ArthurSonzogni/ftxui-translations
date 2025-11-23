// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#include <memory>  // for allocator, __shared_ptr_access
#include <string>  // for char_traits, operator+, string, basic_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // for Ref

int main() {
  using namespace ftxui;

  // Les données :
  std::string first_name;
  std::string last_name;
  std::string password;
  std::string phoneNumber;

  // Les composants d'entrée de base :
  Component input_first_name = Input(&first_name, "first name");
  Component input_last_name = Input(&last_name, "last name");

  // Le composant d'entrée du mot de passe :
  InputOption password_option;
  password_option.password = true;
  Component input_password = Input(&password, "password", password_option);

  // Le composant d'entrée du numéro de téléphone :
  // Nous utilisons `CatchEvent` pour filtrer les caractères non numériques.
  Component input_phone_number = Input(&phoneNumber, "phone number");
  input_phone_number |= CatchEvent([&](Event event) {
    return event.is_character() && !std::isdigit(event.character()[0]);
  });
  input_phone_number |= CatchEvent([&](Event event) {
    return event.is_character() && phoneNumber.size() > 10;
  });

  // L'arbre des composants :
  auto component = Container::Vertical({
      input_first_name,
      input_last_name,
      input_password,
      input_phone_number,
  });

  // Ajuster la façon dont l'arbre des composants est rendu :
  auto renderer = Renderer(component, [&] {
    return vbox({
               hbox(text(" First name : "), input_first_name->Render()),
               hbox(text(" Last name  : "), input_last_name->Render()),
               hbox(text(" Password   : "), input_password->Render()),
               hbox(text(" Phone num  : "), input_phone_number->Render()),
               separator(),
               text("Hello " + first_name + " " + last_name),
               text("Your password is " + password),
               text("Your phone number is " + phoneNumber),
           }) |
           border;
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
}