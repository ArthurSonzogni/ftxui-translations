// 版權所有 2025 Arthur Sonzogni。保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱 LICENSE 檔案。
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

// Test fixture for piped input functionality
class PipedInputTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // 儲存原始標準輸入 (stdin) 以便恢復
    original_stdin_ = dup(STDIN_FILENO);
  }

  void TearDown() override {
    // 恢復原始標準輸入 (stdin)
    if (original_stdin_ >= 0) {
      dup2(original_stdin_, STDIN_FILENO);
      close(original_stdin_);
    }
  }

  // 建立管道並將標準輸入 (stdin) 重定向以從中讀取
  void SetupPipedStdin() {
    if (pipe(pipe_fds_) == 0) {
      dup2(pipe_fds_[0], STDIN_FILENO);
      close(pipe_fds_[0]);
      // 保持寫入端開啟以寫入測試資料
      piped_stdin_setup_ = true;
    }
  }

  // 將測試資料寫入管道的標準輸入 (stdin)
  void WriteToPipedStdin(const std::string& data) {
    if (piped_stdin_setup_) {
      write(pipe_fds_[1], data.c_str(), data.length());
      close(pipe_fds_[1]);  // 關閉寫入端以表示 EOF
    }
  }

  // 檢查 /dev/tty 是否可用 (在某些 CI 環境中不可用)
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
  // 測試 HandlePipedInput 預設啟用
  if (!IsTtyAvailable()) {
    GTEST_SKIP() << "/dev/tty 在此環境中不可用";
  }

  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // 由於 HandlePipedInput 預設開啟，Install 應該重定向標準輸入 (stdin)
  screen.Install();

  // 標準輸入 (stdin) 應該是 tty
  EXPECT_TRUE(isatty(STDIN_FILENO));

  screen.Uninstall();
}

TEST_F(PipedInputTest, ExplicitlyDisabled) {
  // 測試明確禁用是否有效
  auto screen = ScreenInteractive::TerminalOutput();
  screen.HandlePipedInput(false);
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  screen.Install();

  // 由於功能已禁用，標準輸入 (stdin) 仍應是管道
  EXPECT_FALSE(isatty(STDIN_FILENO));

  screen.Uninstall();
}

TEST_F(PipedInputTest, ExplicitlyEnabled) {
  if (!IsTtyAvailable()) {
    GTEST_SKIP() << "/dev/tty 在此環境中不可用";
  }

  auto screen = ScreenInteractive::TerminalOutput();
  screen.HandlePipedInput(true);  // 明確啟用
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // 安裝前：標準輸入 (stdin) 應該是管道
  EXPECT_FALSE(isatty(STDIN_FILENO));

  screen.Install();

  // 安裝後並處理管道輸入：標準輸入 (stdin) 應該重定向到 tty
  EXPECT_TRUE(isatty(STDIN_FILENO));

  screen.Uninstall();

  // 卸載後：標準輸入 (stdin) 應恢復到原始狀態
  // 注意：這將是我們設定的管道，因此它應該是非 tty
  EXPECT_FALSE(isatty(STDIN_FILENO));
}

TEST_F(PipedInputTest, NormalStdinUnchanged) {
  // 測試正常標準輸入 (stdin) (未經管道) 不受影響
  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  // 不要設定管道標準輸入 (stdin) - 使用正常標準輸入 (stdin)
  bool original_isatty = isatty(STDIN_FILENO);

  screen.Install();

  // 標準輸入 (stdin) 應保持不變
  EXPECT_EQ(original_isatty, isatty(STDIN_FILENO));

  screen.Uninstall();

  // 標準輸入 (stdin) 仍應保持不變
  EXPECT_EQ(original_isatty, isatty(STDIN_FILENO));
}

TEST_F(PipedInputTest, MultipleInstallUninstallCycles) {
  if (!IsTtyAvailable()) {
    GTEST_SKIP() << "/dev/tty 在此環境中不可用";
  }

  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // 第一個循環
  screen.Install();
  EXPECT_TRUE(isatty(STDIN_FILENO));
  screen.Uninstall();
  EXPECT_FALSE(isatty(STDIN_FILENO));

  // 第二個循環應該相同
  screen.Install();
  EXPECT_TRUE(isatty(STDIN_FILENO));
  screen.Uninstall();
  EXPECT_FALSE(isatty(STDIN_FILENO));
}

TEST_F(PipedInputTest, HandlePipedInputMethodBehavior) {
  auto screen = ScreenInteractive::TerminalOutput();

  // 測試方法可以被呼叫多次
  screen.HandlePipedInput(true);
  screen.HandlePipedInput(false);
  screen.HandlePipedInput(true);

  // 最後一次呼叫後應啟用
  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  if (IsTtyAvailable()) {
    screen.Install();
    EXPECT_TRUE(isatty(STDIN_FILENO));
    screen.Uninstall();
  }
}

// 測試當 /dev/tty 不可用時的優雅回退
// 此測試模擬容器等環境，其中 /dev/tty 可能不存在
TEST_F(PipedInputTest, GracefulFallbackWhenTtyUnavailable) {
  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // 此測試不直接模擬 /dev/tty 不可用，因為在單元測試環境中很難做到，
  // 但程式碼路徑會優雅地處理 freopen() 失敗的情況
  screen.Install();

  // 行為取決於 /dev/tty 是否可用
  // 如果可用，標準輸入 (stdin) 會被重定向；如果不可用，它會保持管道狀態
  // 這兩種行為都是正確的

  screen.Uninstall();

  // 卸載後，標準輸入 (stdin) 應恢復
  EXPECT_FALSE(isatty(STDIN_FILENO));  // 應該仍然是我們的測試管道
}

}  // namespace

}  // namespace ftxui

#endif  // !defined(_WIN32) && !defined(__EMSCRIPTEN__)
