// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/component/loop.hpp"

#include <utility>  // for move

#include "ftxui/component/app.hpp"  // for App, Component

namespace ftxui {

/// @brief Un Loop es un envoltorio alrededor de un Component y una App.
/// Se usa para ejecutar un Component en un terminal.
/// @see Component, App.
/// @see App::Loop().
/// @see App::Exit().
/// @param[in] screen La pantalla a usar.
/// @param[in] component El componente a ejecutar.
// NOLINTNEXTLINE
Loop::Loop(App* screen, Component component)
    : screen_(screen), component_(std::move(component)) {
  screen_->PreMain();
}

Loop::~Loop() {
  screen_->PostMain();
}

/// @brief Si el bucle ha terminado.
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
