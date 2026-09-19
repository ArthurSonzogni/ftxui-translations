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

/// @brief Unicode文字とその関連スタイル。
///
/// @note この構造体は以前はPixelという名前でした。
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

  // スタイルを表すビットフィールド:
  bool blink : 1;
  bool bold : 1;
  bool dim : 1;
  bool italic : 1;
  bool inverted : 1;
  bool underlined : 1;
  bool underlined_double : 1;
  bool strikethrough : 1;
  bool automerge : 1;

  // セルに関連付けられたハイパーリンク。
  // 0はデフォルト値で、ハイパーリンクなしを意味します。
  // Screenのメタデータにアクセスするためのインデックスです。
  uint8_t hyperlink = 0;

  // セルに格納されているグラフェム。結合文字をサポートするために、
  // 例えばa?のように、複数のコードポイントを含む可能性があります。
  std::string character = "";

  // 色:
  Color background_color = Color::Default;
  Color foreground_color = Color::Default;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_CELL_HPP
