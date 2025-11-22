// 版权所有 2022 Arthur Sonzogni。保留所有权利。
// 本源代码的使用受 MIT 许可证的管辖，该许可证可在
// LICENSE 文件中找到。
#include <string>  // for allocator, string

#include "ftxui/dom/elements.hpp"   // for operator|, text, dim, Element
#include "ftxui/dom/node.hpp"       // for Render
#include "ftxui/screen/screen.hpp"  // for Screen, Pixel
#include "gtest/gtest.h"  // for Test, AssertionResult, EXPECT_TRUE, Message, TEST, TestPartResult

// NOLINTBEGIN
namespace ftxui {

TEST(DimTest, Basic) {
  auto element = text("text") | dim;
  Screen screen(5, 1);
  Render(screen, element);
  EXPECT_TRUE(screen.PixelAt(0, 0).dim);
}

}  // namespace ftxui
// NOLINTEND
