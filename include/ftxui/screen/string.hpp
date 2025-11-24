// Copyright 2020 Arthur Sonzogni. 無断複写・転載を禁じます。
// このソースコードの使用は、MITライセンスに準拠しています。
// ライセンスファイルに記載されています。
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

// 文字列をグリフに分割します。全角グリフの後には空のグリフが挿入されます。
std::vector<std::string> Utf8ToGlyphs(const std::string& input);

// |input|によって描画されるすべてのセルを対応するグリフにマッピングします。半角
// グリフは1セルを占め、全角グリフは2セルを占めます。
std::vector<int> CellToGlyphIndex(const std::string& input);

}  // namespace ftxui

#endif /* end of include guard: FTXUI_SCREEN_STRING_HPP */
