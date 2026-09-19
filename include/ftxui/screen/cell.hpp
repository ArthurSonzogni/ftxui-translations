// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_CELL_HPP
#define FTXUI_SCREEN_CELL_HPP

#include <cstdint>                 // for uint8_t
#include <string>                  // for string, basic_string, allocator
#include "ftxui/screen/color.hpp"  // for Color, Color::Default
#include "ftxui/util/export.hpp"

namespace ftxui {

/// @brief 一個 Unicode 字元及其相關樣式。
///
/// @note 此結構先前名為 Pixel。
///
/// @ingroup screen
struct FTXUI_EXPORT(SCREEN) Cell {
  Cell()
      : blink(false),
        bold(false),
        dim(false),
        italic(false),
        inverted(false),
        underlined(false),
        underlined_double(false),
        strikethrough(false),
        automerge(false) {}

  // 表示樣式的位元欄位：
  bool blink : 1;
  bool bold : 1;
  bool dim : 1;
  bool italic : 1;
  bool inverted : 1;
  bool underlined : 1;
  bool underlined_double : 1;
  bool strikethrough : 1;
  bool automerge : 1;

  // 與該儲存格相關聯的超連結。
  // 0 是預設值，代表沒有超連結。
  // 它是用來存取 Screen 中繼資料的索引
  uint8_t hyperlink = 0;

  // 儲存在儲存格中的字位。為支援組合字元，
  // 例如：a?，這裡可能包含多個碼位。
  std::string character = "";

  // 顏色：
  Color background_color = Color::Default;
  Color foreground_color = Color::Default;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_CELL_HPP
