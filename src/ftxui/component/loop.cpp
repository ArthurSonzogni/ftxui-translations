// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/component/loop.hpp"

#include <utility>  // for move

#include "ftxui/component/app.hpp"  // for App, Component

namespace ftxui {

/// @brief A Loop is a wrapper around a Component and an App.
/// It is used to run a Component in a terminal.
/// @see Component, App.
/// @see App::Loop().
/// @see App::Exit().
/// @param[in] screen The screen to use.
/// @param[in] component The component to run.
// NOLINTNEXTLINE
Loop::Loop(App* screen, Component component)
    : screen_(screen), component_(std::move(component)) {
  screen_->PreMain();
}

Loop::~Loop() {
  screen_->PostMain();
}

/// @brief Whether the loop has quit.
bool Loop::HasQuitted() {
  return screen_->HasQuitted();
}

/// @brief Exécute la boucle une seule fois. Fait en sorte que le `component` traite toutes les tâches/événements en attente.
/// Une nouvelle trame peut être dessinée si la précédente a été invalidée.
/// Retourne vrai tant que la boucle n'est pas terminée.
void Loop::RunOnce() {
  screen_->RunOnce(component_);
}

/// @brief Attend qu'au moins un événement soit géré et exécute
/// `Loop::RunOnce()`.
void Loop::RunOnceBlocking() {
  screen_->RunOnceBlocking(component_);
}

/// Execute the loop, blocking the current thread, up until the loop has
/// quit.
void Loop::Run() {
  while (!HasQuitted()) {
    RunOnceBlocking();
  }
}

}  // namespace ftxui
