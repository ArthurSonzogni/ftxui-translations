// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_COMPONENT_COMPONENT_OPTIONS_HPP
#define FTXUI_COMPONENT_COMPONENT_OPTIONS_HPP

#include <chrono>                         // for milliseconds
#include <ftxui/component/animation.hpp>  // for Duration, QuadraticInOut, Function
#include <ftxui/dom/direction.hpp>  // for Direction, Direction::Left, Direction::Right, Direction::Down
#include <ftxui/dom/elements.hpp>  // for Element, separator
#include <ftxui/util/ref.hpp>      // for Ref, ConstRef, StringRef
#include <ftxui/util/warn_windows_macro.hpp>
#include <functional>  // for function
#include <limits>      // for numeric_limits
#include <string>      // for string

#include "ftxui/component/component_base.hpp"  // for Component
#include "ftxui/screen/color.hpp"  // for Color, Color::GrayDark, Color::White

namespace ftxui {

/// @brief |ButtonOption|、|CheckboxOption|、|RadioboxOption|、|MenuEntryOption|、|MenuOption|からの変換の引数。
struct EntryState {
  std::string label;  ///< 表示するラベル。
  bool state;         ///< ボタン/チェックボックス/ラジオボックスの状態。
  bool active;        ///< エントリがアクティブであるかどうか。
  bool focused;       ///< エントリがユーザーによってフォーカスされているかどうか。
  int index;          ///< 該当する場合のエントリのインデックス、または-1。
};

/// @brief 下線効果のオプション。
/// @ingroup component
struct UnderlineOption {
  bool enabled = false;

  Color color_active = Color::White;
  Color color_inactive = Color::GrayDark;

  animation::easing::Function leader_function =
      animation::easing::QuadraticInOut;
  animation::easing::Function follower_function =
      animation::easing::QuadraticInOut;

  animation::Duration leader_duration = std::chrono::milliseconds(250);
  animation::Duration leader_delay = std::chrono::milliseconds(0);
  animation::Duration follower_duration = std::chrono::milliseconds(250);
  animation::Duration follower_delay = std::chrono::milliseconds(0);

  void SetAnimation(animation::Duration d, animation::easing::Function f);
  void SetAnimationDuration(animation::Duration d);
  void SetAnimationFunction(animation::easing::Function f);
  void SetAnimationFunction(animation::easing::Function f_leader,
                            animation::easing::Function f_follower);
};

/// @brief アニメーション化された可能性のある色に関するオプション。
/// @ingroup component
struct AnimatedColorOption {
  void Set(
      Color inactive,
      Color active,
      animation::Duration duration = std::chrono::milliseconds(250),
      animation::easing::Function function = animation::easing::QuadraticInOut);

  bool enabled = false;
  Color inactive;
  Color active;
  animation::Duration duration = std::chrono::milliseconds(250);
  animation::easing::Function function = animation::easing::QuadraticInOut;
};

struct AnimatedColorsOption {
  AnimatedColorOption background;
  AnimatedColorOption foreground;
};

/// @brief MenuEntryコンポーネントのオプション。
/// @ingroup component
struct MenuEntryOption {
  ConstStringRef label = "MenuEntry";
  std::function<Element(const EntryState& state)> transform;
  AnimatedColorsOption animated_colors;
};

/// @brief Menuコンポーネントのオプション。
/// @ingroup component
struct MenuOption {
  // 標準コンストラクタ:
  static MenuOption Horizontal();
  static MenuOption HorizontalAnimated();
  static MenuOption Vertical();
  static MenuOption VerticalAnimated();
  static MenuOption Toggle();

  ConstStringListRef entries;  ///> エントリのリスト。
  Ref<int> selected = 0;       ///> 選択されたエントリのインデックス。

  // スタイル:
  UnderlineOption underline;
  MenuEntryOption entries_option;
  Direction direction = Direction::Down;
  std::function<Element()> elements_prefix;
  std::function<Element()> elements_infix;
  std::function<Element()> elements_postfix;

  // オブザーバー:
  std::function<void()> on_change;  ///> 選択されたエントリが変更されたときに呼び出されます。
  std::function<void()> on_enter;   ///> ユーザーがEnterを押したときに呼び出されます。
  Ref<int> focused_entry = 0;
};

/// @brief AnimatedButtonコンポーネントのオプション。
/// @ingroup component
struct ButtonOption {
  // 標準コンストラクタ:
  static ButtonOption Ascii();
  static ButtonOption Simple();
  static ButtonOption Border();
  static ButtonOption Animated();
  static ButtonOption Animated(Color color);
  static ButtonOption Animated(Color background, Color foreground);
  static ButtonOption Animated(Color background,
                               Color foreground,
                               Color background_active,
                               Color foreground_active);

  ConstStringRef label = "Button";
  std::function<void()> on_click = [] {};

  // スタイル:
  std::function<Element(const EntryState&)> transform;
  AnimatedColorsOption animated_colors;
};

/// @brief Checkboxコンポーネントのオプション。
/// @ingroup component
struct CheckboxOption {
  // 標準コンストラクタ:
  static CheckboxOption Simple();

  ConstStringRef label = "Checkbox";

  Ref<bool> checked = false;

  // スタイル:
  std::function<Element(const EntryState&)> transform;

