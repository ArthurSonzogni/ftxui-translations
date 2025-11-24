// Copyright 2025 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスによって管理されています。
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

// パイプ入力機能のテストフィクスチャ
class PipedInputTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // 復元のために元の標準入力を保存する
    original_stdin_ = dup(STDIN_FILENO);
  }

  void TearDown() override {
    // 元の標準入力を復元する
    if (original_stdin_ >= 0) {
      dup2(original_stdin_, STDIN_FILENO);
      close(original_stdin_);
    }
  }

  // パイプを作成し、そこから読み取るように標準入力をリダイレクトする
  void SetupPipedStdin() {
    if (pipe(pipe_fds_) == 0) {
      dup2(pipe_fds_[0], STDIN_FILENO);
      close(pipe_fds_[0]);
      // テストデータを書き込むために書き込み側を開いたままにする
      piped_stdin_setup_ = true;
    }
  }

  // パイプされた標準入力にテストデータを書き込む
  void WriteToPipedStdin(const std::string& data) {
    if (piped_stdin_setup_) {
      write(pipe_fds_[1], data.c_str(), data.length());
      close(pipe_fds_[1]);  // EOF を通知するために書き込み側を閉じる
    }
  }

  // /dev/tty が利用可能かどうかを確認する（一部の CI 環境では利用不可）
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
  // HandlePipedInput がデフォルトで有効になっていることをテストする
  if (!IsTtyAvailable()) {
    GTEST_SKIP() << "/dev/tty not available in this environment";
  }

  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // HandlePipedInput がデフォルトでオンになっているため、インストールは標準入力をリダイレクトするはずです
  screen.Install();

  // 標準入力は tty であるべきです
  EXPECT_TRUE(isatty(STDIN_FILENO));

  screen.Uninstall();
}

TEST_F(PipedInputTest, ExplicitlyDisabled) {
  // 明示的に無効にすることが機能することをテストする
  auto screen = ScreenInteractive::TerminalOutput();
  screen.HandlePipedInput(false);
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  screen.Install();

  // 機能が無効になっているため、標準入力はまだパイプであるはずです
  EXPECT_FALSE(isatty(STDIN_FILENO));

  screen.Uninstall();
}

TEST_F(PipedInputTest, ExplicitlyEnabled) {
  if (!IsTtyAvailable()) {
    GTEST_SKIP() << "/dev/tty not available in this environment";
  }

  auto screen = ScreenInteractive::TerminalOutput();
  screen.HandlePipedInput(true);  // 明示的に有効にする
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // インストール前：標準入力はパイプであるべきです
  EXPECT_FALSE(isatty(STDIN_FILENO));

  screen.Install();

  // パイプ入力処理後のインストール：標準入力は tty にリダイレクトされるべきです
  EXPECT_TRUE(isatty(STDIN_FILENO));

  screen.Uninstall();

  // アンインストール後：標準入力は元の状態に戻されるべきです
  // 注：これは設定したパイプなので、非 tty であるはずです
  EXPECT_FALSE(isatty(STDIN_FILENO));
}

TEST_F(PipedInputTest, NormalStdinUnchanged) {
  // 通常の標準入力（パイプされていない）が影響を受けないことをテストする
  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  // パイプされた標準入力を設定しない - 通常の標準入力を使用する
  bool original_isatty = isatty(STDIN_FILENO);

  screen.Install();

  // 標準入力は変更されないはずです
  EXPECT_EQ(original_isatty, isatty(STDIN_FILENO));

  screen.Uninstall();

  // 標準入力はまだ変更されないはずです
  EXPECT_EQ(original_isatty, isatty(STDIN_FILENO));
}

TEST_F(PipedInputTest, MultipleInstallUninstallCycles) {
  if (!IsTtyAvailable()) {
    GTEST_SKIP() << "/dev/tty not available in this environment";
  }

  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // First cycle
  screen.Install();
  EXPECT_TRUE(isatty(STDIN_FILENO));
  screen.Uninstall();
  EXPECT_FALSE(isatty(STDIN_FILENO));

  // 2 回目のサイクルも同様に機能するはずです
  screen.Install();
  EXPECT_TRUE(isatty(STDIN_FILENO));
  screen.Uninstall();
  EXPECT_FALSE(isatty(STDIN_FILENO));
}

TEST_F(PipedInputTest, HandlePipedInputMethodBehavior) {
  auto screen = ScreenInteractive::TerminalOutput();

  // テストメソッドが複数回呼び出せることをテストする
  screen.HandlePipedInput(true);
  screen.HandlePipedInput(false);
  screen.HandlePipedInput(true);

  // 最後の呼び出し後に有効になっているはずです
  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  if (IsTtyAvailable()) {
    screen.Install();
    EXPECT_TRUE(isatty(STDIN_FILENO));
    screen.Uninstall();
  }
}

// /dev/tty が利用できない場合の正常なフォールバックをテストする
// このテストは、/dev/tty が存在しない可能性のあるコンテナなどの環境をシミュレートします
TEST_F(PipedInputTest, GracefulFallbackWhenTtyUnavailable) {
  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // このテストは、単体テスト環境で実行するのが難しいため、/dev/tty の利用不可を直接モックしませんが、コードパスは freopen() の失敗を正常に処理します
  screen.Install();

  // 動作は /dev/tty が利用可能かどうかに依存します
  // 利用可能な場合、標準入力はリダイレクトされます。そうでない場合、パイプされたままになります
  // どちらの動作も正しいです

  screen.Uninstall();

  // アンインストール後、標準入力は復元されるべきです
  EXPECT_FALSE(isatty(STDIN_FILENO));  // まだテストパイプであるはずです
}

}  // namespace

}  // namespace ftxui

#endif  // !defined(_WIN32) && !defined(__EMSCRIPTEN__)