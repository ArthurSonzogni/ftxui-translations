// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include "ftxui/component/screen_interactive.hpp"
#include <algorithm>  // for copy, max, min
#include <array>      // for array
#include <atomic>
#include <chrono>  // for operator-, milliseconds, operator>=, duration, common_type<>::type, time_point
#include <csignal>  // for signal, SIGTSTP, SIGABRT, SIGWINCH, raise, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM, __sighandler_t, size_t
#include <cstdint>
#include <cstdio>                    // for fileno, stdin
#include <ftxui/component/task.hpp>  // for Task, Closure, AnimationTask
#include <ftxui/screen/screen.hpp>  // for Pixel, Screen::Cursor, Screen, Screen::Cursor::Hidden
#include <functional>        // for function
#include <initializer_list>  // for initializer_list
#include <iostream>  // for cout, ostream, operator<<, basic_ostream, endl, flush
#include <memory>
#include <stack>  // for stack
#include <string>
#include <thread>   // for thread, sleep_for
#include <tuple>    // for _Swallow_assign, ignore
#include <utility>  // for move, swap
#include <variant>  // for visit, variant
#include <vector>   // for vector
#include "ftxui/component/animation.hpp"  // for TimePoint, Clock, Duration, Params, RequestAnimationFrame
#include "ftxui/component/captured_mouse.hpp"  // for CapturedMouse, CapturedMouseInterface
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/component/loop.hpp"            // for Loop
#include "ftxui/component/task_runner.hpp"
#include "ftxui/component/terminal_input_parser.hpp"  // for TerminalInputParser
#include "ftxui/dom/node.hpp"                         // for Node, Render
#include "ftxui/screen/terminal.hpp"                  // for Dimensions, Size
#include "ftxui/screen/util.hpp"                      // for util::clamp
#include "ftxui/util/autoreset.hpp"                   // for AutoReset

#if defined(_WIN32)
#define DEFINE_CONSOLEV2_PROPERTIES
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#ifndef UNICODE
#error Must be compiled in UNICODE mode
#endif
#else
#include <fcntl.h>
#include <sys/select.h>  // for select, FD_ISSET, FD_SET, FD_ZERO, fd_set, timeval
#include <termios.h>  // for tcsetattr, termios, tcgetattr, TCSANOW, cc_t, ECHO, ICANON, VMIN, VTIME
#include <unistd.h>  // for STDIN_FILENO, read
#include <cerrno>
#endif

// Quick exit is missing in standard CLang headers
#if defined(__clang__) && defined(__APPLE__)
#define quick_exit(a) exit(a)
#endif

namespace ftxui {

struct ScreenInteractive::Internal {
  // Convierte char a Event.
  TerminalInputParser terminal_input_parser;

  task::TaskRunner task_runner;

  // La última vez que se recibió un carácter.
  std::chrono::time_point<std::chrono::steady_clock> last_char_time =
      std::chrono::steady_clock::now();

  explicit Internal(std::function<void(Event)> out)
      : terminal_input_parser(std::move(out)) {}
};

namespace animation {
void RequestAnimationFrame() {
  auto* screen = ScreenInteractive::Active();
  if (screen) {
    screen->RequestAnimationFrame();
  }
}
}  // namespace animation

namespace {

ScreenInteractive* g_active_screen = nullptr;  // NOLINT

void Flush() {
  // Emscripten no implementa flush. Interpretamos cero como flush.
  std::cout << '\0' << std::flush;
}

constexpr int timeout_milliseconds = 20;
[[maybe_unused]] constexpr int timeout_microseconds =
    timeout_milliseconds * 1000;
#if defined(_WIN32)

#elif defined(__EMSCRIPTEN__)
#include <emscripten.h>

extern "C" {
EMSCRIPTEN_KEEPALIVE
void ftxui_on_resize(int columns, int rows) {
  Terminal::SetFallbackSize({
      columns,
      rows,
  });
  std::raise(SIGWINCH);
}
}

#else  // POSIX (Linux & Mac)

int CheckStdinReady(int fd) {
  timeval tv = {0, 0};  // NOLINT
  fd_set fds;
  FD_ZERO(&fds);                                // NOLINT
  FD_SET(fd, &fds);                             // NOLINT
  select(fd + 1, &fds, nullptr, nullptr, &tv);  // NOLINT
  return FD_ISSET(fd, &fds);                    // NOLINT
}

#endif

std::stack<Closure> on_exit_functions;  // NOLINT
void OnExit() {
  while (!on_exit_functions.empty()) {
    on_exit_functions.top()();
    on_exit_functions.pop();
  }
}

std::atomic<int> g_signal_exit_count = 0;  // NOLINT
#if !defined(_WIN32)
std::atomic<int> g_signal_stop_count = 0;    // NOLINT
std::atomic<int> g_signal_resize_count = 0;  // NOLINT
#endif

// Función asíncrona segura para señales
void RecordSignal(int signal) {
  switch (signal) {
    case SIGABRT:
    case SIGFPE:
    case SIGILL:
    case SIGINT:
    case SIGSEGV:
    case SIGTERM:
      g_signal_exit_count++;
      break;

#if !defined(_WIN32)
    case SIGTSTP:  // NOLINT
      g_signal_stop_count++;
      break;

    case SIGWINCH:  // NOLINT
      g_signal_resize_count++;
      break;
#endif

    default:
      break;
  }
}

void ExecuteSignalHandlers() {
  int signal_exit_count = g_signal_exit_count.exchange(0);
  while (signal_exit_count--) {
    ScreenInteractive::Private::Signal(*g_active_screen, SIGABRT);
  }

#if !defined(_WIN32)
  int signal_stop_count = g_signal_stop_count.exchange(0);
  while (signal_stop_count--) {
    ScreenInteractive::Private::Signal(*g_active_screen, SIGTSTP);
  }

  int signal_resize_count = g_signal_resize_count.exchange(0);
  while (signal_resize_count--) {
    ScreenInteractive::Private::Signal(*g_active_screen, SIGWINCH);
  }
#endif
}

void InstallSignalHandler(int sig) {
  auto old_signal_handler = std::signal(sig, RecordSignal);
  on_exit_functions.emplace(
      [=] { std::ignore = std::signal(sig, old_signal_handler); });
}

// CSI: Introductor de Secuencia de Control
                                  //
// DCS: Cadena de Control de Dispositivo
const std::string ST = "\x1b\\";  // NOLINT

// DECRQSS: Solicitud de Cadena de Estado
// DECSCUSR: Establecer Estilo de Cursor

// DEC: Corporación de Equipos Digitales
enum class DECMode : std::uint16_t {
  kLineWrap = 7,
  kCursor = 25,

