// 版权所有 2022 Arthur Sonzogni。保留所有权利。
// 本源代码受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
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
