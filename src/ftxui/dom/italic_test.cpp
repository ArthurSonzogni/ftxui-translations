// 版權所有 2025 Arthur Sonzogni. 保留所有權利。
// 此原始碼受 MIT 授權條款約束，詳情請參閱
// LICENSE 檔案。
#include <string>  // for allocator, string

#include "ftxui/dom/elements.hpp"   // for operator|, text, bold, Element
#include "ftxui/dom/node.hpp"       // for Render
#include "ftxui/screen/screen.hpp"  // for Screen, Pixel
#include "gtest/gtest.h"  // for Test, AssertionResult, EXPECT_TRUE, Message, TEST, TestPartResult

// NOLINTBEGIN
namespace ftxui {

TEST(ItalicTest, Basic) {
  auto element = text("text") | italic;
  Screen screen(5, 1);
  Render(screen, element);
  EXPECT_TRUE(screen.PixelAt(0, 0).italic);
}

}  // namespace ftxui
// NOLINTEND
