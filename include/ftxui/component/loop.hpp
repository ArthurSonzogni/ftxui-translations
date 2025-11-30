// Copyright 2022 Arthur Sonzogni. All rights reserved.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef FTXUI_COMPONENT_LOOP_HPP
#define FTXUI_COMPONENT_LOOP_HPP

#include <memory>  // for shared_ptr

#include "ftxui/component/component_base.hpp"  // for ComponentBase

namespace ftxui {
class ComponentBase;

using Component = std::shared_ptr<ComponentBase>;
class ScreenInteractive;

/// @brief Loop es una clase que gestiona el bucle de eventos de un componente.
///
/// Es responsable de ejecutar el componente, manejar los eventos y
/// actualizar la pantalla.
///
/// La clase Loop está diseñada para ser utilizada con un objeto ScreenInteractive,
/// que representa la pantalla del terminal.
///
/// **Ejemplo**
/// ```cpp
/// #include <ftxui/component/component.hpp>
/// #include <ftxui/component/screen_interactive.hpp>
/// #include <ftxui/component/loop.hpp>
///
/// int main() {
///  auto screen = ftxui::ScreenInteractive::TerminalOutput();
///  auto component = ftxui::Button("Click me", [] { ... });
///
///  ftxui::Loop loop(screen.get(), component);
///
///  // O
///  loop.Run();  // Bloquea hasta que el componente se cierra.
///
///  // O
///  loop.RunOnce();  // No bloqueante, regresa inmediatamente.
///
///  // O
///  loop.RunOnceBlocking();  // Bloquea hasta manejar un evento.
///
///  // O en un bucle:
///  while (!loop.HasQuitted()) {
///    loop.RunOnce();
///
///    // Haz otra cosa como ejecutar una función de bucle de otra biblioteca.
///  }
/// }
/// ```
///
/// @ingroup component
class Loop {
 public:
  Loop(ScreenInteractive* screen, Component component);
  ~Loop();

  bool HasQuitted();
  void RunOnce();
  void RunOnceBlocking();
  void Run();

  // Esta clase no es copiable/movible.
  Loop(const Loop&) = default;
  Loop(Loop&&) = delete;
  Loop& operator=(Loop&&) = delete;
  Loop(const ScreenInteractive&) = delete;
  Loop& operator=(const Loop&) = delete;

 private:
  ScreenInteractive* screen_;
  Component component_;
};

}  // namespace ftxui

#endif  // FTXUI_COMPONENT_LOOP_HPP