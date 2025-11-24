// Copyright 2021 Arthur Sonzogni. 版權所有。
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱 LICENSE 檔案。
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

// 將標準輸出 (stdout) 捕獲到字串中。
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
  // 元件樹。這定義了如何使用鍵盤進行導航。
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

// 回歸測試，針對：
// https://github.com/ArthurSonzogni/FTXUI/issues/402
TEST(ScreenInteractive, PostEventToNonActive) {
  auto screen = ScreenInteractive::FitComponent();
  screen.Post(Event::Custom);
}

// 回歸測試，針對：
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

// 回歸測試，針對：
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
      // 安裝 ScreenInteractive。
      "\0"           // 刷新 stdout。
      "\x1BP$q q"    // 將游標形狀設定為 1 (區塊)。
      "\x1B\\"       // 重設游標位置。
      "\x1B[?7l"     // 禁用自動換行。
      "\x1B[?1000h"  // 啟用滑鼠追蹤。
      "\x1B[?1003h"  // 啟用滑鼠移動追蹤。
      "\x1B[?1015h"  // 啟用滑鼠滾輪追蹤。
      "\x1B[?1006h"  // 啟用 SGR 滑鼠追蹤。
      "\0"           // 刷新 stdout。

      // 重設螢幕。
      "\r"       // 重設游標位置。
      "\x1B[2K"  // 清除該行。
      "\x1B[1A"  // 游標上移一行。
      "\x1B[2K"  // 清除該行。

      // 列印文件。
      "AB\r\n"  // 列印 "AB" 並移至下一行。
      "  "      // 列印兩個空格以填滿該行。

      // 設定游標位置。
      "\x1B[1D"    // 游標左移一個字元。
      "\x1B[?25l"  // 隱藏游標。

      // 刷新
      "\0"  // 刷新 stdout。

      // 卸載 ScreenInteractive。
      "\x1B[1C"      // 游標右移一個字元。
      "\x1B[?1006l"  // 禁用 SGR 滑鼠追蹤。
      "\x1B[?1015l"  // 禁用滑鼠滾輪追蹤。
      "\x1B[?1003l"  // 禁用滑鼠移動追蹤。
      "\x1B[?1000l"  // 禁用滑鼠追蹤。
      "\x1B[?7h"     // 啟用自動換行。
      "\x1B[?25h"    // 顯示游標。
      "\x1B[1 q"     // 將游標形狀設定為 1 (區塊)。
      "\0"           // 刷新 stdout。

      // 跳過一行以避免提示字元列印在最後的繪圖上方。
      "\r\n"sv;
  ASSERT_EQ(expected, output);
#endif
}

}  // namespace ftxui