  kMouseX10 = 9,
  kMouseVt200 = 1000,
  kMouseVt200Highlight = 1001,

  kMouseBtnEventMouse = 1002,
  kMouseAnyEvent = 1003,

  kMouseUtf8 = 1005,
  kMouseSgrExtMode = 1006,
  kMouseUrxvtMode = 1015,
  kMouseSgrPixelsMode = 1016,
  kAlternateScreen = 1049,
};

// Informe de estado del dispositivo (DSR) {
enum class DSRMode : std::uint8_t {
  kCursor = 6,
};

std::string Serialize(const std::vector<DECMode>& parameters) {
  bool first = true;
  std::string out;
  for (const DECMode parameter : parameters) {
    if (!first) {
      out += ";";
    }
    out += std::to_string(int(parameter));
    first = false;
  }
  return out;
}

// Establecer Modo Privado DEC (DECSET)
std::string Set(const std::vector<DECMode>& parameters) {
  return CSI + "?" + Serialize(parameters) + "h";
}

// Restablecer Modo Privado DEC (DECRST)
std::string Reset(const std::vector<DECMode>& parameters) {
  return CSI + "?" + Serialize(parameters) + "l";
}

// Informe de Estado del Dispositivo (DSR)
std::string DeviceStatusReport(DSRMode ps) {
  return CSI + std::to_string(int(ps)) + "n";
}

class CapturedMouseImpl : public CapturedMouseInterface {
 public:
  explicit CapturedMouseImpl(std::function<void(void)> callback)
      : callback_(std::move(callback)) {}
  ~CapturedMouseImpl() override { callback_(); }
  CapturedMouseImpl(const CapturedMouseImpl&) = delete;
  CapturedMouseImpl(CapturedMouseImpl&&) = delete;
  CapturedMouseImpl& operator=(const CapturedMouseImpl&) = delete;
  CapturedMouseImpl& operator=(CapturedMouseImpl&&) = delete;

