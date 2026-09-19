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

  // État de la gestion de l'entrée redirigée (POSIX uniquement)
  bool handle_piped_input_ = true;
  bool is_stdin_a_tty_ = false;
  bool is_stdout_a_tty_ = false;
  // Descripteur de fichier pour /dev/tty, utilisé pour la gestion de l'entrée
  // redirigée.
  int tty_fd_ = -1;

  std::string terminal_name_ = "unknown";
  int terminal_version_ = 0;

  std::string terminal_emulator_name_ = "unknown";
  std::string terminal_emulator_version_ = "unknown";

  std::vector<int> terminal_capabilities_;

  // API de sélection :
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

  // Préexistant dans Internal :
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

      // N'autorise qu'une seule requête en attente à la fois. Ceci évite de
      // saturer le terminal de requêtes.
      if (HasPending()) {
        return;
      }

      const auto now = std::chrono::steady_clock::now();
      if (now - last_request_time_ < std::chrono::milliseconds(500)) {
        // Trop tôt depuis la dernière requête. On l'ignore : la requête doit
        // être envoyée de manière synchrone depuis Draw(), juste après que le
        // curseur a été déplacé vers l'origine de la frame, afin que la
        // réponse du terminal reflète cette position. Draw() appelle à
        // nouveau Request() à la prochaine frame, donc la requête n'est pas
        // perdue, seulement retardée.
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

// CSI : Control Sequence Introducer
const std::string CSI = "\x1b[";  // NOLINT
                                  //
// DCS : Device Control String
const std::string DCS = "\x1bP";  // NOLINT

// ST : String Terminator
const std::string ST = "\x1b\\";  // NOLINT

// DECRQSS : Request Status String
// DECSCUSR : Set Cursor Style
const std::string DECRQSS_DECSCUSR = DCS + "$q q" + ST;  // NOLINT

// DEC : Digital Equipment Corporation
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

// Device Status Report (DSR) {
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

// DEC Private Mode Set (DECSET)
std::string Set(const std::vector<DECMode>& parameters) {
  return CSI + "?" + Serialize(parameters) + "h";
}

// DEC Private Mode Reset (DECRST)
std::string Reset(const std::vector<DECMode>& parameters) {
  return CSI + "?" + Serialize(parameters) + "l";
}

// Device Status Report (DSR)
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

// Indique si le terminal est actuellement configuré en mode brut (raw).
// Utilisé pour éviter une double restauration lors des sorties d'urgence et
// normales.
std::atomic<bool> g_terminal_is_raw{false};

// Stocke le dernier signal différé reçu (par ex. SIGINT, SIGTERM) pour le
// relever pendant la désinstallation/sortie.
std::atomic<int> g_last_signal{0};  // NOLINT

#if defined(_WIN32)
using SignalHandler = void (*)(int);
// Stocke les gestionnaires de signaux d'origine avant que FTXUI n'installe
// les siens.
std::map<int, SignalHandler> g_old_signal_handlers;

// Stocke les modes de console d'origine pour les restaurer à la sortie.
DWORD g_original_stdout_mode = 0;
DWORD g_original_stdin_mode = 0;
bool g_has_original_console_mode = false;
#else
// Stocke les structures sigaction d'origine avant que FTXUI n'installe les
// siennes.
std::map<int, struct sigaction> g_old_sigactions;

// Stocke les réglages termios du terminal d'origine pour les restaurer à la
// sortie.
struct termios g_original_termios;
bool g_has_original_termios = false;
int g_tty_fd = -1;
#endif

// Restaure le gestionnaire de signal d'origine pour le signal donné et le
// relève.
// Fonction sûre pour un signal asynchrone (async-signal-safe).
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

// Restauration d'urgence de l'état du terminal.
// Fonction sûre pour un signal asynchrone (async-signal-safe).
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
        "\x1b[?25h"    // Affiche le curseur.
        "\x1b[?1049l"  // Bascule vers le tampon d'écran normal.
        "\x1b[?1000l"  // Désactive le suivi normal de la souris.
        "\x1b[?1002l"  // Désactive le suivi des événements de bouton souris.
        "\x1b[?1003l"  // Désactive le suivi de tous les mouvements souris.
        "\x1b[?1006l"  // Désactive le suivi souris SGR.
        "\x1b[?1015l"  // Désactive le suivi souris Urxvt.
        "\x1b[?7h";    // Active le retour à la ligne automatique.
    std::ignore = write(STDOUT_FILENO, restore_seq, sizeof(restore_seq) - 1);
    tcsetattr(g_tty_fd, TCSANOW, &g_original_termios);
  }
