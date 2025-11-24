// Copyright 2020 Arthur Sonzogni. All rights reserved.
// 本原始碼的使用受 MIT 授權條款約束，詳情請見 LICENSE 檔案。
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
    // Microsoft 的終端機對於返回鍵使用不同的換行符號。
    // 這在 Linux 上使用 `bind` 命令時也會發生：
    // 請參閱 https://github.com/ArthurSonzogni/FTXUI/issues/337
    // 在此，我們將換行符號統一為 `\n`。
    {"\r", "\n"},

    // 請參閱：https://github.com/ArthurSonzogni/FTXUI/issues/508
    {std::string({8}), std::string({127})},

    // 請參閱：https://github.com/ArthurSonzogni/FTXUI/issues/626
    //
    // 根據游標鍵模式 (DECCKM)，終端機會發送不同的逸出序列：
    //
    //   按鍵    普通模式   應用程式模式
    //   -----   --------  -----------
    //   上      ESC [ A   ESC O A
    //   下      ESC [ B   ESC O B
    //   右      ESC [ C   ESC O C
    //   左      ESC [ D   ESC O D
    //   Home    ESC [ H   ESC O H
    //   End     ESC [ F   ESC O F
    //
    {"\x1BOA", "\x1B[A"},  // 上
    {"\x1BOB", "\x1B[B"},  // 下
    {"\x1BOC", "\x1B[C"},  // 右
    {"\x1BOD", "\x1B[D"},  // 左
    {"\x1BOH", "\x1B[H"},  // Home
    {"\x1BOF", "\x1B[F"},  // End

    // FN 鍵的變體。
    // 內部我們使用：
    // vt220, xterm-vt200, xterm-xf86-v44, xterm-new, mgt, screen
    // 請參閱：https://invisible-island.net/xterm/xterm-function-keys.html

    // 適用於 Linux 作業系統控制台 (CTRL+ALT+FN)，不屬於任何
    // 真實標準。
    // 請參閱：https://github.com/ArthurSonzogni/FTXUI/issues/685
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
  }
  // NOT_REACHED().
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

// 字碼點 <-> UTF-8 轉換
//
// ┏━━━━━━━━┳━━━━━━━━┳━━━━━━━━┳━━━━━━━━┓
// ┃位元組 1┃位元組 2┃位元組 3┃位元組 4┃
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
// 如果存在相同字碼點的較短表示，則某些序列是非法的。
TerminalInputParser::Output TerminalInputParser::ParseUTF8() {
  auto head = Current();
  unsigned char selector = 0b1000'0000;  // NOLINT

  // 第一個位元組中非字碼點的部分。
  unsigned char mask = selector;

  // 在第一個位元組中找到第一個零。
  unsigned int first_zero = 8;            // NOLINT
  for (unsigned int i = 0; i < 8; ++i) {  // NOLINT
    mask |= selector;
    if (!(head & selector)) {
      first_zero = i;
      break;
    }
    selector >>= 1U;
  }

  // Accumulate the value of the first byte.
  auto value = uint32_t(head & ~mask);  // NOLINT

  // 無效的 UTF8，超過 5 個位元組。
  const unsigned int max_utf8_bytes = 5;
  if (first_zero == 1 || first_zero >= max_utf8_bytes) {
    return DROP;
  }

  // 多位元組 UTF-8。
  for (unsigned int i = 2; i <= first_zero; ++i) {
    if (!Eat()) {
      return UNCOMPLETED;
    }

    // 無效的連續位元組。
    head = Current();
    if ((head & 0b1100'0000) != 0b1000'0000) {  // NOLINT
      return DROP;
    }
    value <<= 6;                  // NOLINT
    value += head & 0b0011'1111;  // NOLINT
  }

  // 檢查過長的 UTF8 編碼。
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

    // Expecting 2 characters.
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
      return SPECIAL;
    }
    // Expecting 1 character:
    default:
      return SPECIAL;
  }
}

// ESC P ... ESC 反斜線
TerminalInputParser::Output TerminalInputParser::ParseDCS() {
  // 解析直到字串終止符 ST。
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
  bool altered = false;
  int argument = 0;
  std::vector<int> arguments;
  while (true) {
    if (!Eat()) {
      return UNCOMPLETED;
    }

    if (Current() == '<') {
      altered = true;
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

    // CSI 由範圍 0x40–0x7E 中的字元終止
    // (ASCII @A–Z[\^_`a–z{|}~)。
    // 注意：我不記得為什麼我們排除了 '<'
    // 為了處理 F1-F4，我們排除了 '['。
    if (Current() >= '@' && Current() <= '~' &&
        // Note: I don't remember why we exclude '<'
        Current() != '<' &&
        // To handle F1-F4, we exclude '['.
        Current() != '[') {
      arguments.push_back(argument);
      argument = 0;  // NOLINT

      switch (Current()) {
        case 'M':
          return ParseMouse(altered, true, std::move(arguments));
        case 'm':
          return ParseMouse(altered, false, std::move(arguments));
        case 'R':
          return ParseCursorPosition(std::move(arguments));
        default:
          return SPECIAL;
      }
    }

    // CSI 中無效的 ESC。
    if (Current() == '\x1B') {
      return SPECIAL;
    }
  }
}

TerminalInputParser::Output TerminalInputParser::ParseOSC() {
  // 解析直到字串終止符 ST。
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

  // 位元 值 修飾符 註解
  // ---- ----- ------- ---------
  // 0 1  1 2   按鈕    0 = 左鍵, 1 = 中鍵, 2 = 右鍵, 3 = 釋放
  // 2    4     Shift
  // 3    8     Meta
  // 4    16    Control
  // 5    32    移動
  // 6    64    滾輪

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

  // 移動事件。
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

}  // namespace ftxui