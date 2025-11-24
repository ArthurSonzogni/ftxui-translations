// Copyright 2021 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスに従って管理されます。
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

// 標準出力 (stdout) を文字列にキャプチャします。
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
  // コンポーネントツリー。これはキーボードを使用してナビゲートする方法を定義します。
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

// 回帰テスト:
// https://github.com/ArthurSonzogni/FTXUI/issues/402
TEST(ScreenInteractive, PostEventToNonActive) {
  auto screen = ScreenInteractive::FitComponent();
  screen.Post(Event::Custom);
}

// 回帰テスト:
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

// 回帰テスト:
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
      // ScreenInteractiveをインストールします。
      "\0"           // 標準出力をフラッシュ。
      "\x1B\\"       // カーソル位置をリセットします。
      "\x1B[?7l"     // 行の折り返しを無効にします。
      "\x1B[?1000h"  // マウストラッキングを有効にします。
      "\x1B[?1003h"  // マウスモーション追跡を有効にします。
      "\x1B[?1015h"  // マウスホイール追跡を有効にします。
      "\0"           // 標準出力をフラッシュ。

      // 画面をリセットします。
      "\r"       // カーソル位置をリセットします。
      "\x1B[2K"  // 行をクリアします。
      "\x1B[1A"  // カーソルを1行上に移動します。
      "\x1B[2K"  // 行をクリアします。

      // ドキュメントを印刷します。
      "AB\r\n"  // "AB"を印刷して次の行に移動します。
      "  "      // 行を埋めるために2つのスペースを印刷します。

      // カーソル位置を設定します。
      "\x1B[1D"    // カーソルを左に1文字移動します。
      "\x1B[?25l"  // カーソルを非表示にします。

      // フラッシュ
      "\0"  // 標準出力をフラッシュ。

      // ScreenInteractiveをアンインストールします。
      "\x1B[1C"      // カーソルを右に1文字移動します。
      "\x1B[?1006l"  // SGR マウストラッキングを無効にします。
      "\x1B[?1015l"  // マウスホイール追跡を無効にします。
      "\x1B[?1003l"  // マウスモーション追跡を無効にします。
      "\x1B[?1000l"  // マウストラッキングを無効にします。
      "\x1B[?7h"     // 行の折り返しを有効にします。
      "\x1B[?25h"    // カーソルを表示します。
      "\0"           // 標準出力をフラッシュ。

      // 最後の描画の上にプロンプトが印刷されないように、1行スキップします。
      "\r\n"sv;
  ASSERT_EQ(expected, output);
#endif
}

}  // namespace ftxui
