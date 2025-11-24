// Copyright 2024 Arthur Sonzogni. All rights reserved.
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱 LICENSE 檔案。
#ifndef FTXUI_SCREEN_PIXEL_HPP
#define FTXUI_SCREEN_PIXEL_HPP

#include <cstdint>                 // for uint8_t
#include <string>                  // for string, basic_string, allocator
#include "ftxui/screen/color.hpp"  // for Color, Color::Default

namespace ftxui {

/// @brief 一個 Unicode 字元及其相關樣式。
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

  // 表示樣式的位元欄位:
  bool blink : 1;
  bool bold : 1;
  bool dim : 1;
  bool italic : 1;
  bool inverted : 1;
  bool underlined : 1;
  bool underlined_double : 1;
  bool strikethrough : 1;
  bool automerge : 1;

  // 與像素關聯的超連結。
  // 0 是預設值，表示沒有超連結。
  // 它是用於存取螢幕元資料的索引。
  uint8_t hyperlink = 0;

  // 儲存在像素中的字素。為了支援組合字元，
  // 例如：á，這可能包含多個碼點。
  std::string character = "";

  // 顏色:
  Color background_color = Color::Default;
  Color foreground_color = Color::Default;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_PIXEL_HPP