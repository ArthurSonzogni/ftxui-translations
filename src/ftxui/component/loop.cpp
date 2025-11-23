// Copyright 2022 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#include "ftxui/component/loop.hpp"

#include <utility>  // for move

#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive, Component

namespace ftxui {

/// @brief Une boucle est un wrapper autour d'un Component et d'un ScreenInteractive.
/// Elle est utilisée pour exécuter un Component dans un terminal.
/// @see Component, ScreenInteractive.
/// @see ScreenInteractive::Loop().
/// @see ScreenInteractive::ExitLoop().
/// @param[in] screen The screen to use.
/// @param[in] component The component to run.
// NOLINTNEXTLINE
Loop::Loop(ScreenInteractive* screen, Component component)
    : screen_(screen), component_(std::move(component)) {
  screen_->PreMain();
}

Loop::~Loop() {
  screen_->PostMain();
}

/// @brief Indique si la boucle a été quittée.
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

/// Exécute la boucle, bloquant le thread actuel, jusqu'à ce que la boucle soit
/// quittée.
void Loop::Run() {
  while (!HasQuitted()) {
    RunOnceBlocking();
  }
}

}  // namespace ftxui