 private:
  std::function<void(void)> callback_;
};

}  // namespace

ScreenInteractive::ScreenInteractive(Dimension dimension,
                                     int dimx,
                                     int dimy,
                                     bool use_alternative_screen)
    : Screen(dimx, dimy),
      dimension_(dimension),
      use_alternative_screen_(use_alternative_screen) {
  internal_ = std::make_unique<Internal>(
      [&](Event event) { PostEvent(std::move(event)); });
}

// static
ScreenInteractive ScreenInteractive::FixedSize(int dimx, int dimy) {
  return {
      Dimension::Fixed,
      dimx,
      dimy,
      /*use_alternative_screen=*/false,
  };
}

/// Crea una ScreenInteractive que ocupa todo el tamaño del terminal. Esto utiliza el
/// búfer de pantalla alternativo para evitar interferir con el contenido del terminal.
/// @note Es lo mismo que `ScreenInteractive::FullscreenAlternateScreen()`
// static
ScreenInteractive ScreenInteractive::Fullscreen() {
  return FullscreenAlternateScreen();
}

/// Crea una ScreenInteractive que ocupa todo el tamaño del terminal. Se utiliza el
/// búfer de pantalla principal. Esto significa que si el terminal se redimensiona, el contenido
/// anterior podría desordenarse con el contenido del terminal.
// static
ScreenInteractive ScreenInteractive::FullscreenPrimaryScreen() {
  auto terminal = Terminal::Size();
  return {
      Dimension::Fullscreen,
      terminal.dimx,
      terminal.dimy,
      /*use_alternative_screen=*/false,
  };
}

/// Crea una ScreenInteractive que ocupa todo el tamaño del terminal. Esto utiliza el
/// búfer de pantalla alternativo para evitar interferir con el contenido del terminal.
// static
ScreenInteractive ScreenInteractive::FullscreenAlternateScreen() {
  auto terminal = Terminal::Size();
  return {
      Dimension::Fullscreen,
      terminal.dimx,
      terminal.dimy,
      /*use_alternative_screen=*/true,
  };
}

/// Crea una ScreenInteractive cuyo ancho coincide con el ancho de salida del terminal y
/// cuya altura coincide con el componente que se está dibujando.
// static
ScreenInteractive ScreenInteractive::TerminalOutput() {
  auto terminal = Terminal::Size();
  return {
      Dimension::TerminalOutput,
      terminal.dimx,
      terminal.dimy,  // Best guess.
      /*use_alternative_screen=*/false,
  };
}

ScreenInteractive::~ScreenInteractive() = default;

/// Crea una ScreenInteractive cuyo ancho y altura coinciden con el componente
/// que se está dibujando.
// static
ScreenInteractive ScreenInteractive::FitComponent() {
  auto terminal = Terminal::Size();
  return {
      Dimension::FitComponent,
      terminal.dimx,  // Best guess.
      terminal.dimy,  // Best guess.
      false,
  };
}

/// @brief Establece si el ratón es rastreado y se informan los eventos.
/// se llama fuera del bucle principal. Por ejemplo, `ScreenInteractive::Loop(...)`.
/// @param enable Si se habilita el seguimiento de eventos del ratón.
/// @note Esto debe llamarse fuera del bucle principal. Por ejemplo, antes de llamar a
/// `ScreenInteractive::Loop`.
/// @note El seguimiento del ratón está habilitado por defecto.
/// @note El seguimiento del ratón solo es compatible con terminales que lo soporten.
///
/// ### Ejemplo
///
/// ```cpp
/// auto screen = ScreenInteractive::TerminalOutput();
/// screen.TrackMouse(false);
/// screen.Loop(component);
/// ```
void ScreenInteractive::TrackMouse(bool enable) {
  track_mouse_ = enable;
}

/// @brief Habilita o deshabilita el manejo automático de entrada por tubería.
/// Cuando está habilitado, FTXUI detectará la entrada por tubería y redirigirá stdin desde /dev/tty
/// para la entrada de teclado, permitiendo que las aplicaciones lean datos canalizados mientras
/// siguen recibiendo eventos interactivos de teclado.
/// @param enable Si se habilita el manejo de entrada por tubería. El valor predeterminado es verdadero.
/// @note Esto debe llamarse antes de Loop().
/// @note Esta característica está habilitada por defecto.
/// @note Esta característica solo está disponible en sistemas POSIX (Linux/macOS).
void ScreenInteractive::HandlePipedInput(bool enable) {
  handle_piped_input_ = enable;
}

/// @brief Añade una tarea al bucle principal.
/// Se ejecutará más tarde, después de todas las demás tareas programadas.
void ScreenInteractive::Post(Task task) {
  internal_->task_runner.PostTask([this, task = std::move(task)]() mutable {
    HandleTask(component_, task);
  });
}

/// @brief Añade un evento al bucle principal.
/// Se ejecutará más tarde, después de todos los demás eventos programados.
void ScreenInteractive::PostEvent(Event event) {
  Post(event);
}

/// @brief Añade una tarea para dibujar la pantalla una vez más, hasta que todas las animaciones
/// hayan terminado.
void ScreenInteractive::RequestAnimationFrame() {
  if (animation_requested_) {
    return;
  }
  animation_requested_ = true;
  auto now = animation::Clock::now();
  const auto time_histeresis = std::chrono::milliseconds(33);
  if (now - previous_animation_time_ >= time_histeresis) {
    previous_animation_time_ = now;
  }
}

/// @brief Intenta obtener el bloqueo único para poder capturar el ratón.
/// @return Un bloqueo único si el ratón no está ya capturado, de lo contrario un
/// nulo.
CapturedMouse ScreenInteractive::CaptureMouse() {
  if (mouse_captured) {
    return nullptr;
  }
  mouse_captured = true;
  return std::make_unique<CapturedMouseImpl>(
      [this] { mouse_captured = false; });
}

/// @brief Ejecuta el bucle principal.
/// @param component El componente a dibujar.
void ScreenInteractive::Loop(Component component) {  // NOLINT
  class Loop loop(this, std::move(component));
  loop.Run();
}

/// @brief Devuelve si el bucle principal ha terminado.
bool ScreenInteractive::HasQuitted() {
  return quit_;
}

// private
void ScreenInteractive::PreMain() {
  // Suspender la pantalla previamente activa:
  if (g_active_screen) {
    std::swap(suspended_screen_, g_active_screen);
    // Restablecer la posición del cursor a la parte superior de la pantalla y borrar la pantalla.
    suspended_screen_->ResetCursorPosition();
    std::cout << suspended_screen_->ResetPosition(/*clear=*/true);
    suspended_screen_->dimx_ = 0;
    suspended_screen_->dimy_ = 0;

    // Restablecer las dimensiones para forzar el dibujo de la pantalla de nuevo la próxima vez:
    suspended_screen_->Uninstall();
  }

  // Esta pantalla está ahora activa:
  g_active_screen = this;
}

// private
void ScreenInteractive::PostMain() {
  // Colocar la posición del cursor al final del dibujo.
  ResetCursorPosition();

  g_active_screen = nullptr;

  // Restaurar pantalla suspendida.
  if (suspended_screen_) {
    // Borrar pantalla y colocar el cursor al principio del dibujo.
    std::cout << ResetPosition(/*clear=*/true);
    dimx_ = 0;
    dimy_ = 0;
    Uninstall();
    std::swap(g_active_screen, suspended_screen_);
    g_active_screen->Install();
  } else {
    Uninstall();

    std::cout << '\r';
    // Al salir finalmente, mantener el dibujo actual y restablecer la posición del cursor una
    // línea después.
    if (!use_alternative_screen_) {
      std::cout << '\n';
      std::cout << std::flush;
    }
  }
}

/// @brief Decora una función. Se ejecuta de la misma manera, pero con los
/// hooks del terminal de la pantalla activa temporalmente desinstalados durante su ejecución.
/// @param fn La función a decorar.
Closure ScreenInteractive::WithRestoredIO(Closure fn) {  // NOLINT
  return [this, fn] {
    Uninstall();
    fn();
    Install();
  };
}

/// @brief Fuerza a FTXUI a manejar o no Ctrl-C, incluso si el componente
/// captura el Event::CtrlC.
void ScreenInteractive::ForceHandleCtrlC(bool force) {
  force_handle_ctrl_c_ = force;
}

/// @brief Fuerza a FTXUI a manejar o no Ctrl-Z, incluso si el componente
/// captura el Event::CtrlZ.
void ScreenInteractive::ForceHandleCtrlZ(bool force) {
  force_handle_ctrl_z_ = force;
}

/// @brief Devuelve el contenido de la selección actual
std::string ScreenInteractive::GetSelection() {
  if (!selection_) {
    return "";
  }
  return selection_->GetParts();
}

void ScreenInteractive::SelectionChange(std::function<void()> callback) {
  selection_on_change_ = std::move(callback);
}

/// @brief Devuelve la pantalla actualmente activa, o nulo si no hay ninguna.
// static
ScreenInteractive* ScreenInteractive::Active() {
  return g_active_screen;
}

// private
void ScreenInteractive::Install() {
  frame_valid_ = false;

  // Vaciar el búfer de stdout para asegurar que todo lo que el usuario haya impreso antes
  // se aplique completamente antes de que comencemos a modificar la configuración del terminal. Esto
  // es importante porque estamos usando dos canales diferentes (stdout vs
  // termios/WinAPI) para comunicarnos con el emulador de terminal a continuación. Ver
  // https://github.com/ArthurSonzogni/FTXUI/issues/846
  Flush();

  InstallPipedInputHandling();

  // Después de desinstalar la nueva configuración, vaciarla al terminal para
  // asegurar que se aplique completamente:
  on_exit_functions.emplace([] { Flush(); });

  on_exit_functions.emplace([this] { ExitLoopClosure()(); });

  // Solicitar al terminal que informe la forma actual del cursor. Lo restauraremos
  // al salir.
  std::cout << DECRQSS_DECSCUSR;
  on_exit_functions.emplace([this] {
    std::cout << "\033[?25h";  // Habilitar cursor.
    std::cout << "\033[" + std::to_string(cursor_reset_shape_) + " q";
  });

  // Instalar manejadores de señal para restaurar el estado del terminal al salir. Los manejadores
  // de señal predeterminados se restauran al salir.
  for (const int signal : {SIGTERM, SIGSEGV, SIGINT, SIGILL, SIGABRT, SIGFPE}) {
    InstallSignalHandler(signal);
  }

// Guardar la antigua configuración del terminal y restaurarla al salir.
#if defined(_WIN32)
  // Habilitar el procesamiento VT en stdout y stdin
  auto stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  auto stdin_handle = GetStdHandle(STD_INPUT_HANDLE);

  DWORD out_mode = 0;
  DWORD in_mode = 0;
  GetConsoleMode(stdout_handle, &out_mode);
  GetConsoleMode(stdin_handle, &in_mode);
  on_exit_functions.push([=] { SetConsoleMode(stdout_handle, out_mode); });
  on_exit_functions.push([=] { SetConsoleMode(stdin_handle, in_mode); });

  // https://docs.microsoft.com/en-us/windows/console/setconsolemode
  const int enable_virtual_terminal_processing = 0x0004;
  const int disable_newline_auto_return = 0x0008;
  out_mode |= enable_virtual_terminal_processing;
  out_mode |= disable_newline_auto_return;

  // https://docs.microsoft.com/en-us/windows/console/setconsolemode
  const int enable_line_input = 0x0002;
  const int enable_echo_input = 0x0004;
  const int enable_virtual_terminal_input = 0x0200;
  const int enable_window_input = 0x0008;
  in_mode &= ~enable_echo_input;
  in_mode &= ~enable_line_input;
  in_mode |= enable_virtual_terminal_input;
  in_mode |= enable_window_input;

  SetConsoleMode(stdin_handle, in_mode);
  SetConsoleMode(stdout_handle, out_mode);
#else  // POSIX (Linux & Mac)
  for (const int signal : {SIGWINCH, SIGTSTP}) {
    InstallSignalHandler(signal);
  }

  struct termios terminal;  // NOLINT
  tcgetattr(tty_fd_, &terminal);
  on_exit_functions.emplace([terminal = terminal, tty_fd_ = tty_fd_] {
    tcsetattr(tty_fd_, TCSANOW, &terminal);
  });

  // Habilitar el modo de entrada de terminal en crudo
  terminal.c_iflag &= ~IGNBRK;  // Deshabilitar la ignorancia de la condición de interrupción
  terminal.c_iflag &= ~BRKINT;  // Deshabilitar que la interrupción cause el
                                // vaciado de entrada y salida
  terminal.c_iflag &= ~PARMRK;  // Deshabilitar la marcación de errores de paridad.
  terminal.c_iflag &= ~ISTRIP;  // Deshabilitar la eliminación del 8º bit de los caracteres.
  terminal.c_iflag &= ~INLCR;   // Deshabilitar el mapeo de NL a CR.
  terminal.c_iflag &= ~IGNCR;   // Deshabilitar la ignorancia de CR.
  terminal.c_iflag &= ~ICRNL;   // Deshabilitar el mapeo de CR a NL.
  terminal.c_iflag &= ~IXON;    // Deshabilitar el control de flujo XON/XOFF en la salida

  terminal.c_lflag &= ~ECHO;    // Deshabilitar el eco de los caracteres de entrada.
  terminal.c_lflag &= ~ECHONL;  // Deshabilitar el eco de los caracteres de nueva línea.
  terminal.c_lflag &= ~ICANON;  // Deshabilitar el modo canónico.
  terminal.c_lflag &= ~ISIG;    // Deshabilitar el envío de señal al pulsar:
                                // -     => DSUSP
                                // - C-Z => SUSP
                                // - C-C => INTR
                                // - C-d => QUIT
  terminal.c_lflag &= ~IEXTEN;  // Deshabilitar el procesamiento de entrada extendido
  terminal.c_cflag |= CS8;      // 8 bits por byte

  terminal.c_cc[VMIN] = 0;   // Número mínimo de caracteres para lectura no canónica.
  terminal.c_cc[VTIME] = 0;  // Tiempo de espera en deciseGUNDOS para lectura no canónica.

  tcsetattr(tty_fd_, TCSANOW, &terminal);

#endif

  auto enable = [&](const std::vector<DECMode>& parameters) {
    std::cout << Set(parameters);
    on_exit_functions.emplace([=] { std::cout << Reset(parameters); });
  };

  auto disable = [&](const std::vector<DECMode>& parameters) {
    std::cout << Reset(parameters);
    on_exit_functions.emplace([=] { std::cout << Set(parameters); });
  };

  if (use_alternative_screen_) {
    enable({
        DECMode::kAlternateScreen,
    });
  }

  disable({
      // DECMode::kCursor,
      DECMode::kLineWrap,
  });

  if (track_mouse_) {
    enable({DECMode::kMouseVt200});
    enable({DECMode::kMouseAnyEvent});
    enable({DECMode::kMouseUrxvtMode});
    enable({DECMode::kMouseSgrExtMode});
  }

  // Después de instalar la nueva configuración, vaciarla al terminal para
  // asegurar que se aplique completamente:
  Flush();

  quit_ = false;

  PostAnimationTask();
}

void ScreenInteractive::InstallPipedInputHandling() {
#if !defined(_WIN32) && !defined(__EMSCRIPTEN__)
  tty_fd_ = STDIN_FILENO;
  // Manejar la redirección de entrada canalizada si la aplicación la habilita explícitamente.
  // Esto permite que las aplicaciones lean datos de stdin mientras siguen recibiendo
  // entrada de teclado del terminal para uso interactivo.
  if (!handle_piped_input_) {
    return;
  }

  // Si stdin es un terminal, no necesitamos abrir /dev/tty.
  if (isatty(STDIN_FILENO)) {
    return;
  }

  // Abrir /dev/tty para la entrada del teclado.
  tty_fd_ = open("/dev/tty", O_RDONLY);
  if (tty_fd_ < 0) {
    // Falló al abrir /dev/tty (contenedores, sistemas sin cabeza, etc.)
    tty_fd_ = STDIN_FILENO;  // Volver a stdin.
    return;
  }

  // Cerrar el descriptor de archivo /dev/tty al salir.
  on_exit_functions.emplace([this] {
    close(tty_fd_);
    tty_fd_ = -1;
  });
#endif
}

// private
void ScreenInteractive::Uninstall() {
  ExitNow();
  OnExit();
}

// private
// NOLINTNEXTLINE
void ScreenInteractive::RunOnceBlocking(Component component) {
  // Establecer FPS a 60 como máximo.
  const auto time_per_frame = std::chrono::microseconds(16666);  // 1s / 60fps

  auto time = std::chrono::steady_clock::now();
  size_t executed_task = internal_->task_runner.ExecutedTasks();

  // Esperar a que se ejecute al menos una tarea.
  while (executed_task == internal_->task_runner.ExecutedTasks() &&
         !HasQuitted()) {
    RunOnce(component);

    const auto now = std::chrono::steady_clock::now();
    const auto delta = now - time;
    time = now;

    if (delta < time_per_frame) {
      const auto sleep_duration = time_per_frame - delta;
      std::this_thread::sleep_for(sleep_duration);
    }
  }
}

// private
void ScreenInteractive::RunOnce(Component component) {
  AutoReset set_component(&component_, component);
  ExecuteSignalHandlers();
  FetchTerminalEvents();

  // Ejecutar las tareas pendientes de la cola.
  const size_t executed_task = internal_->task_runner.ExecutedTasks();
  internal_->task_runner.RunUntilIdle();
  // Si no se ejecutó ninguna tarea, podemos regresar temprano sin volver a dibujar la pantalla.
  if (executed_task == internal_->task_runner.ExecutedTasks()) {
    return;
  }

  ExecuteSignalHandlers();
  Draw(component);

  if (selection_data_previous_ != selection_data_) {
    selection_data_previous_ = selection_data_;
    if (selection_on_change_) {
      selection_on_change_();
      Post(Event::Custom);
    }
  }
}

// private
// NOLINTNEXTLINE
void ScreenInteractive::HandleTask(Component component, Task& task) {
  std::visit(
      [&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        // clang-format off
    // Manejar Evento.
    if constexpr (std::is_same_v<T, Event>) {

      if (arg.is_cursor_position()) {
        cursor_x_ = arg.cursor_x();
        cursor_y_ = arg.cursor_y();
        return;
      }

      if (arg.is_cursor_shape()) {
        cursor_reset_shape_= arg.cursor_shape();
        return;
      }

      if (arg.is_mouse()) {
        arg.mouse().x -= cursor_x_;
        arg.mouse().y -= cursor_y_;
      }

      arg.screen_ = this;

      bool handled = component->OnEvent(arg);

      handled = HandleSelection(handled, arg);

      if (arg == Event::CtrlC && (!handled || force_handle_ctrl_c_)) {
        RecordSignal(SIGABRT);
      }

#if !defined(_WIN32)
      if (arg == Event::CtrlZ && (!handled || force_handle_ctrl_z_)) {
        RecordSignal(SIGTSTP);
      }
#endif
      
      frame_valid_ = false;
      return;
    }

    // Manejar callback
    if constexpr (std::is_same_v<T, Closure>) {
      arg();
      return;
    }

    // Manejar Animación
    if constexpr (std::is_same_v<T, AnimationTask>) {
      if (!animation_requested_) {
        return;
      }

      animation_requested_ = false;
      const animation::TimePoint now = animation::Clock::now();
      const animation::Duration delta = now - previous_animation_time_;
      previous_animation_time_ = now;

      animation::Params params(delta);
      component->OnAnimation(params);
      frame_valid_ = false;
      return;
    }
  },
  task);
  // clang-format on
}

// private
bool ScreenInteractive::HandleSelection(bool handled, Event event) {
  if (handled) {
    selection_pending_ = nullptr;
    selection_data_.empty = true;
    selection_ = nullptr;
    return true;
  }

  if (!event.is_mouse()) {
    return false;
  }

  auto& mouse = event.mouse();
  if (mouse.button != Mouse::Left) {
    return false;
  }

  if (mouse.motion == Mouse::Pressed) {
    selection_pending_ = CaptureMouse();
    selection_data_.start_x = mouse.x;
    selection_data_.start_y = mouse.y;
    selection_data_.end_x = mouse.x;
    selection_data_.end_y = mouse.y;
    return false;
  }

  if (!selection_pending_) {
    return false;
  }

  if (mouse.motion == Mouse::Moved) {
    if ((mouse.x != selection_data_.end_x) ||
        (mouse.y != selection_data_.end_y)) {
      selection_data_.end_x = mouse.x;
      selection_data_.end_y = mouse.y;
      selection_data_.empty = false;
    }

    return true;
  }

  if (mouse.motion == Mouse::Released) {
    selection_pending_ = nullptr;
    selection_data_.end_x = mouse.x;
    selection_data_.end_y = mouse.y;
    selection_data_.empty = false;
    return true;
  }

  return false;
}

// private
// NOLINTNEXTLINE
void ScreenInteractive::Draw(Component component) {
  if (frame_valid_) {
    return;
  }
  auto document = component->Render();
  int dimx = 0;
  int dimy = 0;
  auto terminal = Terminal::Size();
  document->ComputeRequirement();
  switch (dimension_) {
    case Dimension::Fixed:
      dimx = dimx_;
      dimy = dimy_;
      break;
    case Dimension::TerminalOutput:
      dimx = terminal.dimx;
      dimy = util::clamp(document->requirement().min_y, 0, terminal.dimy);
      break;
    case Dimension::Fullscreen:
      dimx = terminal.dimx;
      dimy = terminal.dimy;
      break;
    case Dimension::FitComponent:
      dimx = util::clamp(document->requirement().min_x, 0, terminal.dimy);
      dimy = util::clamp(document->requirement().min_y, 0, terminal.dimy);
      break;
  }

  const bool resized = frame_count_ == 0 || (dimx != dimx_) || (dimy != dimy_);
  ResetCursorPosition();
  std::cout << ResetPosition(/*clear=*/resized);

  // Si el ancho del terminal disminuye, el emulador de terminal comenzará a ajustar
  // líneas y ensuciará la pantalla. Debemos limpiarla por completo.
  if ((dimx < dimx_) && !use_alternative_screen_) {
    std::cout << "\033[J";  // limpiar la salida del terminal
    std::cout << "\033[H";  // mover el cursor a la posición inicial
  }

  // Redimensionar la pantalla si es necesario.
  if (resized) {
    dimx_ = dimx;
    dimy_ = dimy;
    pixels_ = std::vector<std::vector<Pixel>>(dimy, std::vector<Pixel>(dimx));
    cursor_.x = dimx_ - 1;
    cursor_.y = dimy_ - 1;
  }

  // Solicitar periódicamente al emulador de terminal la posición del marco en relación con
  // la pantalla. Esto es útil para convertir la posición del ratón informada en
  // coordenadas de la pantalla a coordenadas del marco.
#if defined(FTXUI_MICROSOFT_TERMINAL_FALLBACK)
  // El terminal de Microsoft sufre de un [bug]. Al informar la posición del cursor,
  // varias secuencias de salida se mezclan generando basura.
  // Esto hace que el usuario de FTXUI vea algunas secuencias "1;1;R" en el componente Input.
  // Ver [issue]. La solución es solicitar la posición del cursor con menos
  // frecuencia. [bug]: https://github.com/microsoft/terminal/pull/7583 [issue]:
  // https://github.com/ArthurSonzogni/FTXUI/issues/136
  static int i = -3;
  ++i;
  if (!use_alternative_screen_ && (i % 150 == 0)) {  // NOLINT
    std::cout << DeviceStatusReport(DSRMode::kCursor);
  }
#else
  static int i = -3;
  ++i;
  if (!use_alternative_screen_ &&
      (previous_frame_resized_ || i % 40 == 0)) {  // NOLINT
    std::cout << DeviceStatusReport(DSRMode::kCursor);
  }
#endif
  previous_frame_resized_ = resized;

  selection_ = selection_data_.empty
                   ? std::make_unique<Selection>()
                   : std::make_unique<Selection>(
                         selection_data_.start_x, selection_data_.start_y,  //
                         selection_data_.end_x, selection_data_.end_y);
  Render(*this, document.get(), *selection_);

  // Establecer la posición del cursor para el usuario que utiliza herramientas para insertar caracteres CJK.
  {
    const int dx = dimx_ - 1 - cursor_.x + int(dimx_ != terminal.dimx);
    const int dy = dimy_ - 1 - cursor_.y;

    set_cursor_position.clear();
    reset_cursor_position.clear();

    if (dy != 0) {
      set_cursor_position += "\x1B[" + std::to_string(dy) + "A";
      reset_cursor_position += "\x1B[" + std::to_string(dy) + "B";
    }

    if (dx != 0) {
      set_cursor_position += "\x1B[" + std::to_string(dx) + "D";
      reset_cursor_position += "\x1B[" + std::to_string(dx) + "C";
    }

    if (cursor_.shape == Cursor::Hidden) {
      set_cursor_position += "\033[?25l";
    } else {
      set_cursor_position += "\033[?25h";
      set_cursor_position +=
          "\033[" + std::to_string(int(cursor_.shape)) + " q";
    }
  }

  std::cout << ToString() << set_cursor_position;
  Flush();
  Clear();
  frame_valid_ = true;
  frame_count_++;
}

// private
void ScreenInteractive::ResetCursorPosition() {
  std::cout << reset_cursor_position;
  reset_cursor_position = "";
}

/// @brief Devuelve una función para salir del bucle principal.
Closure ScreenInteractive::ExitLoopClosure() {
  return [this] { Exit(); };
}

/// @brief Sale del bucle principal.
void ScreenInteractive::Exit() {
  Post([this] { ExitNow(); });
}

// private:
void ScreenInteractive::ExitNow() {
  quit_ = true;
}

// private:
void ScreenInteractive::Signal(int signal) {
  if (signal == SIGABRT) {
    Exit();
    return;
  }

// Windows no soporta SIGTSTP / SIGWINCH
#if !defined(_WIN32)
  if (signal == SIGTSTP) {
    Post([&] {
      ResetCursorPosition();
      std::cout << ResetPosition(/*clear*/ true);  // Cursor to the beginning
      Uninstall();
      dimx_ = 0;
      dimy_ = 0;
      Flush();
      std::ignore = std::raise(SIGTSTP);
      Install();
    });
    return;
  }

  if (signal == SIGWINCH) {
    Post(Event::Special({0}));
    return;
  }
#endif
}

void ScreenInteractive::FetchTerminalEvents() {
#if defined(_WIN32)
  auto get_input_records = [&]() -> std::vector<INPUT_RECORD> {
    // Comprobar si hay entrada en la consola.
    auto console = GetStdHandle(STD_INPUT_HANDLE);
    DWORD number_of_events = 0;
    if (!GetNumberOfConsoleInputEvents(console, &number_of_events)) {
      return std::vector<INPUT_RECORD>();
    }
    if (number_of_events <= 0) {
      // No hay entrada, regresar.
      return std::vector<INPUT_RECORD>();
    }
    // Leer los eventos de entrada.
    std::vector<INPUT_RECORD> records(number_of_events);
    DWORD number_of_events_read = 0;
    if (!ReadConsoleInput(console, records.data(), (DWORD)records.size(),
                          &number_of_events_read)) {
      return std::vector<INPUT_RECORD>();
    }
    records.resize(number_of_events_read);
    return records;
  };

  auto records = get_input_records();
  if (records.size() == 0) {
    const auto timeout =
        std::chrono::steady_clock::now() - internal_->last_char_time;
    const size_t timeout_microseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(timeout).count();
    internal_->terminal_input_parser.Timeout(timeout_microseconds);
    return;
  }
  internal_->last_char_time = std::chrono::steady_clock::now();

  // Convertir los eventos de entrada a eventos FTXUI.
  // Para cada evento, llamamos al analizador de entrada del terminal para convertirlo a
  // Evento.
  for (const auto& r : records) {
    switch (r.EventType) {
      case KEY_EVENT: {
        auto key_event = r.Event.KeyEvent;
        // ignorar eventos de tecla UP
        if (key_event.bKeyDown == FALSE) {
          continue;
        }
        std::wstring wstring;
        wstring += key_event.uChar.UnicodeChar;
        for (auto it : to_string(wstring)) {
          internal_->terminal_input_parser.Add(it);
        }
      } break;
      case WINDOW_BUFFER_SIZE_EVENT:
        Post(Event::Special({0}));
        break;
      case MENU_EVENT:
      case FOCUS_EVENT:
      case MOUSE_EVENT:
        // TODO(mauve): Implementar más tarde.
        break;
    }
  }
#elif defined(__EMSCRIPTEN__)
  // Leer caracteres del terminal.
  // Lo configuramos para que no sea bloqueante.
  std::array<char, 128> out{};
  size_t l = read(STDIN_FILENO, out.data(), out.size());
  if (l == 0) {
    const auto timeout =
        std::chrono::steady_clock::now() - internal_->last_char_time;
    const size_t timeout_microseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(timeout).count();
    internal_->terminal_input_parser.Timeout(timeout_microseconds);
    return;
  }
  internal_->last_char_time = std::chrono::steady_clock::now();

  // Convertir los caracteres a eventos.
  for (size_t i = 0; i < l; ++i) {
    internal_->terminal_input_parser.Add(out[i]);
  }
#else  // POSIX (Linux & Mac)
  if (!CheckStdinReady(tty_fd_)) {
    const auto timeout =
        std::chrono::steady_clock::now() - internal_->last_char_time;
    const size_t timeout_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count();
    internal_->terminal_input_parser.Timeout(timeout_ms);
    return;
  }
  internal_->last_char_time = std::chrono::steady_clock::now();

  // Leer caracteres del terminal.
  std::array<char, 128> out{};
  size_t l = read(tty_fd_, out.data(), out.size());

  // Convertir los caracteres a eventos.
  for (size_t i = 0; i < l; ++i) {
    internal_->terminal_input_parser.Add(out[i]);
  }
#endif
}

void ScreenInteractive::PostAnimationTask() {
  Post(AnimationTask());

  // Repetir la tarea de animación cada 15ms. Esto corresponde a una velocidad de fotogramas
  // de alrededor de 66fps.
  internal_->task_runner.PostDelayedTask([this] { PostAnimationTask(); },
                                         std::chrono::milliseconds(15));
}

bool ScreenInteractive::SelectionData::operator==(
    const ScreenInteractive::SelectionData& other) const {
  if (empty && other.empty) {
    return true;
  }
  if (empty || other.empty) {
    return false;
  }
  return start_x == other.start_x && start_y == other.start_y &&
         end_x == other.end_x && end_y == other.end_y;
}

bool ScreenInteractive::SelectionData::operator!=(
    const ScreenInteractive::SelectionData& other) const {
  return !(*this == other);
}

}  // namespace ftxui.
