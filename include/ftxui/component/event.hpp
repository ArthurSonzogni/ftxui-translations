// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_COMPONENT_EVENT_HPP
#define FTXUI_COMPONENT_EVENT_HPP

#include <ftxui/component/mouse.hpp>  // for Mouse
#include <string>                     // for string, operator==

namespace ftxui {

class ScreenInteractive;
class ComponentBase;

/// @brief イベントを表します。キープレスイベント、ターミナルのリサイズなど、さまざまなイベントがあります。
///
/// 例:
/// - 表示可能な文字はEvent::Character('a')を使用して作成できます。
/// - Event::ArrowLeftのような特殊なイベントは事前に定義されています。
/// - 特別なイベントの任意のコードは、以下を使用して見つけることができます。
///   ./example/util/print_key_press
///   例えば、CTLR+AはEvent::Special({1})にマップされます。
///
/// xtermの仕様に関する役立つドキュメント:
/// https://invisible-island.net/xterm/ctlseqs/ctlseqs.html
///
/// @ingroup component
struct Event {
  // --- コンストラクタセクション ---------------------------------------------------
  static Event Character(std::string);
  static Event Character(char);
  static Event Character(wchar_t);
  static Event Special(std::string);
  static Event Mouse(std::string, Mouse mouse);
  static Event CursorPosition(std::string, int x, int y);  // 内部
  static Event CursorShape(std::string, int shape);        // 内部

  // --- 矢印キー ---
  static const Event ArrowLeft;
  static const Event ArrowRight;
  static const Event ArrowUp;
  static const Event ArrowDown;

  static const Event ArrowLeftCtrl;
  static const Event ArrowRightCtrl;
  static const Event ArrowUpCtrl;
  static const Event ArrowDownCtrl;

  // --- その他 ---
  static const Event Backspace;
  static const Event Delete;
  static const Event Return;
  static const Event Escape;
  static const Event Tab;
  static const Event TabReverse;

  // --- ナビゲーションキー ---
  static const Event Insert;
  static const Event Home;
  static const Event End;
  static const Event PageUp;
  static const Event PageDown;

  // --- ファンクションキー ---
  static const Event F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12;

  // --- コントロールキー ---
  static const Event a, A, CtrlA, AltA, CtrlAltA;
  static const Event b, B, CtrlB, AltB, CtrlAltB;
  static const Event c, C, CtrlC, AltC, CtrlAltC;
  static const Event d, D, CtrlD, AltD, CtrlAltD;
  static const Event e, E, CtrlE, AltE, CtrlAltE;
  static const Event f, F, CtrlF, AltF, CtrlAltF;
  static const Event g, G, CtrlG, AltG, CtrlAltG;
  static const Event h, H, CtrlH, AltH, CtrlAltH;
  static const Event i, I, CtrlI, AltI, CtrlAltI;
  static const Event j, J, CtrlJ, AltJ, CtrlAltJ;
  static const Event k, K, CtrlK, AltK, CtrlAltK;
  static const Event l, L, CtrlL, AltL, CtrlAltL;
  static const Event m, M, CtrlM, AltM, CtrlAltM;
  static const Event n, N, CtrlN, AltN, CtrlAltN;
  static const Event o, O, CtrlO, AltO, CtrlAltO;
  static const Event p, P, CtrlP, AltP, CtrlAltP;
  static const Event q, Q, CtrlQ, AltQ, CtrlAltQ;
  static const Event r, R, CtrlR, AltR, CtrlAltR;
  static const Event s, S, CtrlS, AltS, CtrlAltS;
  static const Event t, T, CtrlT, AltT, CtrlAltT;
  static const Event u, U, CtrlU, AltU, CtrlAltU;
  static const Event v, V, CtrlV, AltV, CtrlAltV;
  static const Event w, W, CtrlW, AltW, CtrlAltW;
  static const Event x, X, CtrlX, AltX, CtrlAltX;
  static const Event y, Y, CtrlY, AltY, CtrlAltY;
  static const Event z, Z, CtrlZ, AltZ, CtrlAltZ;

  // --- カスタム ---
  static const Event Custom;

  //--- メソッドセクション ---------------------------------------------------------
  bool operator==(const Event& other) const { return input_ == other.input_; }
  bool operator!=(const Event& other) const { return !operator==(other); }
  bool operator<(const Event& other) const { return input_ < other.input_; }

  const std::string& input() const { return input_; }

  bool is_character() const { return type_ == Type::Character; }
  std::string character() const { return input_; }

  bool is_mouse() const { return type_ == Type::Mouse; }
  struct Mouse& mouse() { return data_.mouse; }

  // --- 内部メソッドセクション -----------------------------------------------
  bool is_cursor_position() const { return type_ == Type::CursorPosition; }
  int cursor_x() const { return data_.cursor.x; }
  int cursor_y() const { return data_.cursor.y; }

  bool is_cursor_shape() const { return type_ == Type::CursorShape; }
  int cursor_shape() const { return data_.cursor_shape; }

  // デバッグ
  std::string DebugString() const;

  //--- 状態セクション ----------------------------------------------------------
  ScreenInteractive* screen_ = nullptr;

 private:
  friend ComponentBase;
  friend ScreenInteractive;
  enum class Type {
    Unknown,
    Character,
    Mouse,
    CursorPosition,
    CursorShape,
  };
  Type type_ = Type::Unknown;

  struct Cursor {
    int x = 0;
    int y = 0;
  };

  union {
    struct Mouse mouse;
    struct Cursor cursor;
    int cursor_shape;
  } data_ = {};

  std::string input_;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_EVENT_HPP */