// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <gtest/gtest.h>  // for Test, TestInfo (ptr only), TEST, EXPECT_EQ, Message, TestPartResult
#include <csignal>  // for raise, SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM
#include <ftxui/component/event.hpp>  // for Event, Event::Custom
#include <tuple>                      // for _Swallow_assign, ignore

#include "ftxui/component/component.hpp"  // for Renderer
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"  // for text, Element

#if defined(__unix__)
#include <fcntl.h>
#include <unistd.h>
#include <array>
#include <cstdio>
#include <ftxui/component/loop.hpp>
#include <string>
#endif

namespace ftxui {

namespace {
#if defined(__unix__)

// Captura la salida estándar (stdout) en una cadena.
class StdCapture {
 public:
  explicit StdCapture(std::string* captured) : captured_(captured) {
    if (pipe(pipefd_) != 0) {
      return;
    }
    old_stdout_ = dup(fileno(stdout));
    fflush(stdout);
    dup2(pipefd_[1], fileno(stdout));
    close(pipefd_[1]);  // Cierra el extremo de escritura en el padre
  }

  ~StdCapture() {
    fflush(stdout);
    dup2(old_stdout_, fileno(stdout));
    close(old_stdout_);

    char buffer[1024];
    ssize_t count;
    while ((count = read(pipefd_[0], buffer, sizeof(buffer))) > 0) {
      captured_->append(buffer, count);
    }

    close(pipefd_[0]);
  }

  StdCapture(const StdCapture&) = delete;
  StdCapture& operator=(const StdCapture&) = delete;

 private:
  int pipefd_[2]{-1, -1};
  int old_stdout_{-1};
  std::string* const captured_;
};

#endif

bool TestSignal(int signal) {
  int called = 0;
  // El árbol de componentes. Esto define cómo navegar usando el teclado.
  auto component = Renderer([&] {
    called++;
    std::ignore = std::raise(signal);
    called++;
    return text("");
  });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(component);

  EXPECT_EQ(called, 2);
  return true;
}
}  // namespace

TEST(ScreenInteractive, Signal_SIGTERM) {
  TestSignal(SIGTERM);
}
TEST(ScreenInteractive, Signal_SIGSEGV) {
  TestSignal(SIGSEGV);
}
TEST(ScreenInteractive, Signal_SIGINT) {
  TestSignal(SIGINT);
}
TEST(ScreenInteractive, Signal_SIGILL) {
  TestSignal(SIGILL);
}
TEST(ScreenInteractive, Signal_SIGABRT) {
  TestSignal(SIGABRT);
}
TEST(ScreenInteractive, Signal_SIGFPE) {
  TestSignal(SIGFPE);
}

// Prueba de regresión para:
// https://github.com/ArthurSonzogni/FTXUI/issues/402
TEST(ScreenInteractive, PostEventToNonActive) {
  auto screen = ScreenInteractive::FitComponent();
  screen.Post(Event::Custom);
}

// Prueba de regresión para:
// https://github.com/ArthurSonzogni/FTXUI/issues/402
TEST(ScreenInteractive, PostTaskToNonActive) {
  auto screen = ScreenInteractive::FitComponent();
  screen.Post([] {});
}

TEST(ScreenInteractive, CtrlC) {
  auto screen = ScreenInteractive::FitComponent();
  bool called = false;
  auto component = Renderer([&] {
    if (!called) {
      called = true;
      screen.PostEvent(Event::CtrlC);
    }
    return text("");
  });
  screen.Loop(component);
}

TEST(ScreenInteractive, CtrlC_Forced) {
  auto screen = ScreenInteractive::FitComponent();
  screen.ForceHandleCtrlC(true);
  auto component = Renderer([&] {
    screen.PostEvent(Event::CtrlC);
    return text("");
  });

  int ctrl_c_count = 0;
  component |= CatchEvent([&](Event event) {
    if (event != Event::CtrlC) {
      return false;
    }

    ++ctrl_c_count;

    if (ctrl_c_count == 100) {
      return false;
    }

    return true;
  });
  screen.Loop(component);

  ASSERT_LE(ctrl_c_count, 50);
}

TEST(ScreenInteractive, CtrlC_NotForced) {
  auto screen = ScreenInteractive::FitComponent();
  screen.ForceHandleCtrlC(false);
  auto component = Renderer([&] {
    screen.PostEvent(Event::CtrlC);
    return text("");
  });

  int ctrl_c_count = 0;
  component |= CatchEvent([&](Event event) {
    if (event != Event::CtrlC) {
      return false;
    }

    ++ctrl_c_count;

    if (ctrl_c_count == 100) {
      return false;
    }

    return true;
  });
  screen.Loop(component);

  ASSERT_GE(ctrl_c_count, 50);
}

// Prueba de regresión para:
// https://github.com/ArthurSonzogni/FTXUI/pull/1064/files
TEST(ScreenInteractive, FixedSizeInitialFrame) {
#if defined(__unix__)
  std::string output;
  {
    auto capture = StdCapture(&output);

    auto screen = ScreenInteractive::FixedSize(2, 2);
    auto component = Renderer([&] { return text("AB"); });

    Loop loop(&screen, component);
    loop.RunOnce();
  }
  using namespace std::string_view_literals;

  auto expected =
      // Instala ScreenInteractive.
      "\0"           // Vacía stdout.
      "\x1BP$q q"    // Establece la forma del cursor a 1 (bloque).
      "\x1B\\"       // Reinicia la posición del cursor.
      "\x1B[?7l"     // Deshabilita el ajuste de línea.
      "\x1B[?1000h"  // Habilita el seguimiento del ratón.
      "\x1B[?1003h"  // Habilita el seguimiento del movimiento del ratón.
      "\x1B[?1015h"  // Habilita el seguimiento de la rueda del ratón.
      "\x1B[?1006h"  // Habilita el seguimiento del ratón SGR.
      "\0"           // Vacía stdout.

      // Reinicia la pantalla.
      "\r"       // Reinicia la posición del cursor.
      "\x1B[2K"  // Limpia la línea.
      "\x1B[1A"  // Mueve el cursor una línea hacia arriba.
      "\x1B[2K"  // Limpia la línea.

      // Imprime el documento.
      "AB\r\n"  // Imprime "AB" y salta a la siguiente línea.
      "  "      // Imprime dos espacios para rellenar la línea.

      // Establece la posición del cursor.
      "\x1B[1D"    // Mueve el cursor un carácter a la izquierda.
      "\x1B[?25l"  // Oculta el cursor.

      // Vaciar
      "\0"  // Vacía stdout.

      // Desinstala ScreenInteractive.
      "\x1B[1C"      // Mueve el cursor un carácter a la derecha.
      "\x1B[?1006l"  // Deshabilita el seguimiento del ratón SGR.
      "\x1B[?1015l"  // Deshabilita el seguimiento de la rueda del ratón.
      "\x1B[?1003l"  // Deshabilita el seguimiento del movimiento del ratón.
      "\x1B[?1000l"  // Deshabilita el seguimiento del ratón.
      "\x1B[?7h"     // Habilita el ajuste de línea.
      "\x1B[?25h"    // Muestra el cursor.
      "\x1B[1 q"     // Establece la forma del cursor a 1 (bloque).
      "\0"           // Vacía stdout.

      // Salta una línea para evitar que el prompt se imprima sobre el último dibujo.
      "\r\n"sv;
  ASSERT_EQ(expected, output);
#endif
}

}  // namespace ftxui