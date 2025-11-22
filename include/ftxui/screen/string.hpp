// 版权所有 2020 Arthur Sonzogni。保留所有权利。
// 此源代码受 MIT 许可的约束，MIT 许可可在
// LICENSE 文件中找到。
#ifndef FTXUI_SCREEN_STRING_HPP
#define FTXUI_SCREEN_STRING_HPP

#include <string>  // 用于 string, wstring, to_string
#include <vector>  // 用于 vector

namespace ftxui {
std::string to_string(const std::wstring& s);
std::wstring to_wstring(const std::string& s);

template <typename T>
std::wstring to_wstring(T s) {
  return to_wstring(std::to_string(s));
}

int string_width(const std::string&);

// 将字符串拆分为字形。全角字形后会插入一个空字形。
std::vector<std::string> Utf8ToGlyphs(const std::string& input);

// 将 |input| 绘制的每个单元格映射到其对应的字形。半角
// 字形占用一个单元格，全角字形占用两个单元格。
std::vector<int> CellToGlyphIndex(const std::string& input);

}  // namespace ftxui

#endif /* end of include guard: FTXUI_SCREEN_STRING_HPP */
