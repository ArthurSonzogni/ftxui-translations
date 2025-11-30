// Copyright 2022 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include "ftxui/component/loop.hpp"

#include <utility>  // for move

#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive, Component

namespace ftxui {

/// @brief Un Loop es un envoltorio alrededor de un Component y un ScreenInteractive.
/// Se utiliza para ejecutar un Component en un terminal.
/// @see Component, ScreenInteractive.
/// @see ScreenInteractive::Loop().
/// @see ScreenInteractive::ExitLoop().
/// @param[in] screen La pantalla a utilizar.
/// @param[in] component El componente a ejecutar.
// NOLINTNEXTLINE
Loop::Loop(ScreenInteractive* screen, Component component)
    : screen_(screen), component_(std::move(component)) {
  screen_->PreMain();
}

Loop::~Loop() {
  screen_->PostMain();
}

/// @brief Indica si el bucle ha terminado.
bool Loop::HasQuitted() {
  return screen_->HasQuitted();
}

/// @brief Ejecuta el bucle una vez. Hace que el `component` procese todas las tareas/eventos
/// pendientes. Un nuevo fotograma podría dibujarse si el anterior fue invalidado.
/// Devuelve true hasta que el bucle no haya terminado.
void Loop::RunOnce() {
  screen_->RunOnce(component_);
}

/// @brief Espera a que se maneje al menos un evento y ejecuta
/// `Loop::RunOnce()`.
void Loop::RunOnceBlocking() {
  screen_->RunOnceBlocking(component_);
}

/// Ejecuta el bucle, bloqueando el hilo actual, hasta que el bucle haya
/// terminado.
void Loop::Run() {
  while (!HasQuitted()) {
    RunOnceBlocking();
  }
}

}  // namespace ftxui
