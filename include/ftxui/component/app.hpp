// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_COMPONENT_APP_HPP
#define FTXUI_COMPONENT_APP_HPP

#include <atomic>      // for atomic
#include <chrono>      // for steady_clock, time_point
#include <functional>  // for function
#include <memory>      // for shared_ptr, unique_ptr
#include <string>      // for string, basic_string, allocator
#include <vector>      // for vector

#include "ftxui/component/animation.hpp"  // for TimePoint
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/task.hpp"   // for Task, Closure
#include "ftxui/screen/screen.hpp"    // for Screen
#include "ftxui/screen/terminal.hpp"  // for Dimensions
#include "ftxui/util/export.hpp"

namespace ftxui {
class ComponentBase;
using Component = std::shared_ptr<ComponentBase>;
struct Event;
class Selection;
class TaskRunner;

/// @brief App est une classe qui gère le cycle de vie de l'application.
/// Elle est responsable de l'initialisation du terminal, de l'exécution de la
/// boucle principale, et du nettoyage à la sortie.
///
/// @note Cette classe s'appelait précédemment ScreenInteractive.
///
/// @ingroup component
class FTXUI_EXPORT(COMPONENT) App : public Screen {
 public:
  // Constructeurs :

  /// @brief Crée une App de taille fixe.
  /// @param dimx La largeur de l'application.
  /// @param dimy La hauteur de l'application.
  static App FixedSize(int dimx, int dimy);

  /// @brief Crée une App occupant toute la taille du terminal. Ceci utilise
  /// le tampon d'écran alternatif afin de ne pas perturber le contenu du
  /// terminal.
  /// @note Ceci est identique à `App::FullscreenAlternateScreen()`
  static App Fullscreen();

  /// @brief Crée une App occupant toute la taille du terminal. Le tampon
  /// d'écran principal est utilisé. Cela signifie que si le terminal est
  /// redimensionné, le contenu précédent peut perturber le contenu du
  /// terminal.
  static App FullscreenPrimaryScreen();

  /// @brief Crée une App occupant toute la taille du terminal. Ceci utilise
  /// le tampon d'écran alternatif afin de ne pas perturber le contenu du
  /// terminal.
  static App FullscreenAlternateScreen();

  /// @brief Crée une App dont la largeur et la hauteur correspondent au
  /// composant dessiné.
  static App FitComponent();

  /// @brief Crée une App dont la largeur correspond à la largeur de sortie du
  /// terminal et dont la hauteur correspond au composant dessiné.
  static App TerminalOutput();

  // Destructeur.
  ~App() override;

  App(App&&) noexcept;
  App& operator=(App&&) noexcept;
  App(const App&) = delete;
  App& operator=(const App&) = delete;

  // Options. Doivent être appelées avant Loop().

  /// @brief Définit si la souris est suivie et si ses événements sont
  /// rapportés.
  /// @param enable Indique s'il faut activer le suivi des événements souris.
  /// @note Le suivi de la souris est activé par défaut.
  /// @note Le suivi de la souris n'est supporté que sur les terminaux qui le
  /// prennent en charge.
  /// @note Ceci doit être appelé avant d'appeler `App::Loop`.
  void TrackMouse(bool enable = true);

  /// @brief Active ou désactive la gestion automatique de l'entrée redirigée
  /// (pipe).
  /// Lorsque cela est activé, FTXUI détectera une entrée redirigée et
  /// redirigera stdin depuis /dev/tty pour les entrées clavier, permettant
  /// aux applications de lire les données redirigées tout en continuant à
  /// recevoir les événements clavier interactifs.
  /// @param enable Indique s'il faut activer la gestion de l'entrée redirigée.
  /// Par défaut à true.
  /// @note Ceci doit être appelé avant Loop().
  /// @note Cette fonctionnalité est activée par défaut.
  /// @note Cette fonctionnalité n'est disponible que sur les systèmes POSIX
  /// (Linux/macOS).
  void HandlePipedInput(bool enable = true);

  /// @brief Retourne l'application actuellement active, nullptr si aucune.
  static App* Active();

  // Démarrer/arrêter la boucle principale.

  /// @brief Exécute la boucle principale.
  /// @param component Le composant à dessiner.
  void Loop(Component component);

