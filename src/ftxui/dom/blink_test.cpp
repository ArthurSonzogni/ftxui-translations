// 版权所有 2022 Arthur Sonzogni. 保留所有权利。
// 本源代码受 MIT 许可证的约束，可在 LICENSE 文件中找到。
#include <string>  // for allocator, string

#include "ftxui/dom/elements.hpp"   // for operator|, text, blink, Element
#include "ftxui/dom/node.hpp"       // for Render
#include "ftxui/screen/screen.hpp"  // for Screen, Pixel
#include "gtest/gtest.h"  // for Test, AssertionResult, EXPECT_TRUE, Message, TEST, TestPartResult

// NOLINTBEGIN
namespace ftxui {

TEST(BlinkTest, Basic) {
  auto element = text("text") | blink;
  Screen screen(5, 1);
  Render(screen, element);
  EXPECT_TRUE(screen.PixelAt(0, 0).blink);
}

}  // namespace ftxui
// NOLINTEND
