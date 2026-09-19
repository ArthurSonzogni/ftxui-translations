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

/// @brief App es una clase que gestiona el ciclo de vida de la aplicación.
/// Es responsable de inicializar la terminal, ejecutar el bucle principal,
/// y limpiar al salir.
///
/// @note Esta clase se llamaba anteriormente ScreenInteractive.
///
/// @ingroup component
class FTXUI_EXPORT(COMPONENT) App : public Screen {
 public:
  // Constructores:

  /// @brief Crea una App con un tamaño fijo.
  /// @param dimx El ancho de la app.
  /// @param dimy El alto de la app.
  static App FixedSize(int dimx, int dimy);

  /// @brief Crea una App que ocupa el tamaño completo de la terminal. Esto usa
  /// el búfer de pantalla alternativo para evitar interferir con el contenido de la terminal.
  /// @note Esto es igual que `App::FullscreenAlternateScreen()`
  static App Fullscreen();

  /// @brief Crea una App que ocupa el tamaño completo de la terminal. Se usa
  /// el búfer de pantalla primario. Esto significa que si se redimensiona la terminal, el contenido
  /// anterior podría interferir con el contenido de la terminal.
  static App FullscreenPrimaryScreen();

  /// @brief Crea una App que ocupa el tamaño completo de la terminal. Esto usa
  /// el búfer de pantalla alternativo para evitar interferir con el contenido de la terminal.
  static App FullscreenAlternateScreen();

  /// @brief Crea una App cuyo ancho y alto coinciden con el componente que se
  /// dibuja.
  static App FitComponent();

  /// @brief Crea una App cuyo ancho coincide con el ancho de salida de la terminal y
  /// el alto coincide con el componente que se dibuja.
  static App TerminalOutput();

  // Destructor.
  ~App() override;

  App(App&&) noexcept;
  App& operator=(App&&) noexcept;
  App(const App&) = delete;
  App& operator=(const App&) = delete;

  // Opciones. Debe ser llamado antes de Loop().

  /// @brief Establece si el mouse se rastrea y se reportan sus eventos.
  /// @param enable Si se debe habilitar el rastreo de eventos del mouse.
  /// @note El rastreo del mouse está habilitado por defecto.
  /// @note El rastreo del mouse solo es compatible con terminales que lo soportan.
  /// @note Esto debe llamarse antes de llamar a `App::Loop`.
  void TrackMouse(bool enable = true);

  /// @brief Habilita o deshabilita el manejo automático de la entrada por tubería (pipe).
  /// Cuando está habilitado, FTXUI detectará la entrada por tubería y redirigirá stdin desde
  /// /dev/tty para la entrada del teclado, permitiendo que las aplicaciones lean datos por tubería
  /// mientras siguen recibiendo eventos de teclado interactivos.
  /// @param enable Si se debe habilitar el manejo de entrada por tubería. Por defecto es true.
  /// @note Esto debe llamarse antes de Loop().
  /// @note Esta función está habilitada por defecto.
  /// @note Esta función solo está disponible en sistemas POSIX (Linux/macOS).
  void HandlePipedInput(bool enable = true);

  /// @brief Devuelve la app actualmente activa, nullptr si no hay ninguna.
  static App* Active();

  // Iniciar/Detener el bucle principal.

  /// @brief Ejecuta el bucle principal.
  /// @param component El componente a dibujar.
  void Loop(Component component);

  /// @brief Sale del bucle principal.
  void Exit();

  /// @brief Devuelve una función para salir del bucle principal.
  Closure ExitLoopClosure();

  /// @brief Decora una función. La función resultante se ejecutará de forma similar
  /// a la de entrada, pero con los ganchos de terminal de la app actualmente activa
  /// desinstalados temporalmente.
  Closure WithRestoredIO(Closure fn);

  /// @brief FTXUI implementa manejadores para Ctrl-C y Ctrl-Z. Por defecto, estos
  /// manejadores se ejecutan, incluso si el componente captura el evento. Esto evita que
  /// los usuarios que manejan cada evento queden atrapados en la aplicación. Sin embargo, en
  /// algunos casos, la aplicación puede querer manejar estos eventos ella misma. En
  /// este caso, la aplicación puede forzar a FTXUI a no manejar estos eventos
  /// llamando a las siguientes funciones con force=true.
  void ForceHandleCtrlC(bool force = true);

  /// @brief Fuerza a FTXUI a manejar o no manejar Ctrl-Z, incluso si el componente
  /// captura el Event::CtrlZ.
  void ForceHandleCtrlZ(bool force = true);

  // Publica tareas para ser ejecutadas por el bucle.

  /// @brief Agrega una tarea al bucle principal.
  /// Se ejecutará más tarde, después de todas las demás tareas programadas.
  void Post(Task task);

  /// @brief Agrega un evento al bucle principal.
  /// Se ejecutará más tarde, después de todos los demás eventos programados.
  void PostEvent(Event event);

  /// @brief Agrega una tarea al bucle principal.
  /// Se ejecutará más tarde, después de todas las demás tareas programadas.
  static void PostEventOrExecute(Closure closure);

  /// @brief Agrega una tarea para dibujar la pantalla una vez más, hasta que todas las
  /// animaciones hayan terminado.
  void RequestAnimationFrame();

  // API de selección:

  /// @brief Intenta obtener el bloqueo exclusivo (unique lock) para poder capturar el mouse.
  /// @return Un bloqueo exclusivo si el mouse no está ya capturado, de lo contrario un
  /// null.
  CapturedMouse CaptureMouse();

  /// @brief Devuelve el contenido de la selección actual.
  std::string GetSelection();

  /// @brief Establece una función de retorno (callback) que se llamará cuando la selección cambie.
  void SelectionChange(std::function<void()> callback);

  // Información de la terminal.

  /// @brief Devuelve el nombre de la terminal.
  const std::string& TerminalName() const;

  /// @brief Devuelve la versión de la terminal.
  int TerminalVersion() const;

  /// @brief Devuelve el nombre del emulador de terminal.
  const std::string& TerminalEmulatorName() const;

  /// @brief Devuelve la versión del emulador de terminal.
  const std::string& TerminalEmulatorVersion() const;

  /// @brief Devuelve las capacidades de la terminal.
  const std::vector<int>& TerminalCapabilities() const;

  /// @brief Devuelve los nombres de las capacidades de la terminal.
  std::vector<std::string> TerminalCapabilityNames() const;

 private:
  void ExitNow();
  void Install();
  void Uninstall();

  void PreMain();
  void PostMain();

  /// @brief Devuelve si el bucle principal ha terminado.
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
