// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>  // for any_of, copy, max, min
#include <array>      // for array
#include <atomic>
#include <chrono>  // for operator-, milliseconds, operator>=, duration, common_type<>::type, time_point
#include <csignal>  // for signal, SIGTSTP, SIGABRT, SIGWINCH, raise, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM, __sighandler_t, size_t
#include <cstdint>
#include <cstdio>  // for fileno, stdin
#include <ftxui/component/app.hpp>
#include <ftxui/component/task.hpp>  // for Task, Closure, AnimationTask
#include <ftxui/screen/screen.hpp>  // for Cell, Screen::Cursor, Screen, Screen::Cursor::Hidden
#include <functional>        // for function
#include <initializer_list>  // for initializer_list
#include <iostream>  // for cout, ostream, operator<<, basic_ostream, endl, flush
#include <map>
#include <memory>
#include <stack>  // for stack
#include <string>
#include <string_view>
#include <thread>  // for thread, sleep_for
#include <tuple>   // for _Swallow_assign, ignore
#include <type_traits>
#include <utility>  // for move, swap
#include <variant>  // for visit, variant
#include <vector>   // for vector

#include "ftxui/component/animation.hpp"  // for TimePoint, Clock, Duration, Params, RequestAnimationFrame
#include "ftxui/component/captured_mouse.hpp"  // for CapturedMouse, CapturedMouseInterface
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/component/loop.hpp"            // for Loop
#include "ftxui/component/multi_receiver_buffer.hpp"
#include "ftxui/component/task_runner.hpp"
#include "ftxui/component/terminal_input_parser.hpp"  // for TerminalInputParser
#include "ftxui/dom/node.hpp"                         // for Node, Render
#include "ftxui/screen/cell.hpp"                      // for Cell
#include "ftxui/screen/terminal.hpp"                  // for Dimensions, Size
#include "ftxui/screen/util.hpp"                      // for util::clamp
#include "ftxui/util/autoreset.hpp"                   // for AutoReset

#if defined(_WIN32)
#define DEFINE_CONSOLEV2_PROPERTIES
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <io.h>
#include <windows.h>
#else
#include <fcntl.h>
#include <poll.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <termios.h>  // for tcsetattr, termios, tcgetattr, TCSANOW, cc_t, ECHO, ICANON, VMIN, VTIME
#include <unistd.h>  // for STDIN_FILENO, STDOUT_FILENO, read
#endif

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

namespace ftxui {

enum class AppDimension {
  FitComponent,
  Fixed,
  Fullscreen,
  TerminalOutput,
};

namespace animation {
void RequestAnimationFrame() {
  auto* screen = App::Active();
  if (screen) {
    screen->RequestAnimationFrame();
  }
}
}  // namespace animation

#if defined(__EMSCRIPTEN__)
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
#endif

struct App::Internal {
  App* public_;

  App* suspended_screen_ = nullptr;
  const AppDimension dimension_;
  const bool use_alternative_screen_;

  bool track_mouse_ = true;

  std::string set_cursor_position_;
  std::string reset_cursor_position_;

  std::atomic<bool> quit_{false};
  bool installed_ = false;
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

  int cursor_reset_shape_ = 1;

  // Estado del manejo de entrada por tubería (solo POSIX)
  bool handle_piped_input_ = true;
  bool is_stdin_a_tty_ = false;
  bool is_stdout_a_tty_ = false;
  // Descriptor de archivo para /dev/tty, usado para el manejo de entrada por tubería.
  int tty_fd_ = -1;

  std::string terminal_name_ = "unknown";
  int terminal_version_ = 0;

  std::string terminal_emulator_name_ = "unknown";
  std::string terminal_emulator_version_ = "unknown";

  std::vector<int> terminal_capabilities_;

  // API de selección:
  CapturedMouse selection_pending_;
  struct SelectionData {
    int start_x = -1;
    int start_y = -1;
    int end_x = -2;
    int end_y = -2;
    bool empty = true;
    bool operator==(const SelectionData& other) const {
      if (empty && other.empty) {
        return true;
      }
      if (empty || other.empty) {
        return false;
      }
      return start_x == other.start_x && start_y == other.start_y &&
             end_x == other.end_x && end_y == other.end_y;
    }
    bool operator!=(const SelectionData& other) const {
      return !(*this == other);
    }
  };
  SelectionData selection_data_;
  SelectionData selection_data_previous_;
  std::unique_ptr<Selection> selection_;
  std::function<void()> selection_on_change_;

  Component component_;

  // Preexistente en Internal:
  TerminalInputParser terminal_input_parser;
  task::TaskRunner task_runner;
  std::chrono::time_point<std::chrono::steady_clock> last_char_time =
      std::chrono::steady_clock::now();
  std::string output_buffer;

  class ThrottledRequest {
   public:
    ThrottledRequest(App::Internal* internal, std::function<void()> send)
        : internal_(internal), send_(std::move(send)) {}

    void Request(bool force = false) {
      if (!internal_->is_stdin_a_tty_) {
        return;
      }

      if (force) {
        Send();
        return;
      }

      // Permitir solo una solicitud pendiente a la vez. Esto es para evitar saturar
      // el terminal con solicitudes.
      if (HasPending()) {
        return;
      }

      const auto now = std::chrono::steady_clock::now();
      if (now - last_request_time_ < std::chrono::milliseconds(500)) {
        // Demasiado pronto desde la última solicitud. Omitirla: la solicitud debe enviarse
        // de forma síncrona desde Draw(), justo después de que el cursor se mueve al
        // origen del fotograma, para que la respuesta del terminal refleje esa
        // posición. Draw() vuelve a llamar a Request() en el siguiente fotograma, así que la
        // solicitud no se pierde, solo se retrasa.
        return;
      }

      Send();
    }

