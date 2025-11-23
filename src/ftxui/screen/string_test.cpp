// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui se trouve dans
// le fichier LICENSE.
#include "ftxui/screen/string.hpp"
#include <gtest/gtest.h>
#include <string>  // for allocator, string
#include "ftxui/screen/string_internal.hpp"

namespace ftxui {

TEST(StringTest, StringWidth) {
  // Basique :
  EXPECT_EQ(0, string_width(""));
  EXPECT_EQ(1, string_width("a"));
  EXPECT_EQ(2, string_width("ab"));
  EXPECT_EQ(1, string_width("⬤"));

  // Glyphes pleine largeur :
  EXPECT_EQ(2, string_width("测"));
  EXPECT_EQ(4, string_width("测试"));
  EXPECT_EQ(2, string_width("⚫"));
  EXPECT_EQ(2, string_width("🪐"));

  // Caractères combinants :
  EXPECT_EQ(1, string_width("ā"));
  EXPECT_EQ(1, string_width("a⃒"));
  EXPECT_EQ(1, string_width("a̗"));
  // Caractères de contrôle :
  EXPECT_EQ(0, string_width("\1"));
  EXPECT_EQ(2, string_width("a\1a"));
}

TEST(StringTest, Utf8ToGlyphs) {
  using T = std::vector<std::string>;
  // Basique :
  EXPECT_EQ(Utf8ToGlyphs(""), T({}));
  EXPECT_EQ(Utf8ToGlyphs("a"), T({"a"}));
  EXPECT_EQ(Utf8ToGlyphs("ab"), T({"a", "b"}));
  // Glyphes pleine largeur :
  EXPECT_EQ(Utf8ToGlyphs("测"), T({"测", ""}));
  EXPECT_EQ(Utf8ToGlyphs("测试"), T({"测", "", "试", ""}));
  // Caractères combinants :
  EXPECT_EQ(Utf8ToGlyphs("ā"), T({"ā"}));
  EXPECT_EQ(Utf8ToGlyphs("a⃒"), T({"a⃒"}));
  EXPECT_EQ(Utf8ToGlyphs("a̗"), T({"a̗"}));
  // Caractères de contrôle :
  EXPECT_EQ(Utf8ToGlyphs("\1"), T({}));
  EXPECT_EQ(Utf8ToGlyphs("a\1a"), T({"a", "a"}));
}

TEST(StringTest, GlyphCount) {
  // Basique :
  EXPECT_EQ(GlyphCount(""), 0);
  EXPECT_EQ(GlyphCount("a"), 1);
  EXPECT_EQ(GlyphCount("ab"), 2);
  // Glyphes pleine largeur :
  EXPECT_EQ(GlyphCount("测"), 1);
  EXPECT_EQ(GlyphCount("测试"), 2);
  // Caractères combinants :
  EXPECT_EQ(GlyphCount("ā"), 1);
  EXPECT_EQ(GlyphCount("a⃒"), 1);
  EXPECT_EQ(GlyphCount("a̗"), 1);
  // Caractères de contrôle :
  EXPECT_EQ(GlyphCount("\1"), 0);
  EXPECT_EQ(GlyphCount("a\1a"), 2);
}

TEST(StringTest, GlyphIterate) {
  // Basique :
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
  // Glyphes pleine largeur :
  EXPECT_EQ(GlyphIterate("测", 0), 0);
  EXPECT_EQ(GlyphIterate("测", 1), 3);
  EXPECT_EQ(GlyphIterate("测试", 0), 0);
  EXPECT_EQ(GlyphIterate("测试", 1), 3);
  EXPECT_EQ(GlyphIterate("测试", 2), 6);
  EXPECT_EQ(GlyphIterate("测试", 1, 3), 6);
  EXPECT_EQ(GlyphIterate("测试", 1, 0), 3);
  // Caractères combinants :
  EXPECT_EQ(GlyphIterate("ā", 0), 0);
  EXPECT_EQ(GlyphIterate("ā", 1), 3);
  EXPECT_EQ(GlyphIterate("a⃒a̗ā", 0), 0);
  EXPECT_EQ(GlyphIterate("a⃒a̗ā", 1), 4);
  EXPECT_EQ(GlyphIterate("a⃒a̗ā", 2), 7);
  EXPECT_EQ(GlyphIterate("a⃒a̗ā", 3), 10);
  // Caractères de contrôle :
  EXPECT_EQ(GlyphIterate("\1", 0), 0);
  EXPECT_EQ(GlyphIterate("\1", 1), 1);
  EXPECT_EQ(GlyphIterate("a\1a", 0), 0);
  EXPECT_EQ(GlyphIterate("a\1a", 1), 2);
  EXPECT_EQ(GlyphIterate("a\1a", 2), 3);
}

TEST(StringTest, CellToGlyphIndex) {
  // Basique :
  auto basic = CellToGlyphIndex("abc");
  ASSERT_EQ(basic.size(), 3);
  EXPECT_EQ(basic[0], 0);
  EXPECT_EQ(basic[1], 1);
  EXPECT_EQ(basic[2], 2);

  // Glyphes pleine largeur :
  auto fullwidth = CellToGlyphIndex("测试");
  ASSERT_EQ(fullwidth.size(), 4);
  EXPECT_EQ(fullwidth[0], 0);
  EXPECT_EQ(fullwidth[1], 0);
  EXPECT_EQ(fullwidth[2], 1);
  EXPECT_EQ(fullwidth[3], 1);

  // Caractères combinants :
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
  EXPECT_EQ(Utf8ToWordBreakProperty("\r"), T({}));  // FIXME
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
