// 版權所有 2020 Arthur Sonzogni。保留所有權利。
// 本源代碼受 MIT 許可證約束，詳情請參閱
// LICENSE 文件。
#ifndef FTXUI_SCREEN_STRING_HPP
#define FTXUI_SCREEN_STRING_HPP

#include <string>  // for string, wstring, to_string
#include <vector>  // for vector

namespace ftxui {
std::string to_string(const std::wstring& s);
std::wstring to_wstring(const std::string& s);

template <typename T>
std::wstring to_wstring(T s) {
  return to_wstring(std::to_string(s));
}

int string_width(const std::string&);

// 將字串分割成字形。全形字形後會插入一個空字形。
std::vector<std::string> Utf8ToGlyphs(const std::string& input);

// 將 |input| 繪製的每個儲存格映射到其對應的字形。半形字形佔用一個儲存格，
// 全形字形佔用兩個儲存格。
std::vector<int> CellToGlyphIndex(const std::string& input);

}  // namespace ftxui

#endif /* 包含守衛結束：FTXUI_SCREEN_STRING_HPP */