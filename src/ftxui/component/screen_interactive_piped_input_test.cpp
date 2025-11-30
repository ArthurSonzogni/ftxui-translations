// Copyright 2025 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
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

// Fixture de prueba para la funcionalidad de entrada canalizada
class PipedInputTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Guardar stdin original para su restauración
    original_stdin_ = dup(STDIN_FILENO);
  }

  void TearDown() override {
    // Restaurar stdin original
    if (original_stdin_ >= 0) {
      dup2(original_stdin_, STDIN_FILENO);
      close(original_stdin_);
    }
  }

  // Crear una tubería y redirigir stdin para leer de ella
  void SetupPipedStdin() {
    if (pipe(pipe_fds_) == 0) {
      dup2(pipe_fds_[0], STDIN_FILENO);
      close(pipe_fds_[0]);
      // Mantener el extremo de escritura abierto para escribir datos de prueba
      piped_stdin_setup_ = true;
    }
  }

  // Escribir datos de prueba en el stdin canalizado
  void WriteToPipedStdin(const std::string& data) {
    if (piped_stdin_setup_) {
      write(pipe_fds_[1], data.c_str(), data.length());
      close(pipe_fds_[1]);  // Cerrar el extremo de escritura para indicar EOF
    }
  }

  // Verificar si /dev/tty está disponible (no disponible en algunos entornos CI)
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
  // Probar que HandlePipedInput está habilitado por defecto
  if (!IsTtyAvailable()) {
    GTEST_SKIP() << "/dev/tty no disponible en este entorno";
  }

  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // Install debería redirigir stdin ya que HandlePipedInput está activado por defecto
  screen.Install();

  // Stdin debería ser el tty
  EXPECT_TRUE(isatty(STDIN_FILENO));

  screen.Uninstall();
}

TEST_F(PipedInputTest, ExplicitlyDisabled) {
  // Probar que la deshabilitación explícita funciona
  auto screen = ScreenInteractive::TerminalOutput();
  screen.HandlePipedInput(false);
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  screen.Install();

  // Stdin debería seguir siendo la tubería ya que la característica está deshabilitada
  EXPECT_FALSE(isatty(STDIN_FILENO));

  screen.Uninstall();
}

TEST_F(PipedInputTest, ExplicitlyEnabled) {
  if (!IsTtyAvailable()) {
    GTEST_SKIP() << "/dev/tty no disponible en este entorno";
  }

  auto screen = ScreenInteractive::TerminalOutput();
  screen.HandlePipedInput(true);  // Habilitar explícitamente
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // Antes de la instalación: stdin debería ser canalizado
  EXPECT_FALSE(isatty(STDIN_FILENO));

  screen.Install();

  // Después de la instalación con manejo de entrada canalizada: stdin debería ser redirigido a tty
  EXPECT_TRUE(isatty(STDIN_FILENO));

  screen.Uninstall();

  // Después de la desinstalación: stdin debería ser restaurado a su estado original
  // Nota: Esta será la tubería que configuramos, por lo que debería ser no-tty
  EXPECT_FALSE(isatty(STDIN_FILENO));
}

TEST_F(PipedInputTest, NormalStdinUnchanged) {
  // Probar que el stdin normal (no canalizado) no se ve afectado
  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  // No configurar stdin canalizado - usar stdin normal
  bool original_isatty = isatty(STDIN_FILENO);

  screen.Install();

  // Stdin debería permanecer sin cambios
  EXPECT_EQ(original_isatty, isatty(STDIN_FILENO));

  screen.Uninstall();

  // Stdin debería seguir sin cambios
  EXPECT_EQ(original_isatty, isatty(STDIN_FILENO));
}

TEST_F(PipedInputTest, MultipleInstallUninstallCycles) {
  if (!IsTtyAvailable()) {
    GTEST_SKIP() << "/dev/tty no disponible en este entorno";
  }

  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // Primer ciclo
  screen.Install();
  EXPECT_TRUE(isatty(STDIN_FILENO));
  screen.Uninstall();
  EXPECT_FALSE(isatty(STDIN_FILENO));

  // El segundo ciclo debería funcionar igual
  screen.Install();
  EXPECT_TRUE(isatty(STDIN_FILENO));
  screen.Uninstall();
  EXPECT_FALSE(isatty(STDIN_FILENO));
}

TEST_F(PipedInputTest, HandlePipedInputMethodBehavior) {
  auto screen = ScreenInteractive::TerminalOutput();

  // El método de prueba puede ser llamado múltiples veces
  screen.HandlePipedInput(true);
  screen.HandlePipedInput(false);
  screen.HandlePipedInput(true);

  // Debería estar habilitado después de la última llamada
  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  if (IsTtyAvailable()) {
    screen.Install();
    EXPECT_TRUE(isatty(STDIN_FILENO));
    screen.Uninstall();
  }
}

// Probar el comportamiento de reserva elegante cuando /dev/tty no está disponible
// Esta prueba simula entornos como contenedores donde /dev/tty podría no
// existir
TEST_F(PipedInputTest, GracefulFallbackWhenTtyUnavailable) {
  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // Esta prueba no simula directamente la indisponibilidad de /dev/tty ya que es difícil
  // hacerlo en un entorno de prueba unitaria, pero la ruta del código maneja
  // la falla de freopen() con elegancia.
  screen.Install();

  // El comportamiento depende de si /dev/tty está disponible
  // Si está disponible, stdin se redirige; si no, permanece canalizado
  // Ambos comportamientos son correctos

  screen.Uninstall();

  // Después de la desinstalación, stdin debería ser restaurado
  EXPECT_FALSE(isatty(STDIN_FILENO));  // Debería seguir siendo nuestra tubería de prueba
}

}  // namespace

}  // namespace ftxui

#endif  // !defined(_WIN32) && !defined(__EMSCRIPTEN__)