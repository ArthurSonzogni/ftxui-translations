// Copyright 2025 Arthur Sonzogni. All rights reserved.
// 本源代码的使用受 MIT 许可证的约束，该许可证可在
// LICENSE 文件中找到。
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

// 管道输入功能的测试夹具
class PipedInputTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // 保存原始 stdin 以供恢复
    original_stdin_ = dup(STDIN_FILENO);
  }

  void TearDown() override {
    // 恢复原始 stdin
    if (original_stdin_ >= 0) {
      dup2(original_stdin_, STDIN_FILENO);
      close(original_stdin_);
    }
  }

  // 创建一个管道并将 stdin 重定向以从中读取
  void SetupPipedStdin() {
    if (pipe(pipe_fds_) == 0) {
      dup2(pipe_fds_[0], STDIN_FILENO);
      close(pipe_fds_[0]);
      // 保持写入端打开以写入测试数据
      piped_stdin_setup_ = true;
    }
  }

  // 将测试数据写入管道 stdin
  void WriteToPipedStdin(const std::string& data) {
    if (piped_stdin_setup_) {
      write(pipe_fds_[1], data.c_str(), data.length());
      close(pipe_fds_[1]);  // 关闭写入端以表示 EOF
    }
  }

  // 检查 /dev/tty 是否可用（在某些 CI 环境中不可用）
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
  // 测试 HandlePipedInput 默认启用
  if (!IsTtyAvailable()) {
    GTEST_SKIP() << "/dev/tty 在此环境中不可用";
  }

  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // 由于 HandlePipedInput 默认开启，Install 应该重定向 stdin
  screen.Install();

  // Stdin 应该是 tty
  EXPECT_TRUE(isatty(STDIN_FILENO));

  screen.Uninstall();
}

TEST_F(PipedInputTest, ExplicitlyDisabled) {
  // 测试明确禁用是否有效
  auto screen = ScreenInteractive::TerminalOutput();
  screen.HandlePipedInput(false);
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  screen.Install();

  // 由于功能已禁用，stdin 仍应是管道
  EXPECT_FALSE(isatty(STDIN_FILENO));

  screen.Uninstall();
}

TEST_F(PipedInputTest, ExplicitlyEnabled) {
  if (!IsTtyAvailable()) {
    GTEST_SKIP() << "/dev/tty 在此环境中不可用";
  }

  auto screen = ScreenInteractive::TerminalOutput();
  screen.HandlePipedInput(true);  // 明确启用
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // 安装前：stdin 应该是管道
  EXPECT_FALSE(isatty(STDIN_FILENO));

  screen.Install();

  // 安装后，并处理管道输入：stdin 应该重定向到 tty
  EXPECT_TRUE(isatty(STDIN_FILENO));

  screen.Uninstall();

  // 卸载后：stdin 应该恢复到原始状态
  // 注意：这将是我们设置的管道，因此它应该是非 tty
  EXPECT_FALSE(isatty(STDIN_FILENO));
}

TEST_F(PipedInputTest, NormalStdinUnchanged) {
  // 测试正常的 stdin（非管道）不受影响
  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  // 不设置管道 stdin - 使用正常的 stdin
  bool original_isatty = isatty(STDIN_FILENO);

  screen.Install();

  // Stdin 应该保持不变
  EXPECT_EQ(original_isatty, isatty(STDIN_FILENO));

  screen.Uninstall();

  // Stdin 应该仍然不变
  EXPECT_EQ(original_isatty, isatty(STDIN_FILENO));
}

TEST_F(PipedInputTest, MultipleInstallUninstallCycles) {
  if (!IsTtyAvailable()) {
    GTEST_SKIP() << "/dev/tty 在此环境中不可用";
  }

  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // 第一个周期
  screen.Install();
  EXPECT_TRUE(isatty(STDIN_FILENO));
  screen.Uninstall();
  EXPECT_FALSE(isatty(STDIN_FILENO));

  // 第二个周期应该以同样的方式工作
  screen.Install();
  EXPECT_TRUE(isatty(STDIN_FILENO));
  screen.Uninstall();
  EXPECT_FALSE(isatty(STDIN_FILENO));
}

TEST_F(PipedInputTest, HandlePipedInputMethodBehavior) {
  auto screen = ScreenInteractive::TerminalOutput();

  // 测试方法可以被多次调用
  screen.HandlePipedInput(true);
  screen.HandlePipedInput(false);
  screen.HandlePipedInput(true);

  // 最后一次调用后应该启用
  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  if (IsTtyAvailable()) {
    screen.Install();
    EXPECT_TRUE(isatty(STDIN_FILENO));
    screen.Uninstall();
  }
}

// 测试当 /dev/tty 不可用时的优雅回退
// 此测试模拟了容器等环境中 /dev/tty 可能不存在的情况
TEST_F(PipedInputTest, GracefulFallbackWhenTtyUnavailable) {
  auto screen = ScreenInteractive::TerminalOutput();
  auto component = Renderer([] { return text("test"); });

  SetupPipedStdin();
  WriteToPipedStdin("test data\n");

  // 此测试不直接模拟 /dev/tty 不可用，因为这在单元测试环境中很难做到，
  // 但代码路径会优雅地处理 freopen() 失败
  screen.Install();

  // 行为取决于 /dev/tty 是否可用
  // 如果可用，stdin 会被重定向；如果不可用，它会保持管道状态
  // 两种行为都是正确的

  screen.Uninstall();

  // 卸载后，stdin 应该被恢复
  EXPECT_FALSE(isatty(STDIN_FILENO));  // 应该仍然是我们的测试管道
}

}  // namespace

}  // namespace ftxui

#endif  // !defined(_WIN32) && !defined(__EMSCRIPTEN__)
