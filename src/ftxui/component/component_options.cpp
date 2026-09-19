// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/component/component_options.hpp"

#include <ftxui/screen/color.hpp>  // for Color, Color::White, Color::Black, Color::GrayDark, Color::Blue, Color::GrayLight, Color::Red
#include <ftxui/screen/terminal.hpp>
#include <memory>  // for shared_ptr
#include <string>
#include <utility>                        // for move
#include "ftxui/component/animation.hpp"  // for Function, Duration
#include "ftxui/dom/direction.hpp"
#include "ftxui/dom/elements.hpp"  // for operator|=, Element, text, bgcolor, inverted, bold, dim, operator|, color, borderEmpty, hbox, automerge, border, borderLight

namespace ftxui {

/// @brief A color option that can be animated.
/// @param _inactive The color when the component is inactive.
/// @param _active The color when the component is active.
/// @param _duration The duration of the animation.
/// @param _function The easing function of the animation.
void AnimatedColorOption::Set(Color _inactive,
                              Color _active,
                              animation::Duration _duration,
                              animation::easing::Function _function) {
  enabled = true;
  inactive = _inactive;
  active = _active;
  duration = _duration;
  function = std::move(_function);
}

/// @brief 設定下劃線的動畫方式。
/// @param d 動畫的持續時間。
/// @param f 動畫的緩動函數。
void UnderlineOption::SetAnimation(animation::Duration d,
                                   animation::easing::Function f) {
  SetAnimationDuration(d);
  SetAnimationFunction(std::move(f));
}

/// @brief 設定下劃線的動畫方式。
/// @param d 動畫的持續時間。
void UnderlineOption::SetAnimationDuration(animation::Duration d) {
  leader_duration = d;
  follower_duration = d;
}

/// @brief 設定下劃線的動畫方式。
/// @param f 動畫的緩動函數。
void UnderlineOption::SetAnimationFunction(animation::easing::Function f) {
  leader_function = f;
  follower_function = std::move(f);
}

/// @brief 設定下劃線的動畫方式。
/// 這對於使引導者和追隨者的動畫不同步很有用。
/// @param f_leader 引導者動畫的持續時間。
/// @param f_follower 追隨者動畫的持續時間。
void UnderlineOption::SetAnimationFunction(
    animation::easing::Function f_leader,
    animation::easing::Function f_follower) {
  leader_function = std::move(f_leader);
  follower_function = std::move(f_follower);
}

/// @brief 水平選單的標準選項。
/// 這對於實現一個標籤頁列很有用。
// static
MenuOption MenuOption::Horizontal() {
  MenuOption option;
  option.direction = Direction::Right;
  option.entries_option.transform = [](const EntryState& state) {
    Element e = text(state.label);
    if (state.focused) {
      e |= inverted;
    }
    if (state.active) {
      e |= bold;
    }
    if (!state.focused && !state.active) {
      e |= dim;
    }
    return e;
  };
  option.elements_infix = [] { return text(" "); };

  return option;
}

/// @brief 動畫水平選單的標準選項。
/// 這對於實現一個標籤頁列很有用。
// static
MenuOption MenuOption::HorizontalAnimated() {
  auto option = Horizontal();
  option.underline.enabled = true;
  return option;
}

/// @brief 垂直選單的標準選項。
/// 這對於實現一個可選項目列表很有用。
// static
MenuOption MenuOption::Vertical() {
  MenuOption option;
  option.entries_option.transform = [](const EntryState& state) {
    Element e = text((state.active ? "> " : "  ") + state.label);  // NOLINT
    if (state.focused) {
      e |= inverted;
    }
    if (state.active) {
      e |= bold;
    }
    if (!state.focused && !state.active) {
      e |= dim;
    }
    return e;
  };
  return option;
}

/// @brief 動畫垂直選單的標準選項。
/// 這對於實現一個可選項目列表很有用。
// static
MenuOption MenuOption::VerticalAnimated() {
  auto option = MenuOption::Vertical();
  option.entries_option.transform = [](const EntryState& state) {
    Element e = text(state.label);
    if (state.focused) {
      e |= inverted;
    }
    if (state.active) {
      e |= bold;
    }
    if (!state.focused && !state.active) {
      e |= dim;
    }
    return e;
  };
  option.underline.enabled = true;
  return option;
}

/// @brief 帶有分隔線的水平選單標準選項。
/// 這對於實現一個標籤頁列很有用。
// static
MenuOption MenuOption::Toggle() {
  auto option = MenuOption::Horizontal();
  option.elements_infix = [] { return text("│") | automerge; };
  return option;
}

/// @brief 創建一個 ButtonOption，使用 [] 字元突出顯示。
// static
ButtonOption ButtonOption::Ascii() {
  ButtonOption option;
  option.transform = [](const EntryState& s) {
    const std::string t = s.focused ? "[" + s.label + "]"  //
                                    : " " + s.label + " ";
    return text(t);
  };
  return option;
}

/// @brief 創建一個 ButtonOption，在聚焦時反轉。
// static
ButtonOption ButtonOption::Simple() {
  ButtonOption option;
  option.transform = [](const EntryState& s) {
    auto element = text(s.label) | borderLight;
    if (s.focused) {
      element |= inverted;
    }
    return element;
  };
  return option;
}

/// @brief 創建一個 ButtonOption。按鈕使用邊框顯示，聚焦時反轉。
/// 這是目前的預設值。
ButtonOption ButtonOption::Border() {
  ButtonOption option;
  option.transform = [](const EntryState& s) {
    auto element = text(s.label) | border;
    if (s.active) {
      element |= bold;
    }
    if (s.focused) {
      element |= inverted;
    }
    return element;
  };
  return option;
}

/// @brief 創建一個 ButtonOption，使用動畫顏色。
// static
ButtonOption ButtonOption::Animated() {
  return Animated(Color::Black, Color::GrayLight,  //
                  Color::GrayDark, Color::White);
}

/// @brief 創建一個 ButtonOption，使用動畫顏色。
// static
ButtonOption ButtonOption::Animated(Color color) {
  return ButtonOption::Animated(
      Color::Interpolate(0.85F, color, Color::Black),   // NOLINT
      Color::Interpolate(0.10F, color, Color::White),   // NOLINT
      Color::Interpolate(0.10F, color, Color::Black),   // NOLINT
      Color::Interpolate(0.85F, color, Color::White));  // NOLINT
}

/// @brief 創建一個 ButtonOption，使用動畫顏色。
// static
ButtonOption ButtonOption::Animated(Color background, Color foreground) {
  // NOLINTBEGIN
  return ButtonOption::Animated(
      /*background=*/background,
      /*foreground=*/foreground,
      /*background_active=*/foreground,
      /*foreground_active=*/background);
  // NOLINTEND
}

/// @brief 創建一個 ButtonOption，使用動畫顏色。
// static
ButtonOption ButtonOption::Animated(Color background,
                                    Color foreground,
                                    Color background_active,
                                    Color foreground_active) {
  ButtonOption option;
  option.transform = [](const EntryState& s) {
    auto element = text(s.label) | borderEmpty;
    if (s.focused) {
      element |= bold;
    }
    return element;
  };
  option.animated_colors.foreground.Set(foreground, foreground_active);
  option.animated_colors.background.Set(background, background_active);
  return option;
}

/// @brief 標準Checkbox的選項。
// static
CheckboxOption CheckboxOption::Simple() {
  auto option = CheckboxOption();
  option.transform = [](const EntryState& s) {
    auto prefix = (Terminal::GetQuirks().ComponentAscii())
                      ? text(s.state ? "[X] " : "[ ] ")  // NOLINT
                      : text(s.state ? "▣ " : "☐ ");     // NOLINT
    auto t = text(s.label);
    if (s.active) {
      t |= bold;
    }
    if (s.focused) {
      t |= inverted;
    }
    return hbox({prefix, t});
  };
  return option;
}

/// @brief 標準Radiobox的選項。
// static
RadioboxOption RadioboxOption::Simple() {
  auto option = RadioboxOption();
  option.transform = [](const EntryState& s) {
    auto prefix = (Terminal::GetQuirks().ComponentAscii())
                      ? text(s.state ? "(*) " : "( ) ")  // NOLINT
                      : text(s.state ? "◉ " : "○ ");     // NOLINT
    auto t = text(s.label);
    if (s.active) {
      t |= bold;
    }
    if (s.focused) {
      t |= inverted;
    }
    return hbox({prefix, t});
  };
  return option;
}

/// @brief 輸入組件的標準選項。
// static
InputOption InputOption::Default() {
  InputOption option;
  option.transform = [](InputState state) {
    if (state.is_placeholder) {
      state.element |= dim;
    }

    if (state.focused) {
      state.element |= inverted;
    } else if (state.hovered) {
      state.element |= underlined;
    }

    return state.element;
  };
  return option;
}

/// @brief 更美觀的輸入組件的標準選項。
// static
InputOption InputOption::Spacious() {
  InputOption option;
  option.transform = [](InputState state) {
    state.element |= borderEmpty;

    if (state.is_placeholder) {
      state.element |= dim;
    }

    if (state.focused) {
      state.element |= inverted;
    } else if (state.hovered) {
      state.element |= bold;
    }

    return state.element;
  };
  return option;
}

}  // namespace ftxui
