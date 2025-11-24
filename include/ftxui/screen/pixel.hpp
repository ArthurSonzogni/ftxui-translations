// Copyright 2024 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスによって管理されています。
#ifndef FTXUI_SCREEN_PIXEL_HPP
#define FTXUI_SCREEN_PIXEL_HPP

#include <cstdint>                 // for uint8_t
#include <string>                  // for string, basic_string, allocator
#include "ftxui/screen/color.hpp"  // for Color, Color::Default

namespace ftxui {

/// @brief Unicode文字とそれに関連付けられたスタイル。
/// @ingroup screen
struct Pixel {
  Pixel()
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

  // ピクセルに関連付けられたハイパーリンク。
  // 0はデフォルト値で、ハイパーリンクがないことを意味します。
  // これは画面メタデータにアクセスするためのインデックスです。
  uint8_t hyperlink = 0;

  // ピクセルに格納されている書記素。a? のように結合文字をサポートするために、
  // 複数のコードポイントを含む可能性があります。
  std::string character = "";

  // 色:
  Color background_color = Color::Default;
  Color foreground_color = Color::Default;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_PIXEL_HPP