  // オブザーバー:
  /// ユーザーが状態を変更したときに呼び出されます。
  std::function<void()> on_change = [] {};
};

/// @brief Inputコンポーネントのスタイルを定義するために使用されます。
struct InputState {
  Element element;
  bool hovered;         ///< 入力がマウスによってホバーされているかどうか。
  bool focused;         ///< 入力がユーザーによってフォーカスされているかどうか。
  bool is_placeholder;  ///< 入力が空でプレースホルダーを表示しているかどうか。
};

/// @brief Inputコンポーネントのオプション。
/// @ingroup component
struct InputOption {
  // 事前定義されたスタイルセット:

  /// @brief デフォルトの入力スタイルを作成します:
  static InputOption Default();
  /// @brief 余白の広い、白地に黒のスタイル:
  static InputOption Spacious();

  /// 入力の内容。
  StringRef content = "";

  /// 入力が空の場合の入力内容。
  StringRef placeholder = "";

  // スタイル:
  std::function<Element(InputState)> transform;
  Ref<bool> password = false;  ///< 入力内容を'*'で隠します。
  Ref<bool> multiline = true;  ///< 入力を複数行にできるかどうか。
  Ref<bool> insert = true;     ///< 文字の挿入または上書きモード。

  /// 内容が変更されたときに呼び出されます。
  std::function<void()> on_change = [] {};
  /// ユーザーがEnterを押したときに呼び出されます。
  std::function<void()> on_enter = [] {};

  // カーソルの文字位置:
  Ref<int> cursor_position = 0;
};

/// @brief Radioboxコンポーネントのオプション。
/// @ingroup component
struct RadioboxOption {
  // 標準コンストラクタ:
  static RadioboxOption Simple();

  // コンテンツ:
  ConstStringListRef entries;
  Ref<int> selected = 0;

  // スタイル:
  std::function<Element(const EntryState&)> transform;

  // オブザーバー:
  /// 選択されたエントリが変更されたときに呼び出されます。
  std::function<void()> on_change = [] {};
  Ref<int> focused_entry = 0;
};

struct ResizableSplitOption {
  Component main;
  Component back;
  Ref<Direction> direction = Direction::Left;
  Ref<int> main_size =
      (direction() == Direction::Left || direction() == Direction::Right) ? 20
                                                                          : 10;
  std::function<Element()> separator_func = [] { return ::ftxui::separator(); };

  // main_sizeの制約:
  Ref<int> min = 0;
  Ref<int> max = std::numeric_limits<int>::max();
};

// @brief `Slider`コンポーネントのオプション。
// @ingroup component
template <typename T>
struct SliderOption {
  Ref<T> value;
  ConstRef<T> min = T(0);
  ConstRef<T> max = T(100);
  ConstRef<T> increment = (max() - min()) / 20;
  Direction direction = Direction::Right;
  Color color_active = Color::White;
  Color color_inactive = Color::GrayDark;
  std::function<void()> on_change;  ///> `value`が更新されたときに呼び出されます。
};

/// @brief `Window`コンポーネントのレンダー関数に渡される状態。
/// @ingroup component
struct WindowRenderState {
  Element inner;             ///< このウィンドウ内にラップされた要素。
  const std::string& title;  ///< ウィンドウのタイトル。
  bool active = false;       ///< ウィンドウがアクティブであるかどうか。
  bool drag = false;         ///< ウィンドウがドラッグされているかどうか。
  bool resize = false;       ///< ウィンドウがサイズ変更されているかどうか。
  bool hover_left = false;   ///< サイズ変更可能な左側がホバーされているかどうか。
  bool hover_right = false;  ///< サイズ変更可能な右側がホバーされているかどうか。
  bool hover_top = false;    ///< サイズ変更可能な上側がホバーされているかどうか。
  bool hover_down = false;   ///< サイズ変更可能な下側がホバーされているかどうか。
};

// @brief `Window`コンポーネントのオプション。
// @ingroup component
struct WindowOptions {
  Component inner;            ///< このウィンドウでラップされたコンポーネント。
  ConstStringRef title = "";  ///< このウィンドウによって表示されるタイトル。

  Ref<int> left = 0;     ///< ウィンドウの左側の位置。
  Ref<int> top = 0;      ///< ウィンドウの上側の位置。
  Ref<int> width = 20;   ///< ウィンドウの幅。
  Ref<int> height = 10;  ///< ウィンドウの高さ。

  Ref<bool> resize_left = true;   ///< 左側をサイズ変更できるか？
  Ref<bool> resize_right = true;  ///< 右側をサイズ変更できるか？
  Ref<bool> resize_top = true;    ///< 上側をサイズ変更できるか？
  Ref<bool> resize_down = true;   ///< 下側をサイズ変更できるか？

  /// ウィンドウの外観をカスタマイズするためのオプション関数:
  std::function<Element(const WindowRenderState&)> render;
};

/// @brief ドロップダウンコンポーネントのオプション。
/// @ingroup component
/// ドロップダウンメニューは、ラジオボックスを開閉するチェックボックスです。
struct DropdownOption {
  /// ドロップダウンが開いているか閉じているか:
  Ref<bool> open = false;
  // チェックボックスのオプション:
  CheckboxOption checkbox;
  // ラジオボックスのオプション:
  RadioboxOption radiobox;
  // 変換関数:
  std::function<Element(bool open, Element checkbox, Element radiobox)>
      transform;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_COMPONENT_OPTIONS_HPP */
