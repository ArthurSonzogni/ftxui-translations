// 版权所有 2024 Arthur Sonzogni。保留所有权利。
// 本源代码的使用受 MIT 许可协议的约束，该协议可在 LICENSE 文件中找到。
#ifndef FTXUI_SCREEN_PIXEL_HPP
#define FTXUI_SCREEN_PIXEL_HPP

#include <cstdint>                 // for uint8_t
#include <string>                  // for string, basic_string, allocator
#include "ftxui/screen/color.hpp"  // for Color, Color::Default

namespace ftxui {

/// @brief 一个 Unicode 字符及其相关样式。
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

  // 表示样式的位字段：
  bool blink : 1; // 闪烁
  bool bold : 1; // 粗体
  bool dim : 1; // 暗淡
  bool italic : 1; // 斜体
  bool inverted : 1; // 反转
  bool underlined : 1; // 下划线
  bool underlined_double : 1; // 双下划线
  bool strikethrough : 1; // 删除线
  bool automerge : 1; // 自动合并

  // 与像素关联的超链接。
  // 0 是默认值，表示没有超链接。
  // 它是访问屏幕元数据的索引
  uint8_t hyperlink = 0;

  // 存储在像素中的字素。为了支持组合字符，
  // 例如：å，这可能包含多个码点。
  std::string character = "";

  // 颜色：
  Color background_color = Color::Default; // 背景颜色
  Color foreground_color = Color::Default; // 前景颜色
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_PIXEL_HPP
