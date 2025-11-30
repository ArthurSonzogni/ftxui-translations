// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
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

  // Los datos:
  std::string first_name;
  std::string last_name;
  std::string password;
  std::string phoneNumber;

  // Los componentes básicos de entrada:
  Component input_first_name = Input(&first_name, "first name");
  Component input_last_name = Input(&last_name, "last name");

  // El componente de entrada de contraseña:
  InputOption password_option;
  password_option.password = true;
  Component input_password = Input(&password, "password", password_option);

  // El componente de entrada de número de teléfono:
  // Estamos usando `CatchEvent` para filtrar caracteres no numéricos.  Component input_phone_number = Input(&phoneNumber, "phone number");
  input_phone_number |= CatchEvent([&](Event event) {
    return event.is_character() && !std::isdigit(event.character()[0]);
  });
  input_phone_number |= CatchEvent([&](Event event) {
    return event.is_character() && phoneNumber.size() > 10;
  });

  // El árbol de componentes:
  auto component = Container::Vertical({
      input_first_name,
      input_last_name,
      input_password,
      input_phone_number,
  });

  // Ajustar cómo se renderiza el árbol de componentes:
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
