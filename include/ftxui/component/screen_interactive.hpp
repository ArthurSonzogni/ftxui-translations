// 版权所有 2020 Arthur Sonzogni. 保留所有权利。
// 本源代码的使用受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
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

/// @brief ScreenInteractive 是一个可以处理事件、运行主循环和管理组件的 `Screen`。
///
/// @ingroup component
class ScreenInteractive : public Screen {
 public:
  // 构造函数:
  static ScreenInteractive FixedSize(int dimx, int dimy);
  static ScreenInteractive Fullscreen();
  static ScreenInteractive FullscreenPrimaryScreen();
  static ScreenInteractive FullscreenAlternateScreen();
  static ScreenInteractive FitComponent();
  static ScreenInteractive TerminalOutput();

  // 析构函数。
  ~ScreenInteractive() override;

  // 选项。必须在 Loop() 之前调用。
  void TrackMouse(bool enable = true);
  void HandlePipedInput(bool enable = true);

  // 返回当前活动屏幕，如果没有则为 nullptr。
  static ScreenInteractive* Active();

  // 启动/停止主循环。
  void Loop(Component);
  void Exit();
  Closure ExitLoopClosure();

  // 发布要由循环执行的任务。
  void Post(Task task);
  void PostEvent(Event event);
  void RequestAnimationFrame();

  CapturedMouse CaptureMouse();

  // 装饰一个函数。输出的函数将与输入的函数类似地执行，但会暂时卸载当前活动屏幕的终端钩子。
  Closure WithRestoredIO(Closure);

  // FTXUI 实现了 Ctrl-C 和 Ctrl-Z 的处理程序。默认情况下，即使组件捕获了事件，这些处理程序也会被执行。这避免了用户在处理每个事件时被困在应用程序中。但是，在某些情况下，应用程序可能希望自己处理这些事件。在这种情况下，应用程序可以通过调用以下函数并设置 force=true 来强制 FTXUI 不处理这些事件。
  void ForceHandleCtrlC(bool force);
  void ForceHandleCtrlZ(bool force);

  // 选择 API。
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

  // 管道输入处理状态（仅限 POSIX）
  bool handle_piped_input_ = true;
  // /dev/tty 的文件描述符，用于管道输入处理。
  int tty_fd_ = -1;

  // 退出时恢复光标的样式。
  int cursor_reset_shape_ = 1;

  // 选择 API:
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

  // PIMPL 私有实现惯用法 (Pimpl)。
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

#endif /* 头文件保护结束: FTXUI_COMPONENT_SCREEN_INTERACTIVE_HPP */
