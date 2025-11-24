// 著作権 2022 Arthur Sonzogni. 全著作権所有。
// このソースコードは、LICENSE ファイルにある MIT ライセンスに従って使用されます。
#include <string>  // for allocator, string

#include "ftxui/dom/elements.hpp"   // for operator|, text, bold, Element
#include "ftxui/dom/node.hpp"       // for Render
#include "ftxui/screen/screen.hpp"  // for Screen, Pixel
#include "gtest/gtest.h"  // for Test, AssertionResult, EXPECT_TRUE, Message, TEST, TestPartResult

// NOLINTBEGIN
namespace ftxui {

TEST(BoldTest, Basic) {
  auto element = text("text") | bold;
  Screen screen(5, 1);
  Render(screen, element);
  EXPECT_TRUE(screen.PixelAt(0, 0).bold);
}

}  // namespace ftxui
// NOLINTEND