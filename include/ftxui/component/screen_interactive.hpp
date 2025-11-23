// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#ifndef FTXUI_COMPONENT_SCREEN_INTERACTIVE_HPP
#define FTXUI_COMPONENT_SCREEN_INTERACTIVE_HPP

#include <atomic>      // for atomic
#include <functional>  // for function
#include <memory>      // for shared_ptr
#include <string>      // for string

#include "ftxui/component/animation.hpp"       // for TimePoint
#include "ftxui/component/captured_mouse.hpp"  // for CapturedMouse
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/component/task.hpp"            // for Task, Closure
#include "ftxui/dom/selection.hpp"             // for SelectionOption
#include "ftxui/screen/screen.hpp"             // for Screen

namespace ftxui {
class ComponentBase;
class Loop;
struct Event;

using Component = std::shared_ptr<ComponentBase>;
class ScreenInteractivePrivate;

namespace task {
class TaskRunner;
}

/// @brief ScreenInteractive est un `Screen` qui peut gérer les événements, exécuter une boucle
/// principale et gérer les composants.
///
/// @ingroup component
class ScreenInteractive : public Screen {
 public:
  // Constructors:
  static ScreenInteractive FixedSize(int dimx, int dimy);
  static ScreenInteractive Fullscreen();
  static ScreenInteractive FullscreenPrimaryScreen();
  static ScreenInteractive FullscreenAlternateScreen();
  static ScreenInteractive FitComponent();
  static ScreenInteractive TerminalOutput();

  // Destructor.
  ~ScreenInteractive() override;

  // Options. Must be called before Loop().
  void TrackMouse(bool enable = true);
  void HandlePipedInput(bool enable = true);

  // Return the currently active screen, nullptr if none.
  static ScreenInteractive* Active();

  // Start/Stop the main loop.
  void Loop(Component);
  void Exit();
  Closure ExitLoopClosure();

  // Post tasks to be executed by the loop.
  void Post(Task task);
  void PostEvent(Event event);
  void RequestAnimationFrame();

  CapturedMouse CaptureMouse();

  // Décore une fonction. Celle qui en résulte s'exécutera de manière similaire à celle qui
  // a été entrée, mais avec les hooks du terminal de l'écran actuellement actif
  // temporairement désinstallés.
  Closure WithRestoredIO(Closure);

  // FTXUI implémente des gestionnaires pour Ctrl-C et Ctrl-Z. Par défaut, ces gestionnaires
  // sont exécutés, même si le composant intercepte l'événement. Cela évite aux utilisateurs
  // de gérer chaque événement d'être piégés dans l'application. Cependant, dans certains
  // cas, l'application peut vouloir gérer ces événements elle-même. Dans ce
  // cas, l'application peut forcer FTXUI à ne pas gérer ces événements en appelant
  // les fonctions suivantes avec force=true.
  void ForceHandleCtrlC(bool force);
  void ForceHandleCtrlZ(bool force);

  // Selection API.
  std::string GetSelection();
  void SelectionChange(std::function<void()> callback);

 private:
  void ExitNow();

  void Install();
  void Uninstall();

  void PreMain();
  void PostMain();

  bool HasQuitted();
  void RunOnce(Component component);
  void RunOnceBlocking(Component component);

  void HandleTask(Component component, Task& task);
  bool HandleSelection(bool handled, Event event);
  void RefreshSelection();
  void Draw(Component component);
  void ResetCursorPosition();

  void InstallPipedInputHandling();

  void Signal(int signal);

  void FetchTerminalEvents();

  void PostAnimationTask();

  ScreenInteractive* suspended_screen_ = nullptr;
  enum class Dimension {
    FitComponent,
    Fixed,
    Fullscreen,
    TerminalOutput,
  };
  ScreenInteractive(Dimension dimension,
                    int dimx,
                    int dimy,
                    bool use_alternative_screen);

  const Dimension dimension_;
  const bool use_alternative_screen_;

  bool track_mouse_ = true;

  std::string set_cursor_position;
  std::string reset_cursor_position;

  std::atomic<bool> quit_{false};
  bool animation_requested_ = false;
  animation::TimePoint previous_animation_time_;

  int cursor_x_ = 1;
  int cursor_y_ = 1;

  std::uint64_t frame_count_ = 0;
  bool mouse_captured = false;
  bool previous_frame_resized_ = false;

  bool frame_valid_ = false;

  bool force_handle_ctrl_c_ = true;
  bool force_handle_ctrl_z_ = true;

  // État de la gestion de l'entrée piped (POSIX uniquement)
  bool handle_piped_input_ = true;
  // Descripteur de fichier pour /dev/tty, utilisé pour la gestion de l'entrée piped.
  int tty_fd_ = -1;

  // Le style du curseur à restaurer à la sortie.
  int cursor_reset_shape_ = 1;

  // API de sélection:
  CapturedMouse selection_pending_;
  struct SelectionData {
    int start_x = -1;
    int start_y = -1;
    int end_x = -2;
    int end_y = -2;
    bool empty = true;
    bool operator==(const SelectionData& other) const;
    bool operator!=(const SelectionData& other) const;
  };
  SelectionData selection_data_;
  SelectionData selection_data_previous_;
  std::unique_ptr<Selection> selection_;
  std::function<void()> selection_on_change_;

  // Idiome d'implémentation privée PIMPL (Pimpl).
  struct Internal;
  std::unique_ptr<Internal> internal_;

  friend class Loop;

  Component component_;

 public:
  class Private {
   public:
    static void Signal(ScreenInteractive& s, int signal) { s.Signal(signal); }
  };
  friend Private;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_SCREEN_INTERACTIVE_HPP */