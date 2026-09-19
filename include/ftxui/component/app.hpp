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

/// @brief Appはアプリケーションのライフサイクルを管理するクラスです。
/// ターミナルの初期化、メインループの実行、終了時のクリーンアップを
/// 担当します。
///
/// @note このクラスは以前ScreenInteractiveという名前でした。
///
/// @ingroup component
class FTXUI_EXPORT(COMPONENT) App : public Screen {
 public:
  // コンストラクタ:

  /// @brief 固定サイズでAppを作成します。
  /// @param dimx アプリの幅。
  /// @param dimy アプリの高さ。
  static App FixedSize(int dimx, int dimy);

  /// @brief ターミナルのフルサイズを使用してAppを作成します。これは、
  /// ターミナルの内容を乱さないよう、代替スクリーンバッファを使用します。
  /// @note これは`App::FullscreenAlternateScreen()`と同じです
  static App Fullscreen();

  /// @brief ターミナルのフルサイズを使用してAppを作成します。プライマリ
  /// スクリーンバッファが使用されます。つまり、ターミナルがリサイズ
  /// されると、以前の内容がターミナルの内容を乱す可能性があります。
  static App FullscreenPrimaryScreen();

  /// @brief ターミナルのフルサイズを使用してAppを作成します。これは、
  /// ターミナルの内容を乱さないよう、代替スクリーンバッファを使用します。
  static App FullscreenAlternateScreen();

  /// @brief 描画されるコンポーネントに幅と高さが一致するAppを作成します。
  static App FitComponent();

  /// @brief 幅はターミナル出力の幅に一致し、高さは描画される
  /// コンポーネントに一致するAppを作成します。
  static App TerminalOutput();

  // デストラクタ。
  ~App() override;

  App(App&&) noexcept;
  App& operator=(App&&) noexcept;
  App(const App&) = delete;
  App& operator=(const App&) = delete;

  // オプション。Loop() の前に呼び出す必要があります。

  /// @brief マウスが追跡され、イベントが報告されるかどうかを設定します。
  /// @param enable マウスイベントの追跡を有効にするかどうか。
  /// @note マウス追跡はデフォルトで有効です。
  /// @note マウス追跡は、それをサポートするターミナルでのみサポート
  /// されます。
  /// @note これは`App::Loop`を呼び出す前に呼び出す必要があります。
  void TrackMouse(bool enable = true);

  /// @brief パイプ入力の自動処理を有効または無効にします。
  /// 有効にすると、FTXUIはパイプ入力を検出し、キーボード入力のために
  /// /dev/ttyから標準入力をリダイレクトします。これにより、アプリケーションは
  /// インタラクティブなキーボードイベントを受け取りながら、パイプされた
  /// データを読み取ることができます。
  /// @param enable パイプ入力処理を有効にするかどうか。デフォルトはtrueです。
  /// @note これはLoop()の前に呼び出す必要があります。
  /// @note この機能はデフォルトで有効になっています。
  /// @note この機能はPOSIXシステム(Linux/macOS)でのみ利用可能です。
  void HandlePipedInput(bool enable = true);

  /// @brief 現在アクティブなappを返します。存在しない場合はnullptrを返します。
  static App* Active();

  // メインループの開始/停止。

  /// @brief メインループを実行します。
  /// @param component 描画するコンポーネント。
  void Loop(Component component);

  /// @brief メインループを終了します。
  void Exit();

  /// @brief メインループを終了する関数を返します。
  Closure ExitLoopClosure();

  /// @brief 関数をデコレートします。出力された関数は、入力された関数と
  /// 同様に実行されますが、現在アクティブなappのターミナルフックが
  /// 一時的にアンインストールされた状態で実行されます。
  Closure WithRestoredIO(Closure fn);

  /// @brief FTXUIはCtrl-CとCtrl-Zのハンドラを実装しています。デフォルトでは、
  /// これらのハンドラは、コンポーネントがイベントを捕捉したとしても実行されます。
  /// これにより、ユーザーがすべてのイベントを処理してアプリケーションに
  /// 閉じ込められることを回避できます。しかし、場合によってはアプリケーションが
  /// これらのイベントを自分自身で処理したいことがあります。この場合、
  /// アプリケーションは以下の関数をforce=trueで呼び出すことで、FTXUIが
  /// これらのイベントを処理しないように強制できます。
  void ForceHandleCtrlC(bool force = true);

  /// @brief コンポーネントがEvent::CtrlZを捕捉したとしても、FTXUIにCtrl-Zを
  /// 処理させる、または処理させないように強制します。
  void ForceHandleCtrlZ(bool force = true);

  // ループによって実行されるタスクを投稿します。

  /// @brief メインループにタスクを追加します。
  /// 他のすべてのスケジュール済みタスクの後に、後で実行されます。
  void Post(Task task);

  /// @brief メインループにイベントを追加します。
  /// 他のすべてのスケジュール済みイベントの後に、後で実行されます。
  void PostEvent(Event event);

  /// @brief メインループにタスクを追加します。
  /// 他のすべてのスケジュール済みタスクの後に、後で実行されます。
  static void PostEventOrExecute(Closure closure);

  /// @brief すべてのアニメーションが完了するまで、画面をもう一度描画する
  /// タスクを追加します。
  void RequestAnimationFrame();

  // 選択 API:

  /// @brief マウスをキャプチャできることについてのユニークロックの取得を
  /// 試みます。
  /// @return マウスがまだキャプチャされていない場合はユニークロック、
  /// そうでない場合はnullを返します。
  CapturedMouse CaptureMouse();

  /// @brief 現在の選択内容を返します。
  std::string GetSelection();

  /// @brief 選択が変更されたときに呼び出されるコールバックを設定します。
  void SelectionChange(std::function<void()> callback);

  // ターミナル情報。

  /// @brief ターミナル名を返します。
  const std::string& TerminalName() const;

  /// @brief ターミナルのバージョンを返します。
  int TerminalVersion() const;

  /// @brief ターミナルエミュレータ名を返します。
  const std::string& TerminalEmulatorName() const;

  /// @brief ターミナルエミュレータのバージョンを返します。
  const std::string& TerminalEmulatorVersion() const;

  /// @brief ターミナルの能力を返します。
  const std::vector<int>& TerminalCapabilities() const;

  /// @brief ターミナル能力の名前を返します。
  std::vector<std::string> TerminalCapabilityNames() const;

 private:
  void ExitNow();
  void Install();
  void Uninstall();

  void PreMain();
  void PostMain();

  /// @brief メインループが終了したかどうかを返します。
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