    void OnReply() { pending_request_ = false; }

    bool HasPending() const {
      if (!pending_request_) {
        return false;
      }
      const auto now = std::chrono::steady_clock::now();
      return now - last_sent_time_ < std::chrono::seconds(5);
    }

   private:
    void Send() {
      last_sent_time_ = std::chrono::steady_clock::now();
      last_request_time_ = last_sent_time_;
      pending_request_ = true;
      send_();
    }

    App::Internal* internal_;
    std::function<void()> send_;
    bool pending_request_ = false;
    std::chrono::steady_clock::time_point last_request_time_ =
        std::chrono::steady_clock::now() - std::chrono::hours(1);
    std::chrono::steady_clock::time_point last_sent_time_ =
        std::chrono::steady_clock::now() - std::chrono::hours(1);
  };

  ThrottledRequest cursor_position_request;

  MultiReceiverBuffer<Event> event_buffer;
  std::unique_ptr<MultiReceiverBuffer<Event>::Receiver> main_loop_receiver;

  Internal(App* app, AppDimension dimension, bool use_alternative_screen);

  void ExitNow();
  void Install();
  void Uninstall();
  void PreMain();
  void PostMain();
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
};

namespace {

App* g_active_screen = nullptr;  // NOLINT

std::stack<Closure> on_exit_functions;  // NOLINT

void OnExit() {
  while (!on_exit_functions.empty()) {
    on_exit_functions.top()();
    on_exit_functions.pop();
  }
}

// CSI: Control Sequence Introducer
const std::string CSI = "\x1b[";  // NOLINT
                                  //
// DCS: Device Control String
const std::string DCS = "\x1bP";  // NOLINT

// ST: Terminador de cadena (String Terminator)
const std::string ST = "\x1b\\";  // NOLINT

// DECRQSS: Solicitud de Cadena de Estado
// DECSCUSR: Establecer Estilo de Cursor
const std::string DECRQSS_DECSCUSR = DCS + "$q q" + ST;  // NOLINT

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

#if !defined(_WIN32)
std::atomic<int> g_signal_exit_count = 0;    // NOLINT
std::atomic<int> g_signal_stop_count = 0;    // NOLINT
std::atomic<int> g_signal_resize_count = 0;  // NOLINT
#else
std::atomic<int> g_signal_exit_count = 0;  // NOLINT
#endif

// Rastrea si el terminal está actualmente configurado en modo raw.
// Se usa para evitar la doble restauración en salidas de emergencia y normales.
std::atomic<bool> g_terminal_is_raw{false};

// Almacena la última señal diferida recibida (p. ej. SIGINT, SIGTERM) para
// volver a emitirla durante la desinstalación/salida.
std::atomic<int> g_last_signal{0};  // NOLINT

#if defined(_WIN32)
using SignalHandler = void (*)(int);
// Almacena los manejadores de señal originales antes de que FTXUI instalara los suyos.
std::map<int, SignalHandler> g_old_signal_handlers;

// Almacena los modos de consola originales para restaurarlos durante la salida.
DWORD g_original_stdout_mode = 0;
DWORD g_original_stdin_mode = 0;
bool g_has_original_console_mode = false;
#else
// Almacena las estructuras sigaction originales antes de que FTXUI instalara las suyas.
std::map<int, struct sigaction> g_old_sigactions;

// Almacena la configuración termios original del terminal para restaurarla durante la salida.
struct termios g_original_termios;
bool g_has_original_termios = false;
int g_tty_fd = -1;
#endif

// Restaura el manejador de señal original para la señal dada y vuelve a emitirla.
// Función segura para señales asíncronas.
void RestoreSignalHandlerAndRaise(int signal) {
#if defined(_WIN32)
  auto it = g_old_signal_handlers.find(signal);
  auto old_handler = (it != g_old_signal_handlers.end()) ? it->second : SIG_DFL;
  std::signal(signal, old_handler);
#else
  auto it = g_old_sigactions.find(signal);
  if (it != g_old_sigactions.end()) {
    sigaction(signal, &it->second, nullptr);
  } else {
    struct sigaction sa;
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(signal, &sa, nullptr);
  }
#endif
  std::raise(signal);
}

// Restauración de emergencia del estado del terminal.
// Función segura para señales asíncronas.
void RestoreTerminalEmergency() {
  if (!g_terminal_is_raw.exchange(false)) {
    return;
  }
#if defined(_WIN32)
  if (g_has_original_console_mode) {
    auto stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    auto stdin_handle = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(stdout_handle, g_original_stdout_mode);
    SetConsoleMode(stdin_handle, g_original_stdin_mode);
  }
#else
  if (g_has_original_termios && g_tty_fd >= 0) {
    const char restore_seq[] =
        "\x1b[?25h"    // Mostrar cursor.
        "\x1b[?1049l"  // Cambiar al búfer de pantalla normal.
        "\x1b[?1000l"  // Deshabilitar el seguimiento normal del ratón.
        "\x1b[?1002l"  // Deshabilitar el seguimiento de eventos de botón del ratón.
        "\x1b[?1003l"  // Deshabilitar todo el seguimiento de movimiento del ratón.
        "\x1b[?1006l"  // Deshabilitar el seguimiento SGR del ratón.
        "\x1b[?1015l"  // Deshabilitar el seguimiento Urxvt del ratón.
        "\x1b[?7h";    // Habilitar el ajuste de línea.
    std::ignore = write(STDOUT_FILENO, restore_seq, sizeof(restore_seq) - 1);
    tcsetattr(g_tty_fd, TCSANOW, &g_original_termios);
  }
#endif
}

// Función asíncrona segura para señales
void RecordSignal(int signal) {
  switch (signal) {
    // Terminación anormal (p. ej. abort() o fallo de aserción).
    case SIGABRT:
    // Operación aritmética errónea (p. ej. división por cero).
    case SIGFPE:
    // Instrucción ilegal.
    case SIGILL:
    // Referencia de memoria inválida (fallo de segmentación).
    case SIGSEGV:
#if !defined(_WIN32)
    // Error de bus (p. ej. mala alineación de acceso a memoria).
    case SIGBUS:
    // Llamada al sistema incorrecta.
    case SIGSYS:
#endif
    {
      RestoreTerminalEmergency();
      RestoreSignalHandlerAndRaise(signal);
      break;
    }

    // Interrupción del terminal (p. ej. Ctrl-C).
    case SIGINT:
    // Solicitud de terminación.
    case SIGTERM:
#if !defined(_WIN32)
    // Salida del terminal (p. ej. Ctrl-\, produce volcado de núcleo).
    case SIGQUIT:
    // Colgado detectado en el terminal de control o muerte del proceso de control.
    case SIGHUP:
#endif
      g_last_signal.store(signal);
      g_signal_exit_count++;
      break;

#if !defined(_WIN32)
    // Señal de parada del terminal (p. ej. Ctrl-Z).
    case SIGTSTP:  // NOLINT
      g_signal_stop_count++;
      break;

    // Cambio de tamaño de la ventana del terminal.
    case SIGWINCH:  // NOLINT
      g_signal_resize_count++;
      break;
#endif

    default:
      break;
  }
}

void ExecuteSignalHandlers() {
  if (g_last_signal.load() != 0) {
    App::Private::Signal(*g_active_screen, SIGABRT);
  }

  int signal_exit_count = g_signal_exit_count.exchange(0);
  while (signal_exit_count--) {
    App::Private::Signal(*g_active_screen, SIGABRT);
  }

#if !defined(_WIN32)
  int signal_stop_count = g_signal_stop_count.exchange(0);
  while (signal_stop_count--) {
    App::Private::Signal(*g_active_screen, SIGTSTP);
  }

  int signal_resize_count = g_signal_resize_count.exchange(0);
  while (signal_resize_count--) {
    App::Private::Signal(*g_active_screen, SIGWINCH);
  }
#endif
}

void InstallSignalHandler(int sig) {
#if defined(_WIN32)
  auto old_signal_handler = std::signal(sig, RecordSignal);
  g_old_signal_handlers[sig] = old_signal_handler;
  on_exit_functions.emplace(
      [=] { std::ignore = std::signal(sig, old_signal_handler); });
#else
  struct sigaction sa;
  sa.sa_handler = RecordSignal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  struct sigaction old_sa;
  sigaction(sig, &sa, &old_sa);
  g_old_sigactions[sig] = old_sa;
  on_exit_functions.emplace([=] { sigaction(sig, &old_sa, nullptr); });
#endif
}

}  // namespace

App::Internal::Internal(App* app,
                        AppDimension dimension,
                        bool use_alternative_screen)
    : public_(app),
      dimension_(dimension),
      use_alternative_screen_(use_alternative_screen),
      terminal_input_parser([&](Event event) {
        event_buffer.Push(std::move(event));
      }),
      cursor_position_request(this, [this] {
        TerminalSend(DeviceStatusReport(DSRMode::kCursor));
      }) {
  main_loop_receiver = event_buffer.CreateReceiver();
}

void App::Internal::ExitNow() {
  quit_ = true;
}

void App::Internal::Install() {
  frame_valid_ = false;

  // Vaciar el búfer de stdout para asegurar que todo lo que el usuario haya impreso antes
  // se aplique completamente antes de que comencemos a modificar la configuración del terminal. Esto
  // es importante porque estamos usando dos canales diferentes (stdout vs
  // termios/WinAPI) para comunicarnos con el emulador de terminal a continuación. Ver
  // https://github.com/ArthurSonzogni/FTXUI/issues/846
  TerminalFlush();

  InstallPipedInputHandling();

  // Después de desinstalar la nueva configuración, vaciarla al terminal para
  // asegurar que se aplique completamente:
  on_exit_functions.emplace([this] { TerminalFlush(); });

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
  g_original_stdout_mode = out_mode;
  g_original_stdin_mode = in_mode;
  g_has_original_console_mode = true;
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
#else  // POSIX (Linux y Mac)
  for (const int signal :
       {SIGWINCH, SIGTSTP, SIGBUS, SIGSYS, SIGQUIT, SIGHUP}) {
    InstallSignalHandler(signal);
  }

  struct termios terminal;  // NOLINT
  tcgetattr(tty_fd_, &terminal);
  g_original_termios = terminal;
  g_tty_fd = tty_fd_;
  g_has_original_termios = true;
  on_exit_functions.emplace([terminal = terminal, tty_fd_ = tty_fd_] {
    tcsetattr(tty_fd_, TCSANOW, &terminal);
  });

  // Habilitar el modo de entrada de terminal en crudo
  terminal.c_iflag &= ~IGNBRK;  // Deshabilitar la ignorancia de la condición de interrupción
  terminal.c_iflag &= ~BRKINT;  // Deshabilitar que la interrupción cause el
                                // vaciado de entrada y salida
  terminal.c_iflag &= ~PARMRK;  // Deshabilitar la marcación de errores de paridad.
  terminal.c_iflag &= ~ISTRIP;  // Deshabilitar el recorte del octavo bit de los caracteres.
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
    TerminalSend(Set(parameters));
    on_exit_functions.emplace(
        [this, parameters] { TerminalSend(Reset(parameters)); });
  };