  /// @brief Quitte la boucle principale.
  void Exit();

  /// @brief Retourne une fonction permettant de quitter la boucle principale.
  Closure ExitLoopClosure();

  /// @brief Décore une fonction. La fonction retournée s'exécutera de manière
  /// similaire à celle passée en entrée, mais avec les hooks du terminal de
  /// l'application actuellement active temporairement désinstallés.
  Closure WithRestoredIO(Closure fn);

  /// @brief FTXUI implémente des gestionnaires pour Ctrl-C et Ctrl-Z. Par
  /// défaut, ces gestionnaires sont exécutés, même si le composant intercepte
  /// l'événement. Cela évite aux utilisateurs de devoir gérer chaque
  /// événement pour ne pas rester piégés dans l'application. Cependant, dans
  /// certains cas, l'application peut vouloir gérer ces événements
  /// elle-même. Dans ce cas, l'application peut forcer FTXUI à ne pas gérer
  /// ces événements en appelant les fonctions suivantes avec force=true.
  void ForceHandleCtrlC(bool force = true);

  /// @brief Force FTXUI à gérer ou non Ctrl-Z, même si le composant intercepte
  /// l'Event::CtrlZ.
  void ForceHandleCtrlZ(bool force = true);

  // Poster des tâches à exécuter par la boucle.

  /// @brief Ajoute une tâche à la boucle principale.
  /// Elle sera exécutée plus tard, après toutes les autres tâches planifiées.
  void Post(Task task);

  /// @brief Ajoute un événement à la boucle principale.
  /// Il sera exécuté plus tard, après tous les autres événements planifiés.
  void PostEvent(Event event);

  /// @brief Ajoute une tâche à la boucle principale.
  /// Elle sera exécutée plus tard, après toutes les autres tâches planifiées.
  static void PostEventOrExecute(Closure closure);

  /// @brief Ajoute une tâche pour dessiner l'écran une fois de plus, jusqu'à
  /// ce que toutes les animations soient terminées.
  void RequestAnimationFrame();

  // API de sélection :

  /// @brief Tente d'obtenir le verrou unique permettant de capturer la
  /// souris.
  /// @return Un verrou unique si la souris n'est pas déjà capturée, sinon
  /// null.
  CapturedMouse CaptureMouse();

  /// @brief Retourne le contenu de la sélection actuelle.
  std::string GetSelection();

  /// @brief Définit un callback qui sera appelé lorsque la sélection change.
  void SelectionChange(std::function<void()> callback);

  // Informations sur le terminal.

  /// @brief Retourne le nom du terminal.
  const std::string& TerminalName() const;

  /// @brief Retourne la version du terminal.
  int TerminalVersion() const;

  /// @brief Retourne le nom de l'émulateur de terminal.
  const std::string& TerminalEmulatorName() const;

  /// @brief Retourne la version de l'émulateur de terminal.
  const std::string& TerminalEmulatorVersion() const;

  /// @brief Retourne les capacités du terminal.
  const std::vector<int>& TerminalCapabilities() const;

  /// @brief Retourne les noms des capacités du terminal.
  std::vector<std::string> TerminalCapabilityNames() const;

 private:
  void ExitNow();
  void Install();
  void Uninstall();

  void PreMain();
  void PostMain();

  /// @brief Retourne si la boucle principale a été quittée.
  bool HasQuitted();
  void RunOnce(const Component& component);
  void RunOnceBlocking(Component component);

  void HandleTask(Component component, Task& task);
  bool HandleSelection(bool handled, Event event);
  void Draw(Component component);
  std::string ResetCursorPosition();

  void RequestCursorPosition(bool force = false);

  void TerminalSend(std::string_view);
  void TerminalFlush();

  void InstallPipedInputHandling();
  void InstallTerminalInfo();

  void Signal(int signal);

  size_t FetchTerminalEvents();

  void PostAnimationTask();

  struct Internal;
  explicit App(std::unique_ptr<Internal> internal, int dimx, int dimy);

  std::unique_ptr<Internal> internal_;

  friend class Loop;

 public:
  class Private {
   public:
    static void Signal(App& s, int signal) { s.Signal(signal); }
  };
  friend Private;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_APP_HPP */
