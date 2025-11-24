// 版權所有 2020 Arthur Sonzogni。保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，該條款可在 LICENSE 檔案中找到。
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

/// @brief ScreenInteractive 是一個可以處理事件、執行主迴圈並管理組件的 `Screen`。
///
/// @ingroup component
class ScreenInteractive : public Screen {
 public:
  // 建構函式:
  static ScreenInteractive FixedSize(int dimx, int dimy);
  static ScreenInteractive Fullscreen();
  static ScreenInteractive FullscreenPrimaryScreen();
  static ScreenInteractive FullscreenAlternateScreen();
  static ScreenInteractive FitComponent();
  static ScreenInteractive TerminalOutput();

  // 解構函式。
  ~ScreenInteractive() override;

  // 選項。必須在 Loop() 之前呼叫。
  void TrackMouse(bool enable = true);
  void HandlePipedInput(bool enable = true);

  // 返回當前活動的螢幕，如果沒有則返回 nullptr。
  static ScreenInteractive* Active();

  // 啟動/停止主迴圈。
  void Loop(Component);
  void Exit();
  Closure ExitLoopClosure();

  // 發布要由迴圈執行的任務。
  void Post(Task task);
  void PostEvent(Event event);
  void RequestAnimationFrame();

  CapturedMouse CaptureMouse();

  // 裝飾一個函式。輸出函式的執行方式與輸入函式類似，但會暫時卸載當前活動螢幕的終端機掛鉤。
  Closure WithRestoredIO(Closure);

  // FTXUI 實作了 Ctrl-C 和 Ctrl-Z 的處理程式。預設情況下，即使組件捕捉到事件，這些處理程式也會執行。這避免了使用者處理每個事件時被應用程式困住。然而，在某些情況下，應用程式可能希望自行處理這些事件。在這種情況下，應用程式可以透過呼叫以下函式並將 force=true 傳遞來強制 FTXUI 不處理這些事件。
  void ForceHandleCtrlC(bool force);
  void ForceHandleCtrlZ(bool force);

  // 選取 API。
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

  // 管道輸入處理狀態（僅限 POSIX）
  bool handle_piped_input_ = true;
  // /dev/tty 的檔案描述符，用於管道輸入處理。
  int tty_fd_ = -1;

  // 退出時要恢復的游標樣式。
  int cursor_reset_shape_ = 1;

  // 選取 API:
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

  // PIMPL 私有實作慣用語 (Pimpl)。
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

#endif /* 包含防護結束: FTXUI_COMPONENT_SCREEN_INTERACTIVE_HPP */