  auto disable = [&](const std::vector<DECMode>& parameters) {
    TerminalSend(Reset(parameters));
    on_exit_functions.emplace(
        [this, parameters] { TerminalSend(Set(parameters)); });
  };

  if (use_alternative_screen_) {
    enable({
        DECMode::kAlternateScreen,
    });
  }

  disable({
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
  TerminalFlush();

  InstallTerminalInfo();

  quit_ = false;

  PostAnimationTask();

  installed_ = true;
  g_terminal_is_raw = true;
}

void App::Internal::Uninstall() {
  g_terminal_is_raw = false;
  installed_ = false;

  // Durante el apagado, esperar todas las respuestas.
  if (is_stdin_a_tty_ && is_stdout_a_tty_) {
    auto closing_receiver =
        event_buffer.CreateReceiverAt(main_loop_receiver->index());
    auto start = std::chrono::steady_clock::now();
    while (cursor_position_request.HasPending()) {
      FetchTerminalEvents();

      while (closing_receiver->Has()) {
        const auto event = closing_receiver->Pop();
        if (event.is_cursor_position()) {
          cursor_x_ = event.cursor_x();
          cursor_y_ = event.cursor_y();
          cursor_position_request.OnReply();
        }
      }

      task_runner.RunUntilIdle();

      if (std::chrono::steady_clock::now() - start >
          std::chrono::milliseconds(400)) {
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }

  OnExit();
}

void App::Internal::PreMain() {
  // Suspender la pantalla previamente activa:
  if (g_active_screen) {
    std::swap(suspended_screen_, g_active_screen);
    // Restablecer la posición del cursor a la parte superior de la pantalla y borrar la pantalla.
    suspended_screen_->internal_->TerminalSend(
        suspended_screen_->internal_->ResetCursorPosition());
    suspended_screen_->ResetPosition(
        suspended_screen_->internal_->output_buffer,
        /*clear=*/true);
    suspended_screen_->dimx_ = 0;
    suspended_screen_->dimy_ = 0;

    // Restablecer las dimensiones para forzar el dibujo de la pantalla de nuevo la próxima vez:
    suspended_screen_->internal_->Uninstall();
  }

  // Esta pantalla está ahora activa:
  g_active_screen = public_;
  g_active_screen->internal_->Install();

  previous_animation_time_ = animation::Clock::now();
}

void App::Internal::PostMain() {
  // Colocar la posición del cursor al final del dibujo.
  TerminalSend(ResetCursorPosition());

  g_active_screen = nullptr;

  // Restaurar pantalla suspendida.
  if (suspended_screen_) {
    // Borrar pantalla y colocar el cursor al principio del dibujo.
    public_->ResetPosition(output_buffer, /*clear=*/true);
    public_->dimx_ = 0;
    public_->dimy_ = 0;
    Uninstall();
    std::swap(g_active_screen, suspended_screen_);
    g_active_screen->internal_->Install();
  } else {
    Uninstall();

    std::cout << "\r";
    // Al salir finalmente, mantener el dibujo actual y restablecer la posición del cursor una
    // línea después.
    if (!use_alternative_screen_) {
      std::cout << "\n";
    }
    std::cout << std::flush;
  }

  int sig = g_last_signal.exchange(0);
  if (sig != 0) {
    RestoreSignalHandlerAndRaise(sig);
  }
}

bool App::Internal::HasQuitted() {
  return quit_;
}

void App::Internal::RunOnce(const Component& component) {
  const AutoReset set_component(&component_, component);
  ExecuteSignalHandlers();
  FetchTerminalEvents();

  while (!quit_ && main_loop_receiver->Has()) {
    public_->Post(main_loop_receiver->Pop());
  }

  // Ejecutar las tareas pendientes de la cola.
  const size_t executed_task = task_runner.ExecutedTasks();
  task_runner.RunUntilIdle();
  // Si no se ejecutó ninguna tarea, podemos regresar temprano sin volver a dibujar la pantalla.
  if (executed_task == task_runner.ExecutedTasks()) {
    return;
  }

  ExecuteSignalHandlers();
  Draw(component);

  if (selection_data_previous_ != selection_data_) {
    selection_data_previous_ = selection_data_;
    if (selection_on_change_) {
      selection_on_change_();
      public_->Post(Event::Custom);
    }
  }
}

void App::Internal::RunOnceBlocking(Component component) {
  // Establecer FPS a 60 como máximo.
  const auto time_per_frame = std::chrono::microseconds(16666);  // 1s / 60fps

  auto time = std::chrono::steady_clock::now();
  const size_t executed_task = task_runner.ExecutedTasks();

  // Esperar a que se ejecute al menos una tarea.
  while (executed_task == task_runner.ExecutedTasks() && !HasQuitted()) {
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

void App::Internal::HandleTask(Component component, Task& task) {
  std::visit(
      [&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        // clang-format off

    // Manejar Evento.
    if constexpr (std::is_same_v<T, Event>) {

      if (arg.is_cursor_position()) {
        cursor_x_ = arg.cursor_x();
        cursor_y_ = arg.cursor_y();
        cursor_position_request.OnReply();
        return;
      }

      if (arg.is_cursor_shape()) {
        cursor_reset_shape_ = arg.cursor_shape();
        return;
      }

      if (arg.IsTerminalCapabilities()) {
        terminal_capabilities_ = arg.TerminalCapabilities();
        return;
      }

      if (arg.IsTerminalNameVersion()) {
        terminal_name_ = arg.TerminalName();
        terminal_version_ = arg.TerminalVersion();
        return;
      }

      if (arg.IsTerminalEmulator()) {
        terminal_emulator_name_ = arg.TerminalEmulatorName();
        terminal_emulator_version_ = arg.TerminalEmulatorVersion();
        return;
      }

      if (arg.is_mouse()) {
        arg.mouse().x -= cursor_x_;
        arg.mouse().y -= cursor_y_;
      }

      arg.screen_ = public_;

      bool handled = component->OnEvent(arg);
      handled = HandleSelection(handled, arg);

      if (arg == Event::CtrlC && (!handled || force_handle_ctrl_c_)) {
        RecordSignal(SIGINT);
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

bool App::Internal::HandleSelection(bool handled, Event event) {
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
    selection_pending_ = public_->CaptureMouse();
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

void App::Internal::Draw(Component component) {
  if (frame_valid_) {
    return;
  }
  auto document = component->Render();
  int dimx = 0;
  int dimy = 0;
  auto terminal = Terminal::Size();
  document->ComputeRequirement();
  switch (dimension_) {
    case AppDimension::Fixed:
      dimx = public_->dimx_;
      dimy = public_->dimy_;
      break;
    case AppDimension::TerminalOutput:
      dimx = terminal.dimx;
      dimy = util::clamp(document->requirement().min_y, 0, terminal.dimy);
      break;
    case AppDimension::Fullscreen:
      dimx = terminal.dimx;
      dimy = terminal.dimy;
      break;
    case AppDimension::FitComponent:
      dimx = util::clamp(document->requirement().min_x, 0, terminal.dimx);
      dimy = util::clamp(document->requirement().min_y, 0, terminal.dimy);
      break;
  }

  // Ocultar el cursor para evitar parpadeos durante el reinicio.
  TerminalSend("\033[?25l");

  const bool resized =
      frame_count_ == 0 || (dimx != public_->dimx_) || (dimy != public_->dimy_);
  TerminalSend(ResetCursorPosition());

  if (frame_count_ != 0) {
    // Restablecer la posición del cursor a la esquina inferior izquierda para comenzar a dibujar el
    // nuevo fotograma.
    public_->ResetPosition(output_buffer, resized);

    // Si el ancho del terminal disminuye, el emulador de terminal comenzará a ajustar
    // las líneas y ensuciará la pantalla. Debemos limpiarla completamente.
    if ((dimx < public_->dimx_) && !use_alternative_screen_) {
      TerminalSend("\033[J");  // limpiar la salida del terminal
      TerminalSend("\033[H");  // mover el cursor a la posición inicial
    }
  }

  // Redimensionar la pantalla si es necesario.
  if (resized) {
    public_->dimx_ = dimx;
    public_->dimy_ = dimy;
    public_->cells_ = std::vector<Cell>(static_cast<size_t>(dimx) *
                                        static_cast<size_t>(dimy));
    Cursor cursor = public_->cursor_;
    cursor.x = dimx - 1;
    cursor.y = dimy - 1;
    public_->SetCursor(cursor);
  }

  // Solicitar periódicamente al emulador de terminal la posición del marco en relación con
  // la pantalla. Esto es útil para convertir la posición del ratón informada en
  // coordenadas de la pantalla a coordenadas del marco.
  if (!use_alternative_screen_ && is_stdout_a_tty_) {
    RequestCursorPosition(previous_frame_resized_);
  }
  previous_frame_resized_ = resized;

  selection_ = selection_data_.empty
                   ? std::make_unique<Selection>()
                   : std::make_unique<Selection>(
                         selection_data_.start_x, selection_data_.start_y,  //
                         selection_data_.end_x, selection_data_.end_y);
  Render(*public_, document.get(), *selection_);

  // Establecer la posición del cursor para el usuario que utiliza herramientas para insertar caracteres CJK.
  {
    const int dx = public_->dimx_ - 1 - public_->cursor_.x +
                   int(public_->dimx_ != terminal.dimx);
    const int dy = public_->dimy_ - 1 - public_->cursor_.y;

    set_cursor_position_.clear();
    reset_cursor_position_.clear();

    if (dy != 0) {
      set_cursor_position_ += "\x1B[" + std::to_string(dy) + "A";
      reset_cursor_position_ += "\x1B[" + std::to_string(dy) + "B";
    }

    if (dx != 0) {
      set_cursor_position_ += "\x1B[" + std::to_string(dx) + "D";
      reset_cursor_position_ += "\x1B[" + std::to_string(dx) + "C";
    }

    if (public_->cursor_.shape != Screen::Cursor::Hidden) {
      set_cursor_position_ += "\033[?25h";
      set_cursor_position_ +=
          "\033[" + std::to_string(int(public_->cursor_.shape)) + " q";
    }
  }

  public_->ToString(output_buffer);
  TerminalSend(set_cursor_position_);
  TerminalFlush();

  public_->Clear();
  frame_valid_ = true;
  frame_count_++;
}

std::string App::Internal::ResetCursorPosition() {
  std::string result = std::move(reset_cursor_position_);
  reset_cursor_position_ = "";
  return result;
}

void App::Internal::RequestCursorPosition(bool force) {
  cursor_position_request.Request(force);
}

void App::Internal::TerminalSend(std::string_view s) {
  output_buffer += s;
}

void App::Internal::TerminalFlush() {
  // Emscripten no implementa flush. Interpretamos cero como flush.
  output_buffer += '\0';
  std::cout << output_buffer << std::flush;
  output_buffer.clear();
}

void App::Internal::InstallPipedInputHandling() {
  is_stdin_a_tty_ = false;
  is_stdout_a_tty_ = false;
#if defined(__EMSCRIPTEN__)
  is_stdin_a_tty_ = true;
  is_stdout_a_tty_ = true;
#elif defined(_WIN32)
  is_stdin_a_tty_ = _isatty(_fileno(stdin));
  is_stdout_a_tty_ = _isatty(_fileno(stdout));
#else
  tty_fd_ = STDIN_FILENO;
  is_stdout_a_tty_ = isatty(STDOUT_FILENO);
  // Manejar la redirección de entrada canalizada si la aplicación la habilita explícitamente.
  // Esto permite que las aplicaciones lean datos de stdin mientras siguen recibiendo
  // entrada de teclado del terminal para uso interactivo.
  if (!handle_piped_input_) {
    is_stdin_a_tty_ = isatty(STDIN_FILENO);
  } else if (isatty(STDIN_FILENO)) {
    is_stdin_a_tty_ = true;
  } else {
    // Abrir /dev/tty para la entrada del teclado.
    tty_fd_ = open("/dev/tty", O_RDONLY);  // NOLINT
    if (tty_fd_ < 0) {
      // Falló al abrir /dev/tty (contenedores, sistemas sin cabeza, etc.)
      tty_fd_ = STDIN_FILENO;  // Volver a stdin.
      is_stdin_a_tty_ = isatty(STDIN_FILENO);
    } else {
      is_stdin_a_tty_ = true;
      // Cerrar el descriptor de archivo /dev/tty al salir.
      on_exit_functions.emplace([this] {
        close(tty_fd_);
        tty_fd_ = -1;
      });
    }
  }
#endif
}

void App::Internal::InstallTerminalInfo() {
  // Solicitar al terminal que informe la forma actual del cursor. Lo restauraremos
  // al salir.
  if (is_stdout_a_tty_) {
    TerminalSend(DECRQSS_DECSCUSR);
    TerminalSend("\033[>q");  // XTVERSION
    TerminalSend("\033[>c");  // DA2
    TerminalSend("\033[c");   // DA1
    TerminalFlush();
  }

  // Esperar la respuesta de forma del cursor usando la cabecera de configuración.
  if (is_stdin_a_tty_ && is_stdout_a_tty_) {
    // Un receptor con alcance limitado a la configuración: mantener uno vivo después de la configuración fijaría
    // cada evento subsecuente en el búfer, haciéndolo crecer durante toda la vida
    // de la aplicación.
    auto setup_receiver = event_buffer.CreateReceiver();
    auto start = std::chrono::steady_clock::now();
    bool terminal_capabilities_received = false;
    // Esperar la respuesta de forma del cursor usando la cabecera de configuración.
    while (true) {
      FetchTerminalEvents();
      while (setup_receiver->Has()) {
        const auto event = setup_receiver->Pop();
        if (event.is_cursor_shape()) {
          cursor_reset_shape_ = event.cursor_shape();
        }

        if (event.IsTerminalCapabilities()) {
          terminal_capabilities_ = event.TerminalCapabilities();
          terminal_capabilities_received = true;
        }

        if (event.IsTerminalNameVersion()) {
          terminal_name_ = event.TerminalName();
          terminal_version_ = event.TerminalVersion();
        }

        if (event.IsTerminalEmulator()) {
          terminal_emulator_name_ = event.TerminalEmulatorName();
          terminal_emulator_version_ = event.TerminalEmulatorVersion();
        }
      }

      // Se espera que las respuestas se reciban en orden, así que podemos salir cuando
      // se reciba la última (XTVERSION). También establecemos un tiempo de espera para evitar
      // esperar para siempre en caso de que el terminal no admita estas consultas.
      if (terminal_capabilities_received) {
        break;
      }

      if (std::chrono::steady_clock::now() - start >
          std::chrono::milliseconds(500)) {
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }

  // Establecer peculiaridades y soporte de color según la identificación del terminal.
  Terminal::Quirks quirks = Terminal::GetQuirks();

  auto color_support = Terminal::ComputeColorSupport(
      util::GetEnv("TERM"), util::GetEnv("COLORTERM"),
      util::GetEnv("TERM_PROGRAM"), terminal_name_, terminal_emulator_name_,
      terminal_capabilities_);

  quirks.SetColorSupport(color_support);

  const bool is_modern_emulator = (terminal_emulator_name_ != "unknown");
  const bool is_vt220_plus =
      (terminal_name_ != "vt100" && terminal_name_ != "unknown");
  bool reports_utf8 = false;
  for (const int x : terminal_capabilities_) {
    if (x == 52) {
      reports_utf8 = true;
      break;
    }
  }

  // Heurística: Si el emulador de terminal es moderno, o reporta soporte de
  // UTF-8 o color, podemos asumir que admite caracteres de bloque y ocultación
  // de cursor, que son esenciales para una buena experiencia. Esto es una heurística, pero
  // nos permite evitar algunos emuladores de terminal antiguos que no
  // admiten estas funciones, mientras seguimos proporcionando una buena experiencia en
  // emuladores de terminal modernos que sí las admiten.
  bool modern = is_modern_emulator || is_vt220_plus || reports_utf8;
  if (modern) {
    quirks.SetBlockCharacters(true);
    quirks.SetCursorHiding(true);
    quirks.SetComponentAscii(false);
  }

  Terminal::SetQuirks(quirks);

  on_exit_functions.emplace([this] {
    TerminalSend("\033[?25h");  // Habilitar cursor.
    if (is_stdout_a_tty_) {
      TerminalSend("\033[" + std::to_string(cursor_reset_shape_) + " q");
    }
  });
}

void App::Internal::Signal(int signal) {
  if (signal == SIGABRT) {
    public_->Exit();
    return;
  }

// Windows no soporta SIGTSTP / SIGWINCH
#if !defined(_WIN32)
  if (signal == SIGTSTP) {
    public_->Post([&] {
      TerminalSend(ResetCursorPosition());
      public_->ResetPosition(output_buffer, /*clear*/ true);
      Uninstall();
      public_->dimx_ = 0;
      public_->dimy_ = 0;
      (void)std::raise(SIGTSTP);
      Install();
    });
    return;
  }

  if (signal == SIGWINCH) {
    public_->Post(Event::Special({0}));
    return;
  }
#endif
}

size_t App::Internal::FetchTerminalEvents() {
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
    const auto timeout = std::chrono::steady_clock::now() - last_char_time;
    const size_t timeout_microseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(timeout).count();
    terminal_input_parser.Timeout(timeout_microseconds);
    return 0;
  }
  last_char_time = std::chrono::steady_clock::now();

  // Convertir los eventos de entrada a eventos FTXUI.
  // Para cada evento, llamamos al analizador de entrada del terminal para convertirlo a
  // Evento.
  std::wstring wstring;
  for (const auto& r : records) {
    switch (r.EventType) {
      case KEY_EVENT: {
        auto key_event = r.Event.KeyEvent;
        // ignorar eventos de tecla UP
        if (key_event.bKeyDown == FALSE) {
          continue;
        }
        const wchar_t wc = key_event.uChar.UnicodeChar;
        wstring += wc;
        if (wc >= 0xd800 && wc <= 0xdbff) {
          // Esperar a que llegue el Low Surrogate en el siguiente registro.
          continue;
        }
        for (auto it : to_string(wstring)) {
          terminal_input_parser.Add(it);
        }
        wstring.clear();
      } break;
      case WINDOW_BUFFER_SIZE_EVENT:
        public_->Post(Event::Special({0}));
        break;
      case MENU_EVENT:
      case FOCUS_EVENT:
      case MOUSE_EVENT:
        // TODO(mauve): Implementar más tarde.
        break;
    }
  }
  return records.size();
#elif defined(__EMSCRIPTEN__)
  // Leer caracteres del terminal.
  // Lo configuramos para que no sea bloqueante.
  std::array<char, 4096> out{};
  const ssize_t l = read(STDIN_FILENO, out.data(), out.size());
  if (l <= 0) {
    const auto timeout = std::chrono::steady_clock::now() - last_char_time;
    const size_t timeout_microseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(timeout).count();
    terminal_input_parser.Timeout(timeout_microseconds);
    return 0;
  }
  last_char_time = std::chrono::steady_clock::now();

  // Convertir los caracteres a eventos.
  for (ssize_t i = 0; i < l; ++i) {
    terminal_input_parser.Add(out.at(static_cast<size_t>(i)));
  }
  return (size_t)l;
#else  // POSIX (Linux y Mac)
  struct pollfd pfd = {tty_fd_, POLLIN, 0};
  const int poll_result = poll(&pfd, 1, 0);
  if (poll_result <= 0) {
    const auto timeout = std::chrono::steady_clock::now() - last_char_time;
    const size_t timeout_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count();
    terminal_input_parser.Timeout(static_cast<int>(timeout_ms));
    return 0;
  }
  last_char_time = std::chrono::steady_clock::now();

  // Drenar la entrada disponible, para que las ráfagas (p. ej. desplazamiento rápido
  // de la rueda del ratón) no se acumulen entre fotogramas. El total está limitado para mantener
  // la respuesta del fotograma bajo una inundación continua de entrada. Ver #1348.
  constexpr size_t kMaxBytesPerFetch = 64 * 1024;
  std::array<char, 4096> out{};
  size_t total = 0;
  while (total < kMaxBytesPerFetch) {
    const ssize_t l = read(tty_fd_, out.data(), out.size());
    if (l <= 0) {
      break;
    }

    // Convertir los caracteres a eventos.
    for (ssize_t i = 0; i < l; ++i) {
      terminal_input_parser.Add(out.at(static_cast<size_t>(i)));
    }
    total += static_cast<size_t>(l);

    pfd.revents = 0;
    if (poll(&pfd, 1, 0) <= 0) {
      break;
    }
  }
  return total;
#endif
}

void App::Internal::PostAnimationTask() {
  public_->Post(AnimationTask());

  // Repetir la tarea de animación cada 15ms. Esto corresponde a una velocidad de fotogramas
  // de alrededor de 66fps.
  task_runner.PostDelayedTask([this] { PostAnimationTask(); },
                              std::chrono::milliseconds(15));
}

App::App(std::unique_ptr<Internal> internal, int dimx, int dimy)
    : Screen(dimx, dimy), internal_(std::move(internal)) {
  internal_->public_ = this;
}

App::App(App&& other) noexcept : Screen(std::move(other)) {
  internal_ = std::move(other.internal_);
  if (internal_) {
    internal_->public_ = this;
  }
}

App& App::operator=(App&& other) noexcept {
  Screen::operator=(std::move(other));
  internal_ = std::move(other.internal_);
  if (internal_) {
    internal_->public_ = this;
  }
  return *this;
}

App::~App() = default;

// static
App App::FixedSize(int dimx, int dimy) {
  auto internal =
      std::make_unique<Internal>(nullptr, AppDimension::Fixed, false);
  return App(std::move(internal), dimx, dimy);
}

// static
App App::Fullscreen() {
  return FullscreenAlternateScreen();
}

// static
App App::FullscreenPrimaryScreen() {
  auto terminal = Terminal::Size();
  auto internal =
      std::make_unique<Internal>(nullptr, AppDimension::Fullscreen, false);
  return App(std::move(internal), terminal.dimx, terminal.dimy);
}

// static
App App::FullscreenAlternateScreen() {
  auto terminal = Terminal::Size();
  auto internal =
      std::make_unique<Internal>(nullptr, AppDimension::Fullscreen, true);
  return App(std::move(internal), terminal.dimx, terminal.dimy);
}

// static
App App::FitComponent() {
  auto terminal = Terminal::Size();
  auto internal =
      std::make_unique<Internal>(nullptr, AppDimension::FitComponent, false);
  return App(std::move(internal), terminal.dimx, terminal.dimy);
}

// static
App App::TerminalOutput() {
  auto terminal = Terminal::Size();
  auto internal =
      std::make_unique<Internal>(nullptr, AppDimension::TerminalOutput, false);
  return App(std::move(internal), terminal.dimx, terminal.dimy);
}

void App::TrackMouse(bool enable) {
  internal_->track_mouse_ = enable;
}

void App::HandlePipedInput(bool enable) {
  internal_->handle_piped_input_ = enable;
}

// static
App* App::Active() {
  return g_active_screen;
}

void App::Loop(Component component) {
  class Loop loop(this, std::move(component));
  loop.Run();
}

void App::Exit() {
  Post([this] { internal_->ExitNow(); });
}

Closure App::ExitLoopClosure() {
  return [this] { Exit(); };
}

void App::Post(Task task) {
  internal_->task_runner.PostTask([this, task = std::move(task)]() mutable {
    if (internal_->component_) {
      internal_->HandleTask(internal_->component_, task);
      return;
    }

    // Si no hay ningún componente, aún podemos ejecutar cierres (closures).
    if (std::holds_alternative<Closure>(task)) {
      std::get<Closure>(task)();
    }
  });
}

void App::PostEvent(Event event) {
  // PostEvent está documentado como seguro para hilos: pasa por la cola de tareas
  // protegida por mutex. event_buffer solo es seguro de usar desde el hilo principal.
  Post(Task(std::move(event)));
}

// static
void App::PostEventOrExecute(Closure closure) {
  if (!closure) {
    return;
  }
  if (auto* app = App::Active()) {
    app->Post(std::move(closure));
  } else {
    closure();
  }
}

void App::RequestAnimationFrame() {
  if (internal_->animation_requested_) {
    return;
  }
  internal_->animation_requested_ = true;
  auto now = animation::Clock::now();
  const auto time_histeresis = std::chrono::milliseconds(33);
  if (now - internal_->previous_animation_time_ >= time_histeresis) {
    internal_->previous_animation_time_ = now;
  }
}

CapturedMouse App::CaptureMouse() {
  if (internal_->mouse_captured) {
    return nullptr;
  }
  internal_->mouse_captured = true;
  return std::make_unique<CapturedMouseImpl>(
      [this] { internal_->mouse_captured = false; });
}

Closure App::WithRestoredIO(Closure fn) {
  return [this, fn] {
    internal_->Uninstall();
    fn();
    internal_->Install();
  };
}

void App::ForceHandleCtrlC(bool force) {
  internal_->force_handle_ctrl_c_ = force;
}

void App::ForceHandleCtrlZ(bool force) {
  internal_->force_handle_ctrl_z_ = force;
}

std::string App::GetSelection() {
  if (!internal_->selection_) {
    return "";
  }
  return internal_->selection_->GetParts();
}

void App::SelectionChange(std::function<void()> callback) {
  internal_->selection_on_change_ = std::move(callback);
}

const std::string& App::TerminalName() const {
  return internal_->terminal_name_;
}

int App::TerminalVersion() const {
  return internal_->terminal_version_;
}

const std::string& App::TerminalEmulatorName() const {
  return internal_->terminal_emulator_name_;
}

const std::string& App::TerminalEmulatorVersion() const {
  return internal_->terminal_emulator_version_;
}

const std::vector<int>& App::TerminalCapabilities() const {
  return internal_->terminal_capabilities_;
}

std::vector<std::string> App::TerminalCapabilityNames() const {
  return Event::TerminalCapabilities("", internal_->terminal_capabilities_)
      .TerminalCapabilityNames();
}

// El bucle llama a esto:

void App::ExitNow() {
  internal_->ExitNow();
}
void App::Install() {
  internal_->Install();
}
void App::Uninstall() {
  internal_->Uninstall();
}
void App::PreMain() {
  internal_->PreMain();
}
void App::PostMain() {
  internal_->PostMain();
}
bool App::HasQuitted() {
  return internal_->HasQuitted();
}
void App::RunOnce(const Component& component) {
  internal_->RunOnce(component);
}
void App::RunOnceBlocking(Component component) {
  internal_->RunOnceBlocking(component);
}
void App::HandleTask(Component component, Task& task) {
  internal_->HandleTask(component, task);
}
bool App::HandleSelection(bool handled, Event event) {
  return internal_->HandleSelection(handled, event);
}
void App::Draw(Component component) {
  internal_->Draw(component);
}
std::string App::ResetCursorPosition() {
  return internal_->ResetCursorPosition();
}
void App::RequestCursorPosition(bool force) {
  internal_->RequestCursorPosition(force);
}
void App::TerminalSend(std::string_view s) {
  internal_->TerminalSend(s);
}
void App::TerminalFlush() {
  internal_->TerminalFlush();
}
void App::InstallPipedInputHandling() {
  internal_->InstallPipedInputHandling();
}
void App::InstallTerminalInfo() {
  internal_->InstallTerminalInfo();
}
void App::Signal(int signal) {
  internal_->Signal(signal);
}
size_t App::FetchTerminalEvents() {
  return internal_->FetchTerminalEvents();
}
void App::PostAnimationTask() {
  internal_->PostAnimationTask();
}

Loop::Loop(App* screen, Component component)
    : screen_(screen), component_(std::move(component)) {
  screen_->PreMain();
}

Loop::~Loop() {
  screen_->PostMain();
}

bool Loop::HasQuitted() {
  return screen_->HasQuitted();
}

void Loop::RunOnce() {
  screen_->RunOnce(component_);
}

void Loop::RunOnceBlocking() {
  screen_->RunOnceBlocking(component_);
}

void Loop::Run() {
  while (!HasQuitted()) {
    RunOnceBlocking();
  }
}

}  // namespace ftxui
