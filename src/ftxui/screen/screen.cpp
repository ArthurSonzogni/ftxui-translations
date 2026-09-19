// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <cstddef>  // for size_t
#include <cstdint>
#include <iostream>  // for cout, flush
#include <limits>
#include <map>     // for _Rb_tree_const_iterator, map, operator!=, operator==
#include <string>  // for string
#include <string_view>  // for string_view
#include <utility>      // for pair

#include "ftxui/screen/cell.hpp"  // for Cell
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"    // for string_width
#include "ftxui/screen/surface.hpp"   // for Surface
#include "ftxui/screen/terminal.hpp"  // for Dimensions, Size

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

// 用於提示某表達式可能為 false 的巨集。
#if !defined(FTXUI_UNLIKELY)
#if defined(COMPILER_GCC) || defined(__clang__)
#define FTXUI_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define FTXUI_UNLIKELY(x) (x)
#endif  // defined(COMPILER_GCC)
#endif  // !defined(FTXUI_UNLIKELY)

#if !defined(FTXUI_LIKELY)
#if defined(COMPILER_GCC) || defined(__clang__)
#define FTXUI_LIKELY(x) __builtin_expect(!!(x), 1)
#else
#define FTXUI_LIKELY(x) (x)
#endif  // defined(COMPILER_GCC)
#endif  // !defined(FTXUI_LIKELY)

namespace ftxui {

namespace {

#if defined(_WIN32)
void WindowsEmulateVT100Terminal() {
  static bool done = false;
  if (done) {
    return;
  }
  done = true;

  // 在 stdout 和 stdin 上啟用 VT 處理
  auto stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

  DWORD out_mode = 0;
  if (!GetConsoleMode(stdout_handle, &out_mode)) {
    // 輸出並非主控台（例如被重新導向到檔案或管線）。保留
    // 偵測到的顏色支援，並讓串流的使用者自行解讀
    // 逸出序列。
    return;
  }

  // https://docs.microsoft.com/en-us/windows/console/setconsolemode
  const int enable_virtual_terminal_processing = 0x0004;
  const int disable_newline_auto_return = 0x0008;
  out_mode |= enable_virtual_terminal_processing;
  out_mode |= disable_newline_auto_return;

  SetConsoleMode(stdout_handle, out_mode);
}
#endif

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void UpdateCellStyle(const Screen* screen,
                     std::string& ss,
                     const Cell& prev,
                     const Cell& next) {
  // 參見 https://gist.github.com/egmontkob/eb114294efbcd5adb1944c9f3cb5feda
  if (FTXUI_UNLIKELY(next.hyperlink != prev.hyperlink)) {
    ss += "\x1B]8;;";
    ss += screen->Hyperlink(next.hyperlink);
    ss += "\x1B\\";
  }

  // 粗體
  if (FTXUI_UNLIKELY((next.bold ^ prev.bold) | (next.dim ^ prev.dim))) {
    // BOLD_AND_DIM_RESET:
    if ((prev.bold && !next.bold) || (prev.dim && !next.dim)) {
      ss += "\x1B[22m";
    }
    if (next.bold) {
      ss += "\x1B[1m";  // BOLD_SET
    }
    if (next.dim) {
      ss += "\x1B[2m";  // DIM_SET
    }
  }

  // 底線
  if (FTXUI_UNLIKELY(next.underlined != prev.underlined ||
                     next.underlined_double != prev.underlined_double)) {
    ss += (next.underlined          ? "\x1B[4m"     // UNDERLINE
           : next.underlined_double ? "\x1B[21m"    // UNDERLINE_DOUBLE
                                    : "\x1B[24m");  // UNDERLINE_RESET
  }

  // 閃爍
  if (FTXUI_UNLIKELY(next.blink != prev.blink)) {
    ss += (next.blink ? "\x1B[5m"     // BLINK_SET
                      : "\x1B[25m");  // BLINK_RESET
  }

  // 反相
  if (FTXUI_UNLIKELY(next.inverted != prev.inverted)) {
    ss += (next.inverted ? "\x1B[7m"     // INVERTED_SET
                         : "\x1B[27m");  // INVERTED_RESET
  }

  // 斜體
  if (FTXUI_UNLIKELY(next.italic != prev.italic)) {
    ss += (next.italic ? "\x1B[3m"     // ITALIC_SET
                       : "\x1B[23m");  // ITALIC_RESET
  }

  // 刪除線
  if (FTXUI_UNLIKELY(next.strikethrough != prev.strikethrough)) {
    ss += (next.strikethrough ? "\x1B[9m"     // CROSSED_OUT
                              : "\x1B[29m");  // CROSSED_OUT_RESET
  }

  if (FTXUI_UNLIKELY(next.foreground_color != prev.foreground_color ||
                     next.background_color != prev.background_color)) {
    ss += "\x1B[";
    next.foreground_color.PrintTo(ss, false);
    ss += 'm';
    ss += "\x1B[";
    next.background_color.PrintTo(ss, true);
    ss += 'm';
  }
}

struct TileEncoding {
  std::uint8_t left : 2;
  std::uint8_t top : 2;
  std::uint8_t right : 2;
  std::uint8_t down : 2;
  std::uint8_t round : 1;

