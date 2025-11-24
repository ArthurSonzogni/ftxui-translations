// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_COMPONENT_SCREEN_INTERACTIVE_HPP
#define FTXUI_COMPONENT_SCREEN_INTERACTIVE_HPP

#include <atomic>      // for atomic
#include <functional>  // for function
#include <memory>      // for shared_ptr
#include <string>      // for string

#include "ftxui/component/animation.hpp"       // for TimePoint
#include "ftxui/component/captured_mouse.hpp"  // for CapturedMouse
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/component/task.hpp"            // for Task, Closure
#include "ftxui/dom/selection.hpp"             // for SelectionOption
#include "ftxui/screen/screen.hpp"             // for Screen

namespace ftxui {
class ComponentBase;
class Loop;
struct Event;

using Component = std::shared_ptr<ComponentBase>;
class ScreenInteractivePrivate;

namespace task {
class TaskRunner;
}

/// @brief ScreenInteractive はイベントを処理し、メインループを実行し、コンポーネントを管理できる `Screen` です。
///
/// @ingroup component
class ScreenInteractive : public Screen {
 public:
  // Constructors:
  static ScreenInteractive FixedSize(int dimx, int dimy);
  static ScreenInteractive Fullscreen();
  static ScreenInteractive FullscreenPrimaryScreen();
  static ScreenInteractive FullscreenAlternateScreen();
  static ScreenInteractive FitComponent();
  static ScreenInteractive TerminalOutput();

  // Destructor.
  ~ScreenInteractive() override;

  // オプション。Loop() の前に呼び出す必要があります。
  void TrackMouse(bool enable = true);
  void HandlePipedInput(bool enable = true);

  // 現在アクティブなスクリーンを返します。アクティブなスクリーンがない場合は nullptr を返します。
  static ScreenInteractive* Active();

  // メインループを開始/停止します。
  void Loop(Component);
  void Exit();
  Closure ExitLoopClosure();

  // ループによって実行されるタスクを投稿します。
  void Post(Task task);
  void PostEvent(Event event);
  void RequestAnimationFrame();

  CapturedMouse CaptureMouse();

  // 関数を装飾します。出力された関数は入力された関数と同様に実行されますが、
  // 現在アクティブなスクリーンターミナルフックは一時的にアンインストールされます。
  Closure WithRestoredIO(Closure);

  // FTXUI は Ctrl-C および Ctrl-Z のハンドラを実装しています。デフォルトでは、これらのハンドラは
  // コンポーネントがイベントをキャッチした場合でも実行されます。これにより、ユーザーが
  // すべてのイベントを処理してアプリケーションに閉じ込められることを回避します。ただし、場合によっては
  // アプリケーションがこれらのイベント自体を処理したい場合があります。この場合、
  // アプリケーションは以下の関数を force=true で呼び出すことで、FTXUI がこれらのイベントを処理しないように強制できます。
  void ForceHandleCtrlC(bool force);
  void ForceHandleCtrlZ(bool force);

  // 選択 API。
  std::string GetSelection();
  void SelectionChange(std::function<void()> callback);

 private:
  void ExitNow();

  void Install();
  void Uninstall();

  void PreMain();
  void PostMain();

  bool HasQuitted();
  void RunOnce(Component component);
  void RunOnceBlocking(Component component);

  void HandleTask(Component component, Task& task);
  bool HandleSelection(bool handled, Event event);
  void RefreshSelection();
  void Draw(Component component);
  void ResetCursorPosition();

  void InstallPipedInputHandling();

  void Signal(int signal);

  void FetchTerminalEvents();

  void PostAnimationTask();

  ScreenInteractive* suspended_screen_ = nullptr;
  enum class Dimension {
    FitComponent,
    Fixed,
    Fullscreen,
    TerminalOutput,
  };
  ScreenInteractive(Dimension dimension,
                    int dimx,
                    int dimy,
                    bool use_alternative_screen);

  const Dimension dimension_;
  const bool use_alternative_screen_;

  bool track_mouse_ = true;

  std::string set_cursor_position;
  std::string reset_cursor_position;

  std::atomic<bool> quit_{false};
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

  // パイプ入力処理の状態 (POSIX のみ)
  bool handle_piped_input_ = true;
  // /dev/tty のファイル記述子。パイプ入力処理に使用されます。
  int tty_fd_ = -1;

  // 終了時に復元するカーソルのスタイル。
  int cursor_reset_shape_ = 1;

  // 選択 API:
  CapturedMouse selection_pending_;
  struct SelectionData {
    int start_x = -1;
    int start_y = -1;
    int end_x = -2;
    int end_y = -2;
    bool empty = true;
    bool operator==(const SelectionData& other) const;
    bool operator!=(const SelectionData& other) const;
  };
  SelectionData selection_data_;
  SelectionData selection_data_previous_;
  std::unique_ptr<Selection> selection_;
  std::function<void()> selection_on_change_;

  // PIMPL (Private Implementation) イディオム。
  struct Internal;
  std::unique_ptr<Internal> internal_;

  friend class Loop;

  Component component_;

 public:
  class Private {
   public:
    static void Signal(ScreenInteractive& s, int signal) { s.Signal(signal); }
  };
  friend Private;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_SCREEN_INTERACTIVE_HPP */
