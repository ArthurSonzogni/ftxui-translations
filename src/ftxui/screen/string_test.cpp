// 版权所有 2020 Arthur Sonzogni。保留所有权利。
// 此源代码的使用受 MIT 许可的约束，该许可可在
// LICENSE 文件中找到。
#include "ftxui/screen/string.hpp"
#include <gtest/gtest.h>
#include <string>  // for allocator, string
#include "ftxui/screen/string_internal.hpp"

namespace ftxui {

TEST(StringTest, StringWidth) {
  // 基本：
  EXPECT_EQ(0, string_width(""));
  EXPECT_EQ(1, string_width("a"));
  EXPECT_EQ(2, string_width("ab"));
  EXPECT_EQ(1, string_width("⬤"));

  // 全角字形：
  EXPECT_EQ(2, string_width("测"));
  EXPECT_EQ(4, string_width("测试"));
  EXPECT_EQ(2, string_width("⚫"));
  EXPECT_EQ(2, string_width("🪐"));

  // 组合字符：
  EXPECT_EQ(1, string_width("ā"));
  EXPECT_EQ(1, string_width("a⃒"));
  EXPECT_EQ(1, string_width("a̗"));
  // 控制字符：
  EXPECT_EQ(0, string_width("\1"));
  EXPECT_EQ(2, string_width("a\1a"));
}

TEST(StringTest, Utf8ToGlyphs) {
  using T = std::vector<std::string>;
  // 基本：
  EXPECT_EQ(Utf8ToGlyphs(""), T({}));
  EXPECT_EQ(Utf8ToGlyphs("a"), T({"a"}));
  EXPECT_EQ(Utf8ToGlyphs("ab"), T({"a", "b"}));
  // 全角字形：
  EXPECT_EQ(Utf8ToGlyphs("测"), T({"测", ""}));
  EXPECT_EQ(Utf8ToGlyphs("测试"), T({"测", "", "试", ""}));
  // 组合字符：
  EXPECT_EQ(Utf8ToGlyphs("ā"), T({"ā"}));
  EXPECT_EQ(Utf8ToGlyphs("a⃒"), T({"a⃒"}));
  EXPECT_EQ(Utf8ToGlyphs("a̗"), T({"a̗"}));
  // 控制字符：
  EXPECT_EQ(Utf8ToGlyphs("\1"), T({}));
  EXPECT_EQ(Utf8ToGlyphs("a\1a"), T({"a", "a"}));
}

TEST(StringTest, GlyphCount) {
  // 基本：
  EXPECT_EQ(GlyphCount(""), 0);
  EXPECT_EQ(GlyphCount("a"), 1);
  EXPECT_EQ(GlyphCount("ab"), 2);
  // 全角字形：
  EXPECT_EQ(GlyphCount("测"), 1);
  EXPECT_EQ(GlyphCount("测试"), 2);
  // 组合字符：
  EXPECT_EQ(GlyphCount("ā"), 1);
  EXPECT_EQ(GlyphCount("a⃒"), 1);
  EXPECT_EQ(GlyphCount("a̗"), 1);
  // 控制字符：
  EXPECT_EQ(GlyphCount("\1"), 0);
  EXPECT_EQ(GlyphCount("a\1a"), 2);
}

TEST(StringTest, GlyphIterate) {
  // 基本：
  EXPECT_EQ(GlyphIterate("", -1), 0);
  EXPECT_EQ(GlyphIterate("", 0), 0);
  EXPECT_EQ(GlyphIterate("", 1), 0);
  EXPECT_EQ(GlyphIterate("a", 0), 0);
  EXPECT_EQ(GlyphIterate("a", 1), 1);
  EXPECT_EQ(GlyphIterate("ab", 0), 0);
  EXPECT_EQ(GlyphIterate("ab", 1), 1);
  EXPECT_EQ(GlyphIterate("ab", 2), 2);
  EXPECT_EQ(GlyphIterate("abc", 0), 0);
  EXPECT_EQ(GlyphIterate("abc", 1), 1);
  EXPECT_EQ(GlyphIterate("abc", 2), 2);
  EXPECT_EQ(GlyphIterate("abc", 3), 3);
  // 全角字形：
  EXPECT_EQ(GlyphIterate("测", 0), 0);
  EXPECT_EQ(GlyphIterate("测", 1), 3);
  EXPECT_EQ(GlyphIterate("测试", 0), 0);
  EXPECT_EQ(GlyphIterate("测试", 1), 3);
  EXPECT_EQ(GlyphIterate("测试", 2), 6);
  EXPECT_EQ(GlyphIterate("测试", 1, 3), 6);
  EXPECT_EQ(GlyphIterate("测试", 1, 0), 3);
  // 组合字符：
  EXPECT_EQ(GlyphIterate("ā", 0), 0);
  EXPECT_EQ(GlyphIterate("ā", 1), 3);
  EXPECT_EQ(GlyphIterate("a⃒a̗ā", 0), 0);
  EXPECT_EQ(GlyphIterate("a⃒a̗ā", 1), 4);
  EXPECT_EQ(GlyphIterate("a⃒a̗ā", 2), 7);
  EXPECT_EQ(GlyphIterate("a⃒a̗ā", 3), 10);
  // 控制字符：
  EXPECT_EQ(GlyphIterate("\1", 0), 0);
  EXPECT_EQ(GlyphIterate("\1", 1), 1);
  EXPECT_EQ(GlyphIterate("a\1a", 0), 0);
  EXPECT_EQ(GlyphIterate("a\1a", 1), 2);
  EXPECT_EQ(GlyphIterate("a\1a", 2), 3);
}

TEST(StringTest, CellToGlyphIndex) {
  // 基本：
  auto basic = CellToGlyphIndex("abc");
  ASSERT_EQ(basic.size(), 3);
  EXPECT_EQ(basic[0], 0);
  EXPECT_EQ(basic[1], 1);
  EXPECT_EQ(basic[2], 2);

  // 全角字形：
  auto fullwidth = CellToGlyphIndex("测试");
  ASSERT_EQ(fullwidth.size(), 4);
  EXPECT_EQ(fullwidth[0], 0);
  EXPECT_EQ(fullwidth[1], 0);
  EXPECT_EQ(fullwidth[2], 1);
  EXPECT_EQ(fullwidth[3], 1);

  // 组合字符：
  auto combining = CellToGlyphIndex("a⃒a̗ā");
  ASSERT_EQ(combining.size(), 3);
  EXPECT_EQ(combining[0], 0);
  EXPECT_EQ(combining[1], 1);
  EXPECT_EQ(combining[2], 2);
}

TEST(StringTest, Utf8ToWordBreakProperty) {
  using T = std::vector<WordBreakProperty>;
  using P = WordBreakProperty;
  EXPECT_EQ(Utf8ToWordBreakProperty("a"), T({P::ALetter}));
  EXPECT_EQ(Utf8ToWordBreakProperty("0"), T({P::Numeric}));
  EXPECT_EQ(Utf8ToWordBreakProperty("א"), T({P::Hebrew_Letter}));
  EXPECT_EQ(Utf8ToWordBreakProperty("ㇰ"), T({P::Katakana}));
  EXPECT_EQ(Utf8ToWordBreakProperty(" "), T({P::WSegSpace}));
  EXPECT_EQ(Utf8ToWordBreakProperty("\""), T({P::Double_Quote}));
  EXPECT_EQ(Utf8ToWordBreakProperty("'"), T({P::Single_Quote}));
  EXPECT_EQ(Utf8ToWordBreakProperty(":"), T({P::MidLetter}));
  EXPECT_EQ(Utf8ToWordBreakProperty("."), T({P::MidNumLet}));
  EXPECT_EQ(Utf8ToWordBreakProperty("\r"), T({}));  // 待办 (FIXME)
  EXPECT_EQ(Utf8ToWordBreakProperty("\n"), T({P::LF}));
}

TEST(StringTest, to_string) {
  EXPECT_EQ(to_string(L"hello"), "hello");
  EXPECT_EQ(to_string(L"€"), "€");
  EXPECT_EQ(to_string(L"ÿ"), "ÿ");
  EXPECT_EQ(to_string(L"߿"), "߿");
  EXPECT_EQ(to_string(L"ɰɱ"), "ɰɱ");
  EXPECT_EQ(to_string(L"«»"), "«»");
  EXPECT_EQ(to_string(L"嵰嵲嵫"), "嵰嵲嵫");
  EXPECT_EQ(to_string(L"🎅🎄"), "🎅🎄");
}

TEST(StringTest, to_wstring) {
  EXPECT_EQ(to_wstring(std::string("hello")), L"hello");
  EXPECT_EQ(to_wstring(std::string("€")), L"€");
  EXPECT_EQ(to_wstring(std::string("ÿ")), L"ÿ");
  EXPECT_EQ(to_wstring(std::string("߿")), L"߿");
  EXPECT_EQ(to_wstring(std::string("ɰɱ")), L"ɰɱ");
  EXPECT_EQ(to_wstring(std::string("«»")), L"«»");
  EXPECT_EQ(to_wstring(std::string("嵰嵲嵫")), L"嵰嵲嵫");
  EXPECT_EQ(to_wstring(std::string("🎅🎄")), L"🎅🎄");
}

}  // namespace ftxui
