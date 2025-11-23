// Copyright 2021 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
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

// Capture la sortie standard (stdout) dans une chaîne de caractères.
class StdCapture {
 public:
  explicit StdCapture(std::string* captured) : captured_(captured) {
    if (pipe(pipefd_) != 0) {
      return;
    }
    old_stdout_ = dup(fileno(stdout));
    fflush(stdout);
    dup2(pipefd_[1], fileno(stdout));
    close(pipefd_[1]);  // Close the write end in the parent
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
  // The tree of components. This defines how to navigate using the keyboard.
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

// Test de régression pour :
// https://github.com/ArthurSonzogni/FTXUI/issues/402
TEST(ScreenInteractive, PostEventToNonActive) {
  auto screen = ScreenInteractive::FitComponent();
  screen.Post(Event::Custom);
}

// Test de régression pour :
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

// Test de régression pour :
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
      // Installe le ScreenInteractive.
      "\0"           // Vide stdout.
      "\x1BP$q q"    // Définit la forme du curseur sur 1 (bloc).
      "\x1B\\"       // Réinitialise la position du curseur.
      "\x1B[?7l"     // Désactive le retour à la ligne automatique.
      "\x1B[?1000h"  // Active le suivi de la souris.
      "\x1B[?1003h"  // Active le suivi des mouvements de la souris.
      "\x1B[?1015h"  // Active le suivi de la molette de la souris.
      "\x1B[?1006h"  // Active le suivi de la souris SGR.
      "\0"           // Vide stdout.

      // Réinitialise l'écran.
      "\r"       // Réinitialise la position du curseur.
      "\x1B[2K"  // Efface la ligne.
      "\x1B[1A"  // Déplace le curseur d'une ligne vers le haut.
      "\x1B[2K"  // Efface la ligne.

      // Imprime le document.
      "AB\r\n"  // Imprime "AB" et passe à la ligne suivante.
      "  "      // Imprime deux espaces pour remplir la ligne.

      // Définit la position du curseur.
      "\x1B[1D"    // Déplace le curseur d'un caractère vers la gauche.
      "\x1B[?25l"  // Cache le curseur.

      // Vider
      "\0"  // Vide stdout.

      // Désinstalle le ScreenInteractive.
      "\x1B[1C"      // Déplace le curseur d'un caractère vers la droite.
      "\x1B[?1006l"  // Désactive le suivi de la souris SGR.
      "\x1B[?1015l"  // Désactive le suivi de la molette de la souris.
      "\x1B[?1003l"  // Désactive le suivi des mouvements de la souris.
      "\x1B[?1000l"  // Désactive le suivi de la souris.
      "\x1B[?7h"     // Active le retour à la ligne automatique.
      "\x1B[?25h"    // Affiche le curseur.
      "\x1B[1 q"     // Définit la forme du curseur sur 1 (bloc).
      "\0"           // Vide stdout.

      // Saute une ligne pour éviter que l'invite ne soit imprimée sur le dernier dessin.
      "\r\n"sv;
  ASSERT_EQ(expected, output);
#endif
}

}  // namespace ftxui
