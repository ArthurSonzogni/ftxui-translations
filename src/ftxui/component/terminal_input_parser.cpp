// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/component/terminal_input_parser.hpp"

#include <cstdint>                    // for uint32_t
#include <ftxui/component/mouse.hpp>  // for Mouse, Mouse::Button, Mouse::Motion
#include <functional>                 // for std::function
#include <map>
#include <memory>   // for unique_ptr, allocator
#include <utility>  // for move
#include <vector>
#include "ftxui/component/event.hpp"  // for Event
#include "ftxui/component/task.hpp"   // for Task

namespace ftxui {

// NOLINTNEXTLINE
const std::map<std::string, std::string> g_uniformize = {
    // Microsoftのターミナルは、returnキーに異なる改行文字を使用します。
    // これは、Linuxでも`bind`コマンドで発生します。
    // 詳細: https://github.com/ArthurSonzogni/FTXUI/issues/337
    // ここでは、改行文字を`\n`に統一します。
    {"\r", "\n"},

    // 詳細: https://github.com/ArthurSonzogni/FTXUI/issues/508
    {std::string({8}), std::string({127})},

    // 詳細: https://github.com/ArthurSonzogni/FTXUI/issues/626
    // 
    // カーソルキーモード (DECCKM) に応じて、ターミナルは異なるエスケープシーケンスを送信します。
    // 
    //   キー     通常    アプリケーション
    //   -----   --------  -----------
    //   上      ESC [ A   ESC O A
    //   下      ESC [ B   ESC O B
    //   右      ESC [ C   ESC O C
    //   左      ESC [ D   ESC O D
    //   Home    ESC [ H   ESC O H
    //   End     ESC [ F   ESC O F
    // 
    {"\x1BOA", "\x1B[A"},  // UP
    {"\x1BOB", "\x1B[B"},  // DOWN
    {"\x1BOC", "\x1B[C"},  // RIGHT
    {"\x1BOD", "\x1B[D"},  // LEFT
    {"\x1BOH", "\x1B[H"},  // HOME
    {"\x1BOF", "\x1B[F"},  // END

    // 端末やマルチプレクサから来る一般的なHome/Endシーケンス。
    {"\x1B[1~", "\x1B[H"},  // HOME
    {"\x1B[4~", "\x1B[F"},  // END

    // FNキーのバリエーション。
    // 内部的には以下を使用しています。
    // vt220, xterm-vt200, xterm-xf86-v44, xterm-new, mgt, screen
    // 詳細: https://invisible-island.net/xterm/xterm-function-keys.html

    // どの標準にも属さないLinux OSコンソール (CTRL+ALT+FN) の場合。
    // 詳細: https://github.com/ArthurSonzogni/FTXUI/issues/685
    {"\x1B[[A", "\x1BOP"},    // F1
    {"\x1B[[B", "\x1BOQ"},    // F2
    {"\x1B[[C", "\x1BOR"},    // F3
    {"\x1B[[D", "\x1BOS"},    // F4
    {"\x1B[[E", "\x1B[15~"},  // F5

    // xterm-r5, xterm-r6, rxvt
    {"\x1B[11~", "\x1BOP"},  // F1
    {"\x1B[12~", "\x1BOQ"},  // F2
    {"\x1B[13~", "\x1BOR"},  // F3
    {"\x1B[14~", "\x1BOS"},  // F4

    // vt100
    {"\x1BOt", "\x1B[15~"},  // F5
    {"\x1BOu", "\x1B[17~"},  // F6
    {"\x1BOv", "\x1B[18~"},  // F7
    {"\x1BOl", "\x1B[19~"},  // F8
    {"\x1BOw", "\x1B[20~"},  // F9
    {"\x1BOx", "\x1B[21~"},  // F10

    // scoansi
    {"\x1B[M", "\x1BOP"},    // F1
    {"\x1B[N", "\x1BOQ"},    // F2
    {"\x1B[O", "\x1BOR"},    // F3
    {"\x1B[P", "\x1BOS"},    // F4
    {"\x1B[Q", "\x1B[15~"},  // F5
    {"\x1B[R", "\x1B[17~"},  // F6
    {"\x1B[S", "\x1B[18~"},  // F7
    {"\x1B[T", "\x1B[19~"},  // F8
    {"\x1B[U", "\x1B[20~"},  // F9
    {"\x1B[V", "\x1B[21~"},  // F10
    {"\x1B[W", "\x1B[23~"},  // F11
    {"\x1B[X", "\x1B[24~"},  // F12
};

TerminalInputParser::TerminalInputParser(std::function<void(Event)> out)
    : out_(std::move(out)) {}

void TerminalInputParser::Timeout(int time) {
  timeout_ += time;
  const int timeout_threshold = 50;
  if (timeout_ < timeout_threshold) {
    return;
  }
  timeout_ = 0;
  if (!pending_.empty()) {
    Send(SPECIAL);
  }
}

void TerminalInputParser::Add(char c) {
  pending_ += c;
  timeout_ = 0;
  position_ = -1;
  Send(Parse());
}

unsigned char TerminalInputParser::Current() {
  return pending_[position_];
}

bool TerminalInputParser::Eat() {
  position_++;
  return position_ < static_cast<int>(pending_.size());
}

void TerminalInputParser::Send(TerminalInputParser::Output output) {
  switch (output.type) {
    case UNCOMPLETED:
      return;

    case DROP:
      pending_.clear();
      return;

    case RESYNC: {
      // これまでに蓄積されたバイト列は、|position_|にあるバイトで
      // 継続できず、新しいシーケンスを開始する。切り詰められた接頭辞を
      // 発行し、残りのバイトを再度パースする。
      std::string next = pending_.substr(position_);
      pending_.resize(position_);
      Send(SPECIAL);
      pending_ = std::move(next);
      position_ = -1;
      Send(Parse());
      return;
    }

    case CHARACTER:
      out_(Event::Character(std::move(pending_)));
      pending_.clear();
      return;

    case SPECIAL: {
      auto it = g_uniformize.find(pending_);
      if (it != g_uniformize.end()) {
        pending_ = it->second;
      }
      out_(Event::Special(std::move(pending_)));
      pending_.clear();
    }
      return;

    case MOUSE:
      out_(Event::Mouse(std::move(pending_), output.mouse));  // NOLINT
      pending_.clear();
      return;

    case CURSOR_POSITION:
      out_(Event::CursorPosition(std::move(pending_),  // NOLINT
                                 output.cursor.x,      // NOLINT
                                 output.cursor.y));    // NOLINT
      pending_.clear();
      return;

    case CURSOR_SHAPE:
      out_(Event::CursorShape(std::move(pending_), output.cursor_shape));
      pending_.clear();
      return;

    case TERMINAL_NAME_VERSION:
      out_(Event::TerminalNameVersion(std::move(pending_),
                                      std::move(output.terminal_name),
                                      output.terminal_version));
      pending_.clear();
      return;

    case TERMINAL_EMULATOR:
      out_(Event::TerminalEmulator(std::move(pending_),
                                   std::move(output.terminal_name),
                                   std::move(output.terminal_version_string)));
      pending_.clear();
      return;

    case TERMINAL_CAPABILITIES:
      out_(Event::TerminalCapabilities(
          std::move(pending_), std::move(output.terminal_capabilities)));
      pending_.clear();
      return;
  }
  // NOT_REACHED()。
}

TerminalInputParser::Output TerminalInputParser::Parse() {
  if (!Eat()) {
    return UNCOMPLETED;
  }

  if (Current() == '\x1B') {
    return ParseESC();
  }

  if (Current() < 32) {  // C0 NOLINT
    return SPECIAL;
  }

  if (Current() == 127) {  // Delete // NOLINT
    return SPECIAL;
  }

  return ParseUTF8();
}

// コードポイント <-> UTF-8変換
//
// ┏━━━━━━━━┳━━━━━━━━┳━━━━━━━━┳━━━━━━━━┓
// ┃Byte 1  ┃Byte 2  ┃Byte 3  ┃Byte 4  ┃
// ┡━━━━━━━━╇━━━━━━━━╇━━━━━━━━╇━━━━━━━━┩
// │0xxxxxxx│        │        │        │
// ├────────┼────────┼────────┼────────┤
// │110xxxxx│10xxxxxx│        │        │
// ├────────┼────────┼────────┼────────┤
// │1110xxxx│10xxxxxx│10xxxxxx│        │
// ├────────┼────────┼────────┼────────┤
// │11110xxx│10xxxxxx│10xxxxxx│10xxxxxx│
// └────────┴────────┴────────┴────────┘
//
// 同じコードポイントのより短い表現が存在する場合、一部のシーケンスは不正になります。
TerminalInputParser::Output TerminalInputParser::ParseUTF8() {
  auto head = Current();
  unsigned char selector = 0b1000'0000;  // NOLINT

  // 最初のバイトのコードポイント以外の部分。
  unsigned char mask = selector;

  // 最初のバイトの中の最初のゼロを見つける。
  unsigned int first_zero = 8;            // NOLINT
  for (unsigned int i = 0; i < 8; ++i) {  // NOLINT
    mask |= selector;
    if (!(head & selector)) {
      first_zero = i;
      break;
    }
    selector >>= 1U;
  }

  // 最初のバイトの値を累積する。
  auto value = uint32_t(head & ~mask);  // NOLINT

  // 5バイトを超える無効なUTF8。
  const unsigned int max_utf8_bytes = 5;
  if (first_zero == 1 || first_zero >= max_utf8_bytes) {
    return DROP;
  }

  // マルチバイトUTF-8。
  for (unsigned int i = 2; i <= first_zero; ++i) {
    if (!Eat()) {
      return UNCOMPLETED;
    }

    // 無効な継続バイト。
    head = Current();
    if ((head & 0b1100'0000) != 0b1000'0000) {  // NOLINT
      return DROP;
    }
    value <<= 6;                  // NOLINT
    value += head & 0b0011'1111;  // NOLINT
  }

  // オーバーロングUTF8エンコーディングのチェック。
  int extra_byte = 0;
  if (value <= 0b000'0000'0111'1111) {                 // NOLINT
    extra_byte = 0;                                    // NOLINT
  } else if (value <= 0b000'0111'1111'1111) {          // NOLINT
    extra_byte = 1;                                    // NOLINT
  } else if (value <= 0b1111'1111'1111'1111) {         // NOLINT
    extra_byte = 2;                                    // NOLINT
  } else if (value <= 0b1'0000'1111'1111'1111'1111) {  // NOLINT
    extra_byte = 3;                                    // NOLINT
  } else {                                             // NOLINT
    return DROP;
  }

  if (extra_byte != position_) {
    return DROP;
  }

  return CHARACTER;
}

TerminalInputParser::Output TerminalInputParser::ParseESC() {
  if (!Eat()) {
    return UNCOMPLETED;
  }
  switch (Current()) {
    case 'P':
      return ParseDCS();
    case '[':
      return ParseCSI();
    case ']':
      return ParseOSC();

    // ESCはシーケンス内では許可されない。これは新しいシーケンスを開始する。
    case '\x1B':
      return RESYNC;

    // 2文字を期待している。
    case ' ':
    case '#':
    case '%':
    case '(':
    case ')':
    case '*':
    case '+':
    case 'O':
    case 'N': {
      if (!Eat()) {
        return UNCOMPLETED;
      }
      if (Current() == '\x1B') {
        return RESYNC;
      }
      return SPECIAL;
    }
    // 1文字を期待している:
    default:
      return SPECIAL;
  }
}

// ESC P ... ESC BACKSLASH
TerminalInputParser::Output TerminalInputParser::ParseDCS() {
  // 文字列終端子STまでパースする。
  while (true) {
    if (!Eat()) {
      return UNCOMPLETED;
    }

    if (Current() != '\x1B') {
      continue;
    }

    if (!Eat()) {
      return UNCOMPLETED;
    }

    if (Current() != '\\') {
      continue;
    }

    // XTVERSION: ESC P > | name version ST
    if (pending_.size() >= 5 && pending_[2] == '>' && pending_[3] == '|') {
      // ESC P > | name (version) ST
      // 0   1 2 3 4
      const std::string content = pending_.substr(4, pending_.size() - 6);
      Output output(TERMINAL_EMULATOR);
      const size_t space = content.find(' ');
      const size_t open_paren = content.find('(');
      if (space != std::string::npos) {
        output.terminal_name = content.substr(0, space);
        output.terminal_version_string = content.substr(space + 1);
      } else if (open_paren != std::string::npos) {
        output.terminal_name = content.substr(0, open_paren);
        output.terminal_version_string = content.substr(open_paren + 1);
        if (!output.terminal_version_string.empty() &&
            output.terminal_version_string.back() == ')') {
          output.terminal_version_string.pop_back();
        }
      } else {
        output.terminal_name = content;
        output.terminal_version_string = "unknown";
      }
      return output;
    }

    if (pending_.size() == 10 &&  //
        pending_[2] == '1' &&     //
        pending_[3] == '$' &&     //
        pending_[4] == 'r' &&     //
        true) {
      Output output(CURSOR_SHAPE);
      output.cursor_shape = pending_[5] - '0';
      return output;
    }

    return SPECIAL;
  }
}

TerminalInputParser::Output TerminalInputParser::ParseCSI() {
  bool altered_less = false;
  bool altered_greater = false;
  bool altered_question = false;
  int argument = 0;
  std::vector<int> arguments;
  while (true) {
    if (!Eat()) {
      return UNCOMPLETED;
    }

    if (Current() == '<') {
      altered_less = true;
      continue;
    }

    if (Current() == '>') {
      altered_greater = true;
      continue;
    }

    if (Current() == '?') {
      altered_question = true;
      continue;
    }

    if (Current() >= '0' && Current() <= '9') {
      argument *= 10;  // NOLINT
      argument += Current() - '0';
      continue;
    }

    if (Current() == ';') {
      arguments.push_back(argument);
      argument = 0;
      continue;
    }

    // CSIは0x40-0x7Eの範囲の文字で終了します。
    // (ASCII @A–Z[\\\]^_`a–z{|}~),
    if (Current() >= '@' && Current() <= '~' &&
        // 注:なぜ '<' を除外するのか覚えていない
        Current() != '<' &&
        // F1-F4を処理するために、'['を除外する。
        Current() != '[') {
      arguments.push_back(argument);
      argument = 0;  // NOLINT

      switch (Current()) {
        case 'M':
          return ParseMouse(altered_less, true, std::move(arguments));
        case 'm':
          return ParseMouse(altered_less, false, std::move(arguments));
        case 'R':
          return ParseCursorPosition(std::move(arguments));
        case 'c':
          return ParseDeviceAttributes(altered_greater, altered_question,
                                       std::move(arguments));
        default:
          return SPECIAL;
      }
    }

    // CSI内の無効なESC。これは新しいシーケンスを開始する。
    if (Current() == '\x1B') {
      return RESYNC;
    }
  }
}

TerminalInputParser::Output TerminalInputParser::ParseOSC() {
  // 文字列終端子STまでパースする。
  while (true) {
    if (!Eat()) {
      return UNCOMPLETED;
    }
    if (Current() != '\x1B') {
      continue;
    }
    if (!Eat()) {
      return UNCOMPLETED;
    }
    if (Current() != '\\') {
      continue;
    }
    return SPECIAL;
  }
}

TerminalInputParser::Output TerminalInputParser::ParseMouse(  // NOLINT
    bool altered,
    bool pressed,
    std::vector<int> arguments) {
  if (arguments.size() != 3) {
    return SPECIAL;
  }

  (void)altered;

  Output output(MOUSE);
  output.mouse.motion = Mouse::Motion(pressed);  // NOLINT

  // Bits value Modifier  Comment
  // ---- ----- ------- ---------
  // 0 1  1 2   button   0 = Left, 1 = Middle, 2 = Right, 3 = Release
  // 2    4     Shift
  // 3    8     Meta
  // 4    16    Control
  // 5    32    Move
  // 6    64    Wheel

  // clang-format off
  const int button      = arguments[0] & (1 + 2); // NOLINT
  const bool is_shift   = arguments[0] & 4;       // NOLINT
  const bool is_meta    = arguments[0] & 8;       // NOLINT
  const bool is_control = arguments[0] & 16;      // NOLINT
  const bool is_move    = arguments[0] & 32;      // NOLINT
  const bool is_wheel   = arguments[0] & 64;      // NOLINT
  // clang-format on

  output.mouse.motion = is_move ? Mouse::Moved : Mouse::Motion(pressed);
  output.mouse.button = is_wheel ? Mouse::Button(Mouse::WheelUp + button)  //
                                 : Mouse::Button(button);
  output.mouse.shift = is_shift;
  output.mouse.meta = is_meta;
  output.mouse.control = is_control;
  output.mouse.x = arguments[1];  // NOLINT
  output.mouse.y = arguments[2];  // NOLINT

  // 移動イベント。
  return output;
}

// NOLINTNEXTLINE
TerminalInputParser::Output TerminalInputParser::ParseCursorPosition(
    std::vector<int> arguments) {
  if (arguments.size() != 2) {
    return SPECIAL;
  }
  Output output(CURSOR_POSITION);
  output.cursor.y = arguments[0];  // NOLINT
  output.cursor.x = arguments[1];  // NOLINT
  return output;
}

// NOLINTNEXTLINE
TerminalInputParser::Output TerminalInputParser::ParseDeviceAttributes(
    bool altered_greater,
    bool altered_question,
    std::vector<int> arguments) {
  if (altered_greater) {
    // 二次デバイス属性 (DA2)
    // ESC [ > Pp ; Pv ; Pc c
    if (arguments.size() >= 3) {
      // Pp: 端末種別
      // Pv: ファームウェアバージョン
      // Pc: ハードウェアオプション
      Output output(TERMINAL_NAME_VERSION);
      output.terminal_version = arguments[1];
      switch (arguments[0]) {
        case 0:
          output.terminal_name = "xterm";
          break;
        case 1:
          output.terminal_name = "vt220";
          break;
        case 2:
          output.terminal_name = "vt240";
          break;
        case 18:
          output.terminal_name = "vt330";
          break;
        case 19:
          output.terminal_name = "vt340";
          break;
        case 24:
          output.terminal_name = "vt320";
          break;
        case 41:
          output.terminal_name = "vt420";
          break;
        case 61:
          output.terminal_name = "vt510";
          break;
        case 64:
          output.terminal_name = "vt520";
          break;
        case 65:
          output.terminal_name = "vt525";
          break;
        case 84:
          output.terminal_name = "tmux";
          break;
        case 85:
          output.terminal_name = "urxvt";
          break;
        default:
          output.terminal_name = "unknown";
          break;
      }
      // xtermの特殊ケースで、0;pv;0のような値をよく返すが、
      // 厳密にDEC VTタイプに従っているわけではない。
      return output;
    }
  } else if (altered_question) {
    // 一次デバイス属性 (DA1)
    // ESC [ ? Pp ; ... c
    Output output(TERMINAL_CAPABILITIES);
    output.terminal_capabilities = std::move(arguments);
    return output;
  }
  return SPECIAL;
}

}  // namespace ftxui