#endif
}

// Fonction sûre pour un signal asynchrone (async signal safe)
void RecordSignal(int signal) {
  switch (signal) {
    // Terminaison anormale (par ex. abort() ou échec d'assertion).
    case SIGABRT:
    // Opération arithmétique erronée (par ex. division par zéro).
    case SIGFPE:
    // Instruction illégale.
    case SIGILL:
    // Référence mémoire invalide (erreur de segmentation).
    case SIGSEGV:
#if !defined(_WIN32)
    // Erreur de bus (par ex. mauvais alignement d'accès mémoire).
    case SIGBUS:
    // Appel système invalide.
    case SIGSYS:
#endif
    {
      RestoreTerminalEmergency();
      RestoreSignalHandlerAndRaise(signal);
      break;
    }

    // Interruption terminal (par ex. Ctrl-C).
    case SIGINT:
    // Demande de terminaison.
    case SIGTERM:
#if !defined(_WIN32)
    // Fin du terminal (par ex. Ctrl-\, produit un core dump).
    case SIGQUIT:
    // Raccroché détecté sur le terminal de contrôle ou mort du processus de
    // contrôle.
    case SIGHUP:
#endif
      g_last_signal.store(signal);
      g_signal_exit_count++;
      break;

#if !defined(_WIN32)
    // Signal d'arrêt du terminal (par ex. Ctrl-Z).
    case SIGTSTP:  // NOLINT
      g_signal_stop_count++;
      break;

    // Changement de taille de la fenêtre du terminal.
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

  // Vide le tampon de stdout pour s'assurer que tout ce que l'utilisateur a
  // affiché avant est pleinement appliqué avant que l'on commence à modifier
  // la configuration du terminal. C'est important, car on utilise deux
  // canaux différents (stdout vs termios/WinAPI) pour communiquer avec
  // l'émulateur de terminal ci-dessous. Voir
  // https://github.com/ArthurSonzogni/FTXUI/issues/846
  TerminalFlush();

  InstallPipedInputHandling();

  // Après avoir désinstallé la nouvelle configuration, la vider vers le
  // terminal pour s'assurer qu'elle est pleinement appliquée :
  on_exit_functions.emplace([this] { TerminalFlush(); });

  // Installe des gestionnaires de signaux pour restaurer l'état du terminal
  // à la sortie. Les gestionnaires de signaux par défaut sont restaurés à la
  // sortie.
  for (const int signal : {SIGTERM, SIGSEGV, SIGINT, SIGILL, SIGABRT, SIGFPE}) {
    InstallSignalHandler(signal);
  }

// Sauvegarde l'ancienne configuration du terminal et la restaure à la sortie.
#if defined(_WIN32)
  // Active le traitement VT sur stdout et stdin
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
#else  // POSIX (Linux & Mac)
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

  // Activation du mode d'entrée brut (raw) du terminal
  terminal.c_iflag &= ~IGNBRK;  // Désactive l'ignorance de la condition break
  terminal.c_iflag &= ~BRKINT;  // Désactive le fait qu'un break vide l'entrée
                                // et la sortie
  terminal.c_iflag &= ~PARMRK;  // Désactive le marquage des erreurs de parité.
  terminal.c_iflag &= ~ISTRIP;  // Désactive la suppression du 8e bit des
                                // caractères.
  terminal.c_iflag &= ~INLCR;   // Désactive la conversion NL en CR.
  terminal.c_iflag &= ~IGNCR;   // Désactive l'ignorance de CR.
  terminal.c_iflag &= ~ICRNL;   // Désactive la conversion CR en NL.
  terminal.c_iflag &= ~IXON;    // Désactive le contrôle de flux XON/XOFF en
                                // sortie

  terminal.c_lflag &= ~ECHO;    // Désactive l'écho des caractères saisis.
  terminal.c_lflag &= ~ECHONL;  // Désactive l'écho des retours à la ligne.
  terminal.c_lflag &= ~ICANON;  // Désactive le mode canonique.
  terminal.c_lflag &= ~ISIG;    // Désactive l'envoi de signal lors de la
                                // frappe de :
                                // -     => DSUSP
                                // - C-Z => SUSP
                                // - C-C => INTR
                                // - C-d => QUIT
  terminal.c_lflag &= ~IEXTEN;  // Désactive le traitement étendu des entrées
  terminal.c_cflag |= CS8;      // 8 bits par octet

  terminal.c_cc[VMIN] = 0;   // Nombre minimum de caractères pour une lecture
                             // non canonique.
  terminal.c_cc[VTIME] = 0;  // Délai en dixièmes de seconde pour une lecture
                             // non canonique.

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

  // Après avoir installé la nouvelle configuration, la vider vers le
  // terminal pour s'assurer qu'elle est pleinement appliquée :
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

  // Pendant l'arrêt, attend toutes les réponses.
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
  // Suspend l'écran précédemment actif :
  if (g_active_screen) {
    std::swap(suspended_screen_, g_active_screen);
    // Replace la position du curseur en haut de l'écran et efface l'écran.
    suspended_screen_->internal_->TerminalSend(
        suspended_screen_->internal_->ResetCursorPosition());
    suspended_screen_->ResetPosition(
        suspended_screen_->internal_->output_buffer,
        /*clear=*/true);
    suspended_screen_->dimx_ = 0;
    suspended_screen_->dimy_ = 0;

    // Réinitialise les dimensions pour forcer le redessin de l'écran la
    // prochaine fois :
    suspended_screen_->internal_->Uninstall();
  }

  // Cet écran est maintenant actif :
  g_active_screen = public_;
  g_active_screen->internal_->Install();

  previous_animation_time_ = animation::Clock::now();
}

void App::Internal::PostMain() {
  // Place la position du curseur à la fin du dessin.
  TerminalSend(ResetCursorPosition());

  g_active_screen = nullptr;

  // Restaure l'écran suspendu.
  if (suspended_screen_) {
    // Efface l'écran, et place le curseur au début du dessin.
    public_->ResetPosition(output_buffer, /*clear=*/true);
    public_->dimx_ = 0;
    public_->dimy_ = 0;
    Uninstall();
    std::swap(g_active_screen, suspended_screen_);
    g_active_screen->internal_->Install();
  } else {
    Uninstall();

    std::cout << "\r";
    // À la sortie finale, conserve le dessin actuel et replace le curseur
    // une ligne après celui-ci.
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

  // Exécute les tâches en attente dans la file.
  const size_t executed_task = task_runner.ExecutedTasks();
  task_runner.RunUntilIdle();
  // Si aucune tâche n'a été exécutée, on peut retourner tôt sans redessiner
  // l'écran.
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
  // Limite le FPS à 60 au maximum.
  const auto time_per_frame = std::chrono::microseconds(16666);  // 1s / 60fps

  auto time = std::chrono::steady_clock::now();
  const size_t executed_task = task_runner.ExecutedTasks();

  // Attend qu'au moins une tâche s'exécute.
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

    // Gère les événements.
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

    // Gère le callback
    if constexpr (std::is_same_v<T, Closure>) {
      arg();
      return;
    }

    // Gère l'animation
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

  // Cache le curseur pour éviter un scintillement pendant la réinitialisation.
  TerminalSend("\033[?25l");

  const bool resized =
      frame_count_ == 0 || (dimx != public_->dimx_) || (dimy != public_->dimy_);
  TerminalSend(ResetCursorPosition());

  if (frame_count_ != 0) {
    // Replace la position du curseur dans le coin inférieur gauche pour
    // commencer à dessiner la nouvelle frame.
    public_->ResetPosition(output_buffer, resized);

    // Si la largeur du terminal diminue, l'émulateur de terminal commencera à
    // faire un retour à la ligne automatique et rendra l'affichage sale. Il
    // faut alors le nettoyer entièrement.
    if ((dimx < public_->dimx_) && !use_alternative_screen_) {
      TerminalSend("\033[J");  // efface la sortie du terminal
      TerminalSend("\033[H");  // déplace le curseur à la position d'origine
    }
  }

  // Redimensionne l'écran si nécessaire.
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

  // Demande périodiquement à l'émulateur de terminal la position de la frame
  // relative à l'écran. Ceci est utile pour convertir la position de la
  // souris rapportée dans les coordonnées de l'écran vers les coordonnées de
  // la frame.
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

  // Définit la position du curseur pour les utilisateurs utilisant des outils
  // pour insérer des caractères CJK.
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
  // Emscripten n'implémente pas flush. On interprète zéro comme un flush.
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
  // Gère la redirection de l'entrée piped si explicitement activée par
  // l'application. Cela permet aux applications de lire les données depuis
  // stdin tout en continuant à recevoir les entrées clavier du terminal pour
  // un usage interactif.
  if (!handle_piped_input_) {
    is_stdin_a_tty_ = isatty(STDIN_FILENO);
  } else if (isatty(STDIN_FILENO)) {
    is_stdin_a_tty_ = true;
  } else {
    // Ouvre /dev/tty pour les entrées clavier.
    tty_fd_ = open("/dev/tty", O_RDONLY);  // NOLINT
    if (tty_fd_ < 0) {
      // Échec de l'ouverture de /dev/tty (conteneurs, systèmes sans tête,
      // etc.)
      tty_fd_ = STDIN_FILENO;  // Se rabat sur stdin.
      is_stdin_a_tty_ = isatty(STDIN_FILENO);
    } else {
      is_stdin_a_tty_ = true;
      // Ferme le descripteur de fichier /dev/tty à la sortie.
      on_exit_functions.emplace([this] {
        close(tty_fd_);
        tty_fd_ = -1;
      });
    }
  }
#endif
}

void App::Internal::InstallTerminalInfo() {
  // Demande au terminal de rapporter la forme actuelle du curseur. On la
  // restaurera à la sortie.
  if (is_stdout_a_tty_) {
    TerminalSend(DECRQSS_DECSCUSR);
    TerminalSend("\033[>q");  // XTVERSION
    TerminalSend("\033[>c");  // DA2
    TerminalSend("\033[c");   // DA1
    TerminalFlush();
  }

  // Attend la réponse de la forme du curseur en utilisant la tête de
  // configuration.
  if (is_stdin_a_tty_ && is_stdout_a_tty_) {
    // Un récepteur limité à la configuration : en garder un vivant après la
    // configuration épinglerait chaque événement suivant dans le tampon, le
    // faisant grossir pendant toute la durée de vie de l'application.
    auto setup_receiver = event_buffer.CreateReceiver();
    auto start = std::chrono::steady_clock::now();
    bool terminal_capabilities_received = false;
    // Attend la réponse de la forme du curseur en utilisant la tête de
    // configuration.
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

      // Les réponses sont censées être reçues dans l'ordre, on peut donc
      // sortir de la boucle quand la dernière (XTVERSION) est reçue. On fixe
      // aussi un délai limite pour éviter d'attendre indéfiniment si le
      // terminal ne supporte pas ces requêtes.
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

  // Définit les quirks et le support des couleurs en fonction de
  // l'identification du terminal.
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

  // Heuristique : Si l'émulateur de terminal est moderne, ou s'il rapporte
  // supporter l'UTF-8 ou la couleur, on peut supposer qu'il supporte les
  // caractères de blocs et le masquage du curseur, qui sont essentiels pour
  // une bonne expérience. C'est une heuristique, mais elle nous permet de
  // contourner certains émulateurs de terminal plus anciens qui ne
  // supportent pas ces fonctionnalités, tout en continuant à offrir une bonne
  // expérience sur les émulateurs de terminal modernes qui les supportent.
  bool modern = is_modern_emulator || is_vt220_plus || reports_utf8;
  if (modern) {
    quirks.SetBlockCharacters(true);
    quirks.SetCursorHiding(true);
    quirks.SetComponentAscii(false);
  }

  Terminal::SetQuirks(quirks);

  on_exit_functions.emplace([this] {
    TerminalSend("\033[?25h");  // Active le curseur.
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

// Windows ne supporte pas SIGTSTP / SIGWINCH
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
    // Vérifie s'il y a une entrée dans la console.
    auto console = GetStdHandle(STD_INPUT_HANDLE);
    DWORD number_of_events = 0;
    if (!GetNumberOfConsoleInputEvents(console, &number_of_events)) {
      return std::vector<INPUT_RECORD>();
    }
    if (number_of_events <= 0) {
      // Aucune entrée, on retourne.
      return std::vector<INPUT_RECORD>();
    }
    // Lit les événements d'entrée.
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

  // Convertit les événements d'entrée en événements FTXUI.
  // Pour chaque événement, on appelle l'analyseur d'entrée du terminal pour
  // le convertir en Event.
  std::wstring wstring;
  for (const auto& r : records) {
    switch (r.EventType) {
      case KEY_EVENT: {
        auto key_event = r.Event.KeyEvent;
        // ignore les événements de relâchement de touche
        if (key_event.bKeyDown == FALSE) {
          continue;
        }
        const wchar_t wc = key_event.uChar.UnicodeChar;
        wstring += wc;
        if (wc >= 0xd800 && wc <= 0xdbff) {
          // Attend l'arrivée du Low Surrogate dans le prochain enregistrement.
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
        // TODO(mauve): Implémenter plus tard.
        break;
    }
  }
  return records.size();
#elif defined(__EMSCRIPTEN__)
  // Lit les caractères depuis le terminal.
  // On l'a configuré pour être non bloquant.
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

  // Convertit les caractères en événements.
  for (ssize_t i = 0; i < l; ++i) {
    terminal_input_parser.Add(out.at(static_cast<size_t>(i)));
  }
  return (size_t)l;
#else  // POSIX (Linux & Mac)
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

  // Draine l'entrée disponible, afin que les rafales (par ex. un défilement
  // rapide de la molette souris) ne s'accumulent pas d'une frame à l'autre.
  // Le total est borné pour maintenir la frame réactive en cas d'afflux
  // continu d'entrées. Voir #1348.
  constexpr size_t kMaxBytesPerFetch = 64 * 1024;
  std::array<char, 4096> out{};
  size_t total = 0;
  while (total < kMaxBytesPerFetch) {
    const ssize_t l = read(tty_fd_, out.data(), out.size());
    if (l <= 0) {
      break;
    }

    // Convertit les caractères en événements.
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

  // Répète la tâche d'animation toutes les 15ms. Cela correspond à un taux
  // de rafraîchissement d'environ 66fps.
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

    // S'il n'y a pas de composant, on peut quand même exécuter les closures.
    if (std::holds_alternative<Closure>(task)) {
      std::get<Closure>(task)();
    }
  });
}

void App::PostEvent(Event event) {
  // PostEvent est documenté comme étant thread-safe : on passe par la file de
  // tâches protégée par un mutex. L'event_buffer n'est sûr à utiliser que
  // depuis le thread principal.
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

// Loop appelle ceci :

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