  // clang-format off
  bool operator<(const TileEncoding& other) const {
    if (left < other.left)   { return  true;  }
    if (left > other.left)   { return  false; }
    if (top < other.top)     { return  true;  }
    if (top > other.top)     { return  false; }
    if (right < other.right) { return  true;  }
    if (right > other.right) { return  false; }
    if (down < other.down)   { return  true;  }
    if (down > other.down)   { return  false; }
    if (round < other.round) { return  true;  }
    if (round > other.round) { return  false; }
      return false;
  }
  // clang-format on
};

// clang-format off
const std::map<std::string, TileEncoding> tile_encoding = { // NOLINT
    {"─", {1, 0, 1, 0, 0}},
    {"━", {2, 0, 2, 0, 0}},
    {"╍", {2, 0, 2, 0, 0}},

    {"│", {0, 1, 0, 1, 0}},
    {"┃", {0, 2, 0, 2, 0}},
    {"╏", {0, 2, 0, 2, 0}},

    {"┌", {0, 0, 1, 1, 0}},
    {"┍", {0, 0, 2, 1, 0}},
    {"┎", {0, 0, 1, 2, 0}},
    {"┏", {0, 0, 2, 2, 0}},

    {"┐", {1, 0, 0, 1, 0}},
    {"┑", {2, 0, 0, 1, 0}},
    {"┒", {1, 0, 0, 2, 0}},
    {"┓", {2, 0, 0, 2, 0}},

    {"└", {0, 1, 1, 0, 0}},
    {"┕", {0, 1, 2, 0, 0}},
    {"┖", {0, 2, 1, 0, 0}},
    {"┗", {0, 2, 2, 0, 0}},

    {"┘", {1, 1, 0, 0, 0}},
    {"┙", {2, 1, 0, 0, 0}},
    {"┚", {1, 2, 0, 0, 0}},
    {"┛", {2, 2, 0, 0, 0}},

    {"├", {0, 1, 1, 1, 0}},
    {"┝", {0, 1, 2, 1, 0}},
    {"┞", {0, 2, 1, 1, 0}},
    {"┟", {0, 1, 1, 2, 0}},
    {"┠", {0, 2, 1, 2, 0}},
    {"┡", {0, 2, 2, 1, 0}},
    {"┢", {0, 1, 2, 2, 0}},
    {"┣", {0, 2, 2, 2, 0}},

    {"┤", {1, 1, 0, 1, 0}},
    {"┥", {2, 1, 0, 1, 0}},
    {"┦", {1, 2, 0, 1, 0}},
    {"┧", {1, 1, 0, 2, 0}},
    {"┨", {1, 2, 0, 2, 0}},
    {"┩", {2, 2, 0, 1, 0}},
    {"┪", {2, 1, 0, 2, 0}},
    {"┫", {2, 2, 0, 2, 0}},

    {"┬", {1, 0, 1, 1, 0}},
    {"┭", {2, 0, 1, 1, 0}},
    {"┮", {1, 0, 2, 1, 0}},
    {"┯", {2, 0, 2, 1, 0}},
    {"┰", {1, 0, 1, 2, 0}},
    {"┱", {2, 0, 1, 2, 0}},
    {"┲", {1, 0, 2, 2, 0}},
    {"┳", {2, 0, 2, 2, 0}},

    {"┴", {1, 1, 1, 0, 0}},
    {"┵", {2, 1, 1, 0, 0}},
    {"┶", {1, 1, 2, 0, 0}},
    {"┷", {2, 1, 2, 0, 0}},
    {"┸", {1, 2, 1, 0, 0}},
    {"┹", {2, 2, 1, 0, 0}},
    {"┺", {1, 2, 2, 0, 0}},
    {"┻", {2, 2, 2, 0, 0}},

    {"┼", {1, 1, 1, 1, 0}},
    {"┽", {2, 1, 1, 1, 0}},
    {"┾", {1, 1, 2, 1, 0}},
    {"┿", {2, 1, 2, 1, 0}},
    {"╀", {1, 2, 1, 1, 0}},
    {"╁", {1, 1, 1, 2, 0}},
    {"╂", {1, 2, 1, 2, 0}},
    {"╃", {2, 2, 1, 1, 0}},
    {"╄", {1, 2, 2, 1, 0}},
    {"╅", {2, 1, 1, 2, 0}},
    {"╆", {1, 1, 2, 2, 0}},
    {"╇", {2, 2, 2, 1, 0}},
    {"╈", {2, 1, 2, 2, 0}},
    {"╉", {2, 2, 1, 2, 0}},
    {"╊", {1, 2, 2, 2, 0}},
    {"╋", {2, 2, 2, 2, 0}},

    {"═", {3, 0, 3, 0, 0}},
    {"║", {0, 3, 0, 3, 0}},

    {"╒", {0, 0, 3, 1, 0}},
    {"╓", {0, 0, 1, 3, 0}},
    {"╔", {0, 0, 3, 3, 0}},

    {"╕", {3, 0, 0, 1, 0}},
    {"╖", {1, 0, 0, 3, 0}},
    {"╗", {3, 0, 0, 3, 0}},

    {"╘", {0, 1, 3, 0, 0}},
    {"╙", {0, 3, 1, 0, 0}},
    {"╚", {0, 3, 3, 0, 0}},

    {"╛", {3, 1, 0, 0, 0}},
    {"╜", {1, 3, 0, 0, 0}},
    {"╝", {3, 3, 0, 0, 0}},

    {"╞", {0, 1, 3, 1, 0}},
    {"╟", {0, 3, 1, 3, 0}},
    {"╠", {0, 3, 3, 3, 0}},

    {"╡", {3, 1, 0, 1, 0}},
    {"╢", {1, 3, 0, 3, 0}},
    {"╣", {3, 3, 0, 3, 0}},

    {"╤", {3, 0, 3, 1, 0}},
    {"╥", {1, 0, 1, 3, 0}},
    {"╦", {3, 0, 3, 3, 0}},

    {"╧", {3, 1, 3, 0, 0}},
    {"╨", {1, 3, 1, 0, 0}},
    {"╩", {3, 3, 3, 0, 0}},

    {"╪", {3, 1, 3, 1, 0}},
    {"╫", {1, 3, 1, 3, 0}},
    {"╬", {3, 3, 3, 3, 0}},

    {"╭", {0, 0, 1, 1, 1}},
    {"╮", {1, 0, 0, 1, 1}},
    {"╯", {1, 1, 0, 0, 1}},
    {"╰", {0, 1, 1, 0, 1}},

    {"╴", {1, 0, 0, 0, 0}},
    {"╵", {0, 1, 0, 0, 0}},
    {"╶", {0, 0, 1, 0, 0}},
    {"╷", {0, 0, 0, 1, 0}},

    {"╸", {2, 0, 0, 0, 0}},
    {"╹", {0, 2, 0, 0, 0}},
    {"╺", {0, 0, 2, 0, 0}},
    {"╻", {0, 0, 0, 2, 0}},

    {"╼", {1, 0, 2, 0, 0}},
    {"╽", {0, 1, 0, 2, 0}},
    {"╾", {2, 0, 1, 0, 0}},
    {"╿", {0, 2, 0, 1, 0}},
};
// clang-format on

template <class A, class B>
std::map<B, A> InvertMap(const std::map<A, B>& input) {
  std::map<B, A> output;
  for (const auto& it : input) {
    output[it.second] = it.first;
  }
  return output;
}

const std::map<TileEncoding, std::string> tile_encoding_inverse =  // NOLINT
    InvertMap(tile_encoding);

void UpgradeLeftRight(std::string& left, std::string& right) {
  const auto it_left = tile_encoding.find(left);
  if (it_left == tile_encoding.end()) {
    return;
  }
  const auto it_right = tile_encoding.find(right);
  if (it_right == tile_encoding.end()) {
    return;
  }

  if (it_left->second.right == 0 && it_right->second.left != 0) {
    TileEncoding encoding_left = it_left->second;
    encoding_left.right = it_right->second.left;
    const auto it_left_upgrade = tile_encoding_inverse.find(encoding_left);
    if (it_left_upgrade != tile_encoding_inverse.end()) {
      left = it_left_upgrade->second;
    }
  }

  if (it_right->second.left == 0 && it_left->second.right != 0) {
    TileEncoding encoding_right = it_right->second;
    encoding_right.left = it_left->second.right;
    const auto it_right_upgrade = tile_encoding_inverse.find(encoding_right);
    if (it_right_upgrade != tile_encoding_inverse.end()) {
      right = it_right_upgrade->second;
    }
  }
}

void UpgradeTopDown(std::string& top, std::string& down) {
  const auto it_top = tile_encoding.find(top);
  if (it_top == tile_encoding.end()) {
    return;
  }
  const auto it_down = tile_encoding.find(down);
  if (it_down == tile_encoding.end()) {
    return;
  }

  if (it_top->second.down == 0 && it_down->second.top != 0) {
    TileEncoding encoding_top = it_top->second;
    encoding_top.down = it_down->second.top;
    const auto it_top_down = tile_encoding_inverse.find(encoding_top);
    if (it_top_down != tile_encoding_inverse.end()) {
      top = it_top_down->second;
    }
  }

  if (it_down->second.top == 0 && it_top->second.down != 0) {
    TileEncoding encoding_down = it_down->second;
    encoding_down.top = it_top->second.down;
    const auto it_down_top = tile_encoding_inverse.find(encoding_down);
    if (it_down_top != tile_encoding_inverse.end()) {
      down = it_down_top->second;
    }
  }
}

bool ShouldAttemptAutoMerge(Cell& cell) {
  return cell.automerge && cell.character.size() == 3;
}

}  // namespace

/// 固定尺寸。
/// @see Fit
/// @see Full
Dimensions Dimension::Fixed(int v) {
  return {v, v};
}

/// 使用終端尺寸。
/// @see Fixed
/// @see Fit
Dimensions Dimension::Full() {
  return Terminal::Size();
}

// static
/// 沿著 x 軸和 y 軸創建具有給定尺寸的螢幕。
Screen Screen::Create(Dimensions width, Dimensions height) {
  return {width.dimx, height.dimy};
}

// static
/// 創建具有給定尺寸的螢幕。
Screen Screen::Create(Dimensions dimension) {
  return {dimension.dimx, dimension.dimy};
}

Screen::Screen(int dimx, int dimy) : Surface{dimx, dimy} {
#if defined(_WIN32)
  // 此呼叫的位置有點奇怪，不過我們可以假設任何
  // 實例化 Screen 物件的人最終都想要將某些內容輸出到主控台。
  // 如果不是這種情況，請改用 Surface
  // 的實例。由於我們要求所有輸入/輸出操作都使用 UTF8，我們將
  // 在此切換到 UTF8 編碼
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
  WindowsEmulateVT100Terminal();
#endif
}

/// 生成一個可用於在終端上列印螢幕的 std::string。
/// @note 不要忘記刷新 stdout。或者，您可以使用 Screen::Print();
std::string Screen::ToString() const {
  // 預先配置：每個 cell 約 30 位元組，用於字元 + 逸出碼。
  std::string ss;
  ss.reserve(static_cast<size_t>(dimx_) * static_cast<size_t>(dimy_) * 30);
  ToString(ss);
  return ss;
}

/// 產生一個可用於在終端機上輸出 Screen 的 std::string。
/// @param ss 要附加內容的字串。
void Screen::ToString(std::string& ss) const {
  const Cell default_cell;
  const Cell* previous_cell_ref = &default_cell;

  for (int y = 0; y < dimy_; ++y) {
    // 兩行之間的換行。
    if (y != 0) {
      UpdateCellStyle(this, ss, *previous_cell_ref, default_cell);
      previous_cell_ref = &default_cell;
      ss += "\r\n";
    }

    // 印出全形字元後，需要跳過下一個 cell。
    bool previous_fullwidth = false;
    if (dimx_ > 0) {
      const Cell* line_start = &FastCellAt(0, y);
      const Cell* line_end = line_start + dimx_;
      for (const Cell* it = line_start; it != line_end; ++it) {
        const auto& cell = *it;
        if (!previous_fullwidth) {
          UpdateCellStyle(this, ss, *previous_cell_ref, cell);
          previous_cell_ref = &cell;
          if (cell.character.empty()) {
            ss += ' ';
          } else {
            ss += cell.character;
          }
        }
        if (cell.character.size() <= 1) {
          previous_fullwidth = false;
        } else {
          previous_fullwidth = (string_width(cell.character) == 2);
        }
      }
    }
  }

  // 將樣式重設為預設值：
  UpdateCellStyle(this, ss, *previous_cell_ref, default_cell);
}

// 將 Screen 印出到終端機。
void Screen::Print() const {
  std::cout << ToString() << '\0' << std::flush;
}

/// @brief 返回一個字串，用於將游標位置重置到螢幕的開頭。
///
/// ```cpp
/// std::string reset_position;
/// while(true) {
///   auto document = render();
///   auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
///   Render(screen, document);
///   std::cout << reset_position << screen.ToString() << std::flush;
///   reset_position = screen.ResetPosition();
///
///   using namespace std::chrono_literals;
///   std::this_thread::sleep_for(0.01s);
/// }
/// ```
///
/// @return 用於將游標位置重置到開頭的字串。
std::string Screen::ResetPosition(bool clear) const {
  std::string ss;
  ss.reserve(static_cast<size_t>(dimy_) * 12);
  ResetPosition(ss, clear);
  return ss;
}

/// @brief 附加至字串以將游標位置重設回
///        畫面的開頭。
/// @param ss 要附加內容的字串。
/// @param clear 是否要清除畫面。
void Screen::ResetPosition(std::string& ss, bool clear) const {
  if (clear) {
    // 清除分支必須每次上移一列，因為每一列都需要
    // 各自的 CLEAR_LINE (\x1B[2K) 清除。無法合併成單一
    // 帶參數的游標上移操作。
    ss += '\r';       // MOVE_LEFT;
    ss += "\x1b[2K";  // CLEAR_SCREEN;
    for (int y = 1; y < dimy_; ++y) {
      ss += "\x1B[1A";  // MOVE_UP;
      ss += "\x1B[2K";  // CLEAR_LINE;
    }
  } else {
    // 非清除分支只需要將游標重新定位到左上角，
    // 因此逐列上移的過程可以合併成單一
    // 帶參數的 CSI 游標上移 (\x1B[<n>A)，每幀輸出的位元組數大幅減少。
    ss += '\r';  // MOVE_LEFT;
    if (dimy_ > 1) {
      ss += "\x1B[" + std::to_string(dimy_ - 1) + "A";  // MOVE_UP;
    }
  }
}

/// @brief 清除畫面上的所有 cell。
void Screen::Clear() {
  Surface::Clear();

  cursor_.x = dimx_ - 1;
  cursor_.y = dimy_ - 1;

  hyperlinks_ = {
      "",
  };
}

// clang-format off
void Screen::ApplyShader() {
  // 合併方框繪製字元。
  for (int y = 0; y < dimy_; ++y) {
    for (int x = 0; x < dimx_; ++x) {
      // 方框繪製字元恰好使用 3 個位元組。
      Cell& cur = FastCellAt(x, y);
      if (!ShouldAttemptAutoMerge(cur)) {
        continue;
      }

      if (x > 0) {
        Cell& left = FastCellAt(x - 1, y);
        if (ShouldAttemptAutoMerge(left)) {
          UpgradeLeftRight(left.character, cur.character);
        }
      }
      if (y > 0) {
        Cell& top = FastCellAt(x, y - 1);
        if (ShouldAttemptAutoMerge(top)) {
          UpgradeTopDown(top.character, cur.character);
        }
      }
    }
  }
}
// clang-format on

std::uint8_t Screen::RegisterHyperlink(std::string_view link) {
  for (std::size_t i = 0; i < hyperlinks_.size(); ++i) {
    if (hyperlinks_[i] == link) {
      return i;
    }
  }
  if (hyperlinks_.size() == std::numeric_limits<std::uint8_t>::max()) {
    return 0;
  }
  hyperlinks_.emplace_back(link);
  return hyperlinks_.size() - 1;
}

const std::string& Screen::Hyperlink(std::uint8_t id) const {
  if (id >= hyperlinks_.size()) {
    return hyperlinks_[0];
  }
  return hyperlinks_[id];
}

/// @brief 返回當前選擇樣式。
/// @see SetSelectionStyle
const Screen::SelectionStyle& Screen::GetSelectionStyle() const {
  return selection_style_;
}

/// @brief 設置當前選擇樣式。
/// @see GetSelectionStyle
void Screen::SetSelectionStyle(SelectionStyle decorator) {
  selection_style_ = std::move(decorator);
}

void Screen::Reserved1() {}
void Screen::Reserved2() {}
void Screen::Reserved3() {}
void Screen::Reserved4() {}
void Screen::Reserved5() {}
void Screen::Reserved6() {}
void Screen::Reserved7() {}
void Screen::Reserved8() {}

}  // namespace ftxui
