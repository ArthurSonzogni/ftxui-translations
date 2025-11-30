// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
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

/// @brief ScreenInteractive es una `Screen` que puede manejar eventos, ejecutar un
/// bucle principal y administrar componentes.
///
/// @ingroup component
class ScreenInteractive : public Screen {
 public:
  // Constructores:
  static ScreenInteractive FixedSize(int dimx, int dimy);
  static ScreenInteractive Fullscreen();
  static ScreenInteractive FullscreenPrimaryScreen();
  static ScreenInteractive FullscreenAlternateScreen();
  static ScreenInteractive FitComponent();
  static ScreenInteractive TerminalOutput();

  // Destructor.
  ~ScreenInteractive() override;

  // Opciones. Debe ser llamado antes de Loop().
  void TrackMouse(bool enable = true);
  void HandlePipedInput(bool enable = true);

  // Devuelve la pantalla activa actualmente, nullptr si no hay ninguna.
  static ScreenInteractive* Active();

  // Iniciar/Detener el bucle principal.
  void Loop(Component);
  void Exit();
  Closure ExitLoopClosure();

  // Publica tareas para ser ejecutadas por el bucle.
  void Post(Task task);
  void PostEvent(Event event);
  void RequestAnimationFrame();

  CapturedMouse CaptureMouse();

  // Decora una función. La función resultante se ejecutará de manera similar a la
  // función de entrada, pero con los hooks de terminal de la pantalla activa
  // temporalmente desinstalados.
  Closure WithRestoredIO(Closure);

  // FTXUI implementa manejadores para Ctrl-C y Ctrl-Z. Por defecto, estos
  // manejadores se ejecutan, incluso si el componente captura el evento. Esto
  // evita que los usuarios que manejan cada evento queden atrapados en la
  // aplicación. Sin embargo, en algunos casos, la aplicación puede querer
  // manejar estos eventos por sí misma. En este caso, la aplicación puede
  // forzar a FTXUI a no manejar estos eventos llamando a las siguientes
  // funciones con force=true.
  void ForceHandleCtrlC(bool force);
  void ForceHandleCtrlZ(bool force);

  // API de selección.
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

  // Estado del manejo de entrada por tubería (solo POSIX)
  bool handle_piped_input_ = true;
  // Descriptor de archivo para /dev/tty, usado para el manejo de entrada por tubería.
  int tty_fd_ = -1;

  // El estilo del cursor a restaurar al salir.
  int cursor_reset_shape_ = 1;

  // API de selección:
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

  // Idioma de implementación privada PIMPL (Pimpl).
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
