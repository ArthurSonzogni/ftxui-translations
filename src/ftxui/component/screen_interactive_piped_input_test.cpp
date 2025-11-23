// Copyright 2025 Arthur Sonzogni. All rights reserved.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#if !defined(_WIN32) && !defined(__EMSCRIPTEN__)

namespace ftxui {

namespace {

// Fixture de test pour la fonctionnalité d'entrée par pipe
class PipedInputTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Sauvegarder stdin original pour la restauration
    original_stdin_ = dup(STDIN_FILENO);
  }

  void TearDown() override {
    // Restaurer stdin original
    if (original_stdin_ >= 0) {
      dup2(original_stdin_, STDIN_FILENO);
      close(original_stdin_);
    }
  }

  // Créer un pipe et rediriger stdin pour lire à partir de celui-ci
  void SetupPipedStdin() {
    if (pipe(pipe_fds_) == 0) {
      dup2(pipe_fds_[0], STDIN_FILENO);
      close(pipe_fds_[0]);
      // Garder l'extrémité d'écriture ouverte pour écrire les données de test
      piped_stdin_setup_ = true;
    }
  }

  // Écrire des données de test dans le stdin par pipe
  void WriteToPipedStdin(const std::string& data) {
    if (piped_stdin_setup_) {
      write(pipe_fds_[1], data.c_str(), data.length());
      close(pipe_fds_[1]);  // Fermer l'extrémité d'écriture pour signaler la fin de fichier
    }
  }

  // Vérifier si /dev/tty est disponible (non disponible dans certains environnements CI)
  bool IsTtyAvailable() {
    struct stat st;
    return stat("/dev/tty", &st) == 0;
  }

 private:
  int original_stdin_ = -1;
  int pipe_fds_[2] = {-1, -1};
  bool piped_stdin_setup_ = false;
};

TEST_F(PipedInputTest, DefaultBehaviorEnabled) {
  // Tester que HandlePipedInput est activé par défaut
  if (!IsTtyAvailable()) {
    GTEST_SKIP() << "/dev/tty non disponible dans cet environnement";
  }

  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // Install devrait rediriger stdin puisque HandlePipedInput est activé par défaut
  screen.Install();

  // Stdin devrait être le tty
  EXPECT_TRUE(isatty(STDIN_FILENO));

  screen.Uninstall();
}

TEST_F(PipedInputTest, ExplicitlyDisabled) {
  // Tester que la désactivation explicite fonctionne
  auto screen = ScreenInteractive::TerminalOutput();
  screen.HandlePipedInput(false);
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  screen.Install();

  // Stdin devrait toujours être le pipe puisque la fonctionnalité est désactivée
  EXPECT_FALSE(isatty(STDIN_FILENO));

  screen.Uninstall();
}

TEST_F(PipedInputTest, ExplicitlyEnabled) {
  if (!IsTtyAvailable()) {
    GTEST_SKIP() << "/dev/tty non disponible dans cet environnement";
  }

  auto screen = ScreenInteractive::TerminalOutput();
  screen.HandlePipedInput(true);  // Activer explicitement
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // Avant l'installation : stdin devrait être pipé
  EXPECT_FALSE(isatty(STDIN_FILENO));

  screen.Install();

  // Après l'installation avec la gestion de l'entrée par pipe : stdin devrait être redirigé vers tty
  EXPECT_TRUE(isatty(STDIN_FILENO));

  screen.Uninstall();

  // Après la désinstallation : stdin devrait être restauré à son état original
  // Remarque : Ce sera le pipe que nous avons configuré, donc il ne devrait pas être un tty
  EXPECT_FALSE(isatty(STDIN_FILENO));
}

TEST_F(PipedInputTest, NormalStdinUnchanged) {
  // Tester que le stdin normal (non pipé) n'est pas affecté
  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  // Ne pas configurer le stdin par pipe - utiliser le stdin normal
  bool original_isatty = isatty(STDIN_FILENO);

  screen.Install();

  // Stdin devrait rester inchangé
  EXPECT_EQ(original_isatty, isatty(STDIN_FILENO));

  screen.Uninstall();

  // Stdin devrait toujours être inchangé
  EXPECT_EQ(original_isatty, isatty(STDIN_FILENO));
}

TEST_F(PipedInputTest, MultipleInstallUninstallCycles) {
  if (!IsTtyAvailable()) {
    GTEST_SKIP() << "/dev/tty non disponible dans cet environnement";
  }

  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // Premier cycle
  screen.Install();
  EXPECT_TRUE(isatty(STDIN_FILENO));
  screen.Uninstall();
  EXPECT_FALSE(isatty(STDIN_FILENO));

  // Le deuxième cycle devrait fonctionner de la même manière
  screen.Install();
  EXPECT_TRUE(isatty(STDIN_FILENO));
  screen.Uninstall();
  EXPECT_FALSE(isatty(STDIN_FILENO));
}

TEST_F(PipedInputTest, HandlePipedInputMethodBehavior) {
  auto screen = ScreenInteractive::TerminalOutput();

  // Tester que la méthode peut être appelée plusieurs fois
  screen.HandlePipedInput(true);
  screen.HandlePipedInput(false);
  screen.HandlePipedInput(true);

  // Devrait être activé après le dernier appel
  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  if (IsTtyAvailable()) {
    screen.Install();
    EXPECT_TRUE(isatty(STDIN_FILENO));
    screen.Uninstall();
  }
}

// Tester le retour à la normale en douceur lorsque /dev/tty n'est pas disponible
// Ce test simule des environnements comme les conteneurs où /dev/tty pourrait ne pas exister
TEST_F(PipedInputTest, GracefulFallbackWhenTtyUnavailable) {
  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // Ce test ne simule pas directement l'indisponibilité de /dev/tty car c'est difficile
  // à faire dans un environnement de test unitaire, mais le chemin du code gère
  // l'échec de freopen() en douceur
  screen.Install();

  // Le comportement dépend de la disponibilité de /dev/tty
  // S'il est disponible, stdin est redirigé ; sinon, il reste pipé
  // Les deux comportements sont corrects

  screen.Uninstall();

  // Après la désinstallation, stdin devrait être restauré
  EXPECT_FALSE(isatty(STDIN_FILENO));  // Devrait toujours être notre pipe de test
}

}  // namespace

}  // namespace ftxui

#endif  // !defined(_WIN32) && !defined(__EMSCRIPTEN__)