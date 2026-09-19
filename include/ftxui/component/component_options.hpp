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
#include "ftxui/util/export.hpp"   // for FTXUI_EXPORT

namespace ftxui {

/// @brief 來自 |ButtonOption|、|CheckboxOption|、
/// |RadioboxOption|、|MenuEntryOption|、|MenuOption| 的轉換參數。
struct FTXUI_EXPORT(COMPONENT) EntryState {
  std::string label;  ///< 要顯示的標籤。
  bool state;         ///< 按鈕/核取方塊/單選框的狀態
  bool active;        ///< 該項目是否為作用中的項目。
  bool focused;       ///< 該項目是否為使用者所聚焦的項目。
  int index;          ///< 該項目的索引（若適用），否則為 -1。
};

/// @brief 底線效果的選項。
/// @ingroup component
struct FTXUI_EXPORT(COMPONENT) UnderlineOption {
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

/// @brief 關於潛在動畫顏色的選項。
/// @ingroup component
struct FTXUI_EXPORT(COMPONENT) AnimatedColorOption {
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

struct FTXUI_EXPORT(COMPONENT) AnimatedColorsOption {
  AnimatedColorOption background;
  AnimatedColorOption foreground;
};

/// @brief MenuEntry 元件的選項。
/// @ingroup component
struct FTXUI_EXPORT(COMPONENT) MenuEntryOption {
  ConstStringRef label = "MenuEntry";
  std::function<Element(const EntryState& state)> transform;
  AnimatedColorsOption animated_colors;
};

/// @brief Menu 元件的選項。
/// @ingroup component
struct FTXUI_EXPORT(COMPONENT) MenuOption {
  // 標準建構子：
  static MenuOption Horizontal();
  static MenuOption HorizontalAnimated();
  static MenuOption Vertical();
  static MenuOption VerticalAnimated();
  static MenuOption Toggle();

  ConstStringListRef entries;  ///> 項目清單。
  Ref<int> selected = 0;       ///> 選取項目的索引。

  // 樣式：
  UnderlineOption underline;
  MenuEntryOption entries_option;
  Direction direction = Direction::Down;
  std::function<Element()> elements_prefix;
  std::function<Element()> elements_infix;
  std::function<Element()> elements_postfix;

  // 觀察者：
  std::function<void()> on_change;  ///> 當選取項目改變時呼叫。
  std::function<void()> on_enter;   ///> 當使用者按下 Enter 鍵時呼叫。
  Ref<int> focused_entry = 0;
};

/// @brief AnimatedButton 元件的選項。
/// @ingroup component
struct FTXUI_EXPORT(COMPONENT) ButtonOption {
  // 標準建構子：
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

  // 樣式：
  std::function<Element(const EntryState&)> transform;
  AnimatedColorsOption animated_colors;
};

/// @brief 核取方塊元件的選項。
/// @ingroup component
struct FTXUI_EXPORT(COMPONENT) CheckboxOption {
  // 標準建構子：
  static CheckboxOption Simple();

  ConstStringRef label = "Checkbox";

  Ref<bool> checked = false;

  // 樣式：
  std::function<Element(const EntryState&)> transform;

  // 觀察者：
  /// 當使用者改變狀態時呼叫。
  std::function<void()> on_change = [] {};
};

/// @brief 用於定義 Input 元件的樣式。
struct FTXUI_EXPORT(COMPONENT) InputState {
  Element element;
  bool hovered;         ///< 輸入框是否被滑鼠懸停。
  bool focused;         ///< 輸入框是否被使用者聚焦。
  bool is_placeholder;  ///< 輸入框是否為空並顯示
                        ///< 佔位文字。
};

/// @brief Input 元件的選項。
/// @ingroup component
struct FTXUI_EXPORT(COMPONENT) InputOption {
  // 一組預先定義的樣式：

  /// @brief 建立預設輸入樣式：
  static InputOption Default();
  /// @brief 具有高邊距的黑底白字樣式：
  static InputOption Spacious();

  /// 輸入框的內容。
  StringRef content = "";

  /// 輸入框為空時的內容。
  StringRef placeholder = "";

