// 版权所有 2021 Arthur Sonzogni。保留所有权利。
// 本源代码的使用受 MIT 许可协议的约束，该协议可在 LICENSE 文件中找到。
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

/// @brief 来自 |ButtonOption|、|CheckboxOption|、
/// |RadioboxOption|、|MenuEntryOption|、|MenuOption| 的转换参数。
struct EntryState {
  std::string label;  ///< 要显示的标签。
  bool state;         ///< 按钮/复选框/单选框的状态
  bool active;        ///< 条目是否为活动状态。
  bool focused;       ///< 条目是否被用户聚焦。
  int index;          ///< 适用时为条目的索引，否则为 -1。
};

/// @brief 下划线效果的选项。
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

/// @brief 可能是动画颜色的选项。
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

/// @brief MenuEntry 组件的选项。
/// @ingroup component
struct MenuEntryOption {
  ConstStringRef label = "MenuEntry";
  std::function<Element(const EntryState& state)> transform;
  AnimatedColorsOption animated_colors;
};

/// @brief Menu 组件的选项。
/// @ingroup component
struct MenuOption {
  // 标准构造函数:
  static MenuOption Horizontal();
  static MenuOption HorizontalAnimated();
  static MenuOption Vertical();
  static MenuOption VerticalAnimated();
  static MenuOption Toggle();

  ConstStringListRef entries;  ///> 条目列表。
  Ref<int> selected = 0;       ///> 选中条目的索引。

  // 样式:
  UnderlineOption underline;
  MenuEntryOption entries_option;
  Direction direction = Direction::Down;
  std::function<Element()> elements_prefix;
  std::function<Element()> elements_infix;
  std::function<Element()> elements_postfix;

  // 观察者:
  std::function<void()> on_change;  ///> 当选中条目改变时调用。
  std::function<void()> on_enter;   ///> 当用户按下回车键时调用。
  Ref<int> focused_entry = 0;
};

/// @brief AnimatedButton 组件的选项。
/// @ingroup component
struct ButtonOption {
  // Standard constructors:
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

  // Style:
  std::function<Element(const EntryState&)> transform;
  AnimatedColorsOption animated_colors;
};

/// @brief Checkbox 组件的选项。
/// @ingroup component
struct CheckboxOption {
  // 标准构造函数:
  static CheckboxOption Simple();

  ConstStringRef label = "Checkbox";

  Ref<bool> checked = false;

  // 样式:
  std::function<Element(const EntryState&)> transform;

  // 观察者:
  /// 当用户改变状态时调用。
  std::function<void()> on_change = [] {};
};

/// @brief 用于定义 Input 组件的样式。
struct InputState {
  Element element;
  bool hovered;         ///< 输入框是否被鼠标悬停。
  bool focused;         ///< 输入框是否被用户聚焦。
  bool is_placeholder;  ///< 输入框是否为空并显示占位符。
};

/// @brief Input 组件的选项。
/// @ingroup component
struct InputOption {
  // 一组预定义样式:

  /// @brief 创建默认输入样式:
  static InputOption Default();
  /// @brief 带有大边距的白底黑字样式:
  static InputOption Spacious();

  /// 输入框的内容。
  StringRef content = "";

  /// 当输入框为空时显示的内容。
  StringRef placeholder = "";

  // 样式:
  std::function<Element(InputState)> transform;
  Ref<bool> password = false;  ///< 使用 '*' 遮盖输入内容。
  Ref<bool> multiline = true;  ///< 输入框是否可以多行。
  Ref<bool> insert = true;     ///< 插入或覆盖字符模式。

  /// 当内容改变时调用。
  std::function<void()> on_change = [] {};
  /// 当用户按下回车键时调用。
  std::function<void()> on_enter = [] {};

  // 光标的字符位置:
  Ref<int> cursor_position = 0;
};

/// @brief Radiobox 组件的选项。
/// @ingroup component
struct RadioboxOption {
  // 标准构造函数:
  static RadioboxOption Simple();

  // 内容:
  ConstStringListRef entries;
  Ref<int> selected = 0;

  // 样式:
  std::function<Element(const EntryState&)> transform;

  // 观察者:
  /// 当选中条目改变时调用。
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

  // Constraints on main_size:
  Ref<int> min = 0;
  Ref<int> max = std::numeric_limits<int>::max();
};

// @brief `Slider` 组件的选项。
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
  std::function<void()> on_change;  ///> 当 `value` 更新时调用。
};

/// @brief 传递给 `Window` 组件渲染函数的 State。
/// @ingroup component
struct WindowRenderState {
  Element inner;             ///< 包装在此窗口内的元素。
  const std::string& title;  ///< 窗口的标题。
  bool active = false;       ///< 窗口是否为活动状态。
  bool drag = false;         ///< 窗口是否正在被拖动。
  bool resize = false;       ///< 窗口是否正在被调整大小。
  bool hover_left = false;   ///< 可调整大小的左侧是否被悬停。
  bool hover_right = false;  ///< 可调整大小的右侧是否被悬停。
  bool hover_top = false;    ///< 可调整大小的顶部是否被悬停。
  bool hover_down = false;   ///< 可调整大小的底部是否被悬停。
};

// @brief `Window` 组件的选项。
// @ingroup component
struct WindowOptions {
  Component inner;            ///< 此窗口包装的组件。
  ConstStringRef title = "";  ///< 此窗口显示的标题。

  Ref<int> left = 0;     ///< 窗口的左侧位置。
  Ref<int> top = 0;      ///< 窗口的顶部位置。
  Ref<int> width = 20;   ///< 窗口的宽度。
  Ref<int> height = 10;  ///< 窗口的高度。

  Ref<bool> resize_left = true;   ///< 左侧是否可以调整大小？
  Ref<bool> resize_right = true;  ///< 右侧是否可以调整大小？
  Ref<bool> resize_top = true;    ///< 顶部是否可以调整大小？
  Ref<bool> resize_down = true;   ///< 底部是否可以调整大小？

  /// 用于自定义窗口外观的可选函数:
  std::function<Element(const WindowRenderState&)> render;
};

/// @brief Dropdown 组件的选项。
/// @ingroup component
/// 下拉菜单是打开/关闭单选框的复选框。
struct DropdownOption {
  /// 下拉菜单是打开还是关闭:
  Ref<bool> open = false;
  // 复选框的选项:
  CheckboxOption checkbox;
  // 单选框的选项:
  RadioboxOption radiobox;
  // 转换函数:
  std::function<Element(bool open, Element checkbox, Element radiobox)>
      transform;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_COMPONENT_OPTIONS_HPP */
