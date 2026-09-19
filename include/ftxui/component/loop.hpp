// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_COMPONENT_LOOP_HPP
#define FTXUI_COMPONENT_LOOP_HPP

#include <memory>  // for shared_ptr

#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/util/export.hpp"               // for FTXUI_EXPORT

namespace ftxui {
class ComponentBase;

using Component = std::shared_ptr<ComponentBase>;
class App;

/// @brief Loop es una clase que gestiona el bucle de eventos de un componente.
///
/// Es responsable de ejecutar el componente, manejar eventos, y
/// actualizar la pantalla.
///
/// La clase Loop está diseñada para usarse con un objeto App,
/// que representa la pantalla de la terminal.
///
/// **Ejemplo**
/// ```cpp
/// #include <ftxui/component/component.hpp>
/// #include <ftxui/component/app.hpp>
/// #include <ftxui/component/loop.hpp>
///
/// int main() {
///  auto screen = ftxui::App::TerminalOutput();
///  auto component = ftxui::Button("Click me", [] { ... });
///
///  ftxui::Loop loop(screen.get(), component);
///
///  // O bien
///  loop.Run();  // Bloqueante hasta que el componente termina.
///
///  // O bien
///  loop.RunOnce();  // No bloqueante, retorna inmediatamente.
///
///  // O bien
///  loop.RunOnceBlocking();  // Bloqueante hasta manejar un evento.
///
///  // O en un bucle:
///  while (!loop.HasQuitted()) {
///    loop.RunOnce();
///
///    // Hacer algo más como ejecutar una función de bucle de otra biblioteca.
///  }
/// }
/// ```
///
/// @ingroup component
class FTXUI_EXPORT(COMPONENT) Loop {
 public:
  Loop(App* screen, Component component);
  ~Loop();

  bool HasQuitted();
  void RunOnce();
  void RunOnceBlocking();
  void Run();

  // Esta clase no es copiable/movible.
  Loop& operator=(Loop&&) = delete;
  Loop& operator=(const Loop&) = delete;
  Loop(Loop&&) = delete;
  Loop(const Loop&) = delete;

 private:
  App* screen_;
  Component component_;
};

}  // namespace ftxui

#endif  // FTXUI_COMPONENT_LOOP_HPP