  // 樣式：
  std::function<Element(InputState)> transform;
  Ref<bool> password = false;  ///< 使用 '*' 遮蔽輸入內容。
  Ref<bool> multiline = true;  ///< 輸入框是否可以多行輸入。
  Ref<bool> insert = true;     ///< 插入或覆寫字元模式。

  /// 當內容改變時呼叫。
  std::function<void()> on_change = [] {};
  /// 當使用者按下 Enter 鍵時呼叫。
  std::function<void()> on_enter = [] {};

  // 游標的字元位置：
  Ref<int> cursor_position = 0;
};

/// @brief Radiobox 元件的選項。
/// @ingroup component
struct FTXUI_EXPORT(COMPONENT) RadioboxOption {
  // 標準建構子：
  static RadioboxOption Simple();

  // 內容：
  ConstStringListRef entries;
  Ref<int> selected = 0;

  // 樣式：
  std::function<Element(const EntryState&)> transform;

  // 觀察者：
  /// 當選取項目改變時呼叫。
  std::function<void()> on_change = [] {};
  Ref<int> focused_entry = 0;
};

struct FTXUI_EXPORT(COMPONENT) ResizableSplitOption {
  Component main;
  Component back;
  Ref<Direction> direction = Direction::Left;
  Ref<int> main_size =
      (direction() == Direction::Left || direction() == Direction::Right) ? 20
                                                                          : 10;
  std::function<Element()> separator_func = [] { return ::ftxui::separator(); };

  // main_size 的限制條件：
  Ref<int> min = 0;
  Ref<int> max = std::numeric_limits<int>::max();
};

// @brief `Slider` 元件的選項。
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
  std::function<void()> on_change;  ///> 當 `value` 更新時呼叫。
};

/// @brief 傳遞給 `Window` 元件渲染函式的狀態。
/// @ingroup component
struct FTXUI_EXPORT(COMPONENT) WindowRenderState {
  Element inner;             ///< 包裹在此視窗內的元素。
  const std::string& title;  ///< 視窗的標題。
  bool active = false;       ///< 該視窗是否為作用中的視窗。
  bool drag = false;         ///< 該視窗是否正在被拖曳。
  bool resize = false;       ///< 該視窗是否正在被調整大小。
  bool hover_left = false;   ///< 可調整大小的左側邊是否被懸停。
  bool hover_right = false;  ///< 可調整大小的右側邊是否被懸停。
  bool hover_top = false;    ///< 可調整大小的上側邊是否被懸停。
  bool hover_down = false;   ///< 可調整大小的下側邊是否被懸停。
};

// @brief `Window` 元件的選項。
// @ingroup component
struct FTXUI_EXPORT(COMPONENT) WindowOptions {
  Component inner;            ///< 此視窗所包裹的元件。
  ConstStringRef title = "";  ///< 此視窗所顯示的標題。

  Ref<int> left = 0;     ///< 視窗的左側位置。
  Ref<int> top = 0;      ///< 視窗的上側位置。
  Ref<int> width = 20;   ///< 視窗的寬度。
  Ref<int> height = 10;  ///< 視窗的高度。

  Ref<bool> resize_left = true;   ///< 左側邊是否可調整大小？
  Ref<bool> resize_right = true;  ///< 右側邊是否可調整大小？
  Ref<bool> resize_top = true;    ///< 上側邊是否可調整大小？
  Ref<bool> resize_down = true;   ///< 下側邊是否可調整大小？

  /// 一個選用的函式，用來自訂視窗的外觀：
  std::function<Element(const WindowRenderState&)> render;
};

/// @brief Dropdown 元件的選項。
/// @ingroup component
/// 下拉式選單是開啟/關閉單選方塊的核取方塊。
struct FTXUI_EXPORT(COMPONENT) DropdownOption {
  /// 下拉選單是開啟還是關閉：
  Ref<bool> open = false;
  // 核取方塊的選項：
  CheckboxOption checkbox;
  // 單選框的選項：
  RadioboxOption radiobox;
  // 轉換函式：
  std::function<Element(bool open, Element checkbox, Element radiobox)>
      transform;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_COMPONENT_OPTIONS_HPP */
