// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_COMPONENT_APP_HPP
#define FTXUI_COMPONENT_APP_HPP

#include <atomic>      // for atomic
#include <chrono>      // for steady_clock, time_point
#include <functional>  // for function
#include <memory>      // for shared_ptr, unique_ptr
#include <string>      // for string, basic_string, allocator
#include <vector>      // for vector

#include "ftxui/component/animation.hpp"  // for TimePoint
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/task.hpp"   // for Task, Closure
#include "ftxui/screen/screen.hpp"    // for Screen
#include "ftxui/screen/terminal.hpp"  // for Dimensions
#include "ftxui/util/export.hpp"

namespace ftxui {
class ComponentBase;
using Component = std::shared_ptr<ComponentBase>;
struct Event;
class Selection;
class TaskRunner;

/// @brief App 是一個管理應用程式生命週期的類別。
/// 它負責初始化終端機、執行主迴圈，
/// 並在結束時清理。
///
/// @note 這個類別先前名為 ScreenInteractive。
///
/// @ingroup component
class FTXUI_EXPORT(COMPONENT) App : public Screen {
 public:
  // 建構子：

  /// @brief 建立一個固定大小的 App。
  /// @param dimx App 的寬度。
  /// @param dimy App 的高度。
  static App FixedSize(int dimx, int dimy);

  /// @brief 建立一個佔滿整個終端機大小的 App。這會使用
  /// 替代畫面緩衝區（alternate screen buffer），以避免弄亂終端機的內容。
  /// @note 這與 `App::FullscreenAlternateScreen()` 相同
  static App Fullscreen();

  /// @brief 建立一個佔滿整個終端機大小的 App。使用的是主要畫面
  /// 緩衝區。這代表如果終端機被調整大小，先前的
  /// 內容可能會與終端機內容混雜在一起。
  static App FullscreenPrimaryScreen();

  /// @brief 建立一個佔滿整個終端機大小的 App。這會使用
  /// 替代畫面緩衝區，以避免破壞終端機的內容。
  static App FullscreenAlternateScreen();

  /// @brief 建立一個寬度和高度與正在繪製的元件相符的 App。
  static App FitComponent();

  /// @brief 建立一個寬度符合終端機輸出寬度，
  /// 且高度符合正在繪製的元件的 App。
  static App TerminalOutput();

  // 解構函式。
  ~App() override;

  App(App&&) noexcept;
  App& operator=(App&&) noexcept;
  App(const App&) = delete;
  App& operator=(const App&) = delete;

  // 選項。必須在 Loop() 之前呼叫。

  /// @brief 設定是否追蹤滑鼠並回報事件。
  /// @param enable 是否啟用滑鼠事件追蹤。
  /// @note 滑鼠追蹤預設為啟用。
  /// @note 滑鼠追蹤僅在支援它的終端機上受支援。
  /// @note 必須在呼叫 `App::Loop` 之前呼叫此函式。
  void TrackMouse(bool enable = true);

  /// @brief 啟用或停用自動管道輸入處理。
  /// 啟用時，FTXUI 會偵測管道輸入，並將標準輸入從
  /// /dev/tty 重新導向以取得鍵盤輸入，讓應用程式在讀取管道資料的
  /// 同時仍能接收互動式鍵盤事件。
  /// @param enable 是否啟用管道輸入處理。預設為 true。
  /// @note 必須在 Loop() 之前呼叫。
  /// @note 此功能預設為啟用。
  /// @note 此功能僅在 POSIX 系統（Linux/macOS）上可用。
  void HandlePipedInput(bool enable = true);

  /// @brief 回傳目前作用中的 app，若無則回傳 nullptr。
  static App* Active();

  // 開始/停止主迴圈。

  /// @brief 執行主迴圈。
  /// @param component 要繪製的元件。
  void Loop(Component component);

  /// @brief 結束主迴圈。
  void Exit();

  /// @brief 回傳一個用來結束主迴圈的函式。
  Closure ExitLoopClosure();

  /// @brief 裝飾一個函式。輸出的函式執行方式會與輸入的函式類似，
  /// 但目前作用中的 app 終端機掛勾會暫時被卸除。
  Closure WithRestoredIO(Closure fn);

  /// @brief FTXUI 實作了 Ctrl-C 和 Ctrl-Z 的處理器。預設情況下，
  /// 即使元件捕捉到該事件，這些處理器仍會被執行。這避免使用者
  /// 必須處理每個事件才能跳脫應用程式。然而，在某些情況下，
  /// 應用程式可能想要自行處理這些事件。在這種情況下，
  /// 應用程式可以透過呼叫下列函式並傳入 force=true，
  /// 強制 FTXUI 不處理這些事件。
  void ForceHandleCtrlC(bool force = true);

  /// @brief 強制 FTXUI 處理或不處理 Ctrl-Z，即使元件
  /// 捕捉到了 Event::CtrlZ。
  void ForceHandleCtrlZ(bool force = true);

  // 將任務發布給迴圈執行。

  /// @brief 新增一個任務到主迴圈。
  /// 它會在稍後、所有其他已排程任務之後執行。
  void Post(Task task);

  /// @brief 新增一個事件到主迴圈。
  /// 它會在稍後、所有其他已排程事件之後執行。
  void PostEvent(Event event);

  /// @brief 新增一個任務到主迴圈。
  /// 它會在稍後、所有其他已排程任務之後執行。
  static void PostEventOrExecute(Closure closure);

  /// @brief 新增一個任務，在所有動畫完成之前，
  /// 多繪製畫面一次。
  void RequestAnimationFrame();

  // 選取 API:

  /// @brief 嘗試取得能夠捕捉滑鼠的唯一鎖。
  /// @return 若滑鼠尚未被捕捉，回傳一個唯一鎖，否則回傳
  /// null。
  CapturedMouse CaptureMouse();

  /// @brief 回傳目前選取內容。
  std::string GetSelection();

  /// @brief 設定一個當選取內容改變時會被呼叫的回呼函式。
  void SelectionChange(std::function<void()> callback);

  // 終端機資訊。

  /// @brief 回傳終端機名稱。
  const std::string& TerminalName() const;

  /// @brief 回傳終端機版本。
  int TerminalVersion() const;

  /// @brief 回傳終端機模擬器名稱。
  const std::string& TerminalEmulatorName() const;

  /// @brief 回傳終端機模擬器版本。
  const std::string& TerminalEmulatorVersion() const;

  /// @brief 回傳終端機能力。
  const std::vector<int>& TerminalCapabilities() const;

  /// @brief 回傳終端機能力的名稱。
  std::vector<std::string> TerminalCapabilityNames() const;

 private:
  void ExitNow();
  void Install();
  void Uninstall();

  void PreMain();
  void PostMain();

  /// @brief 回傳主迴圈是否已結束。
  bool HasQuitted();
  void RunOnce(const Component& component);
  void RunOnceBlocking(Component component);

  void HandleTask(Component component, Task& task);
  bool HandleSelection(bool handled, Event event);
  void Draw(Component component);
  std::string ResetCursorPosition();

  void RequestCursorPosition(bool force = false);

  void TerminalSend(std::string_view);
  void TerminalFlush();

  void InstallPipedInputHandling();
  void InstallTerminalInfo();

  void Signal(int signal);

  size_t FetchTerminalEvents();

  void PostAnimationTask();

  struct Internal;
  explicit App(std::unique_ptr<Internal> internal, int dimx, int dimy);

  std::unique_ptr<Internal> internal_;

  friend class Loop;

 public:
  class Private {
   public:
    static void Signal(App& s, int signal) { s.Signal(signal); }
  };
  friend Private;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_APP_HPP */
