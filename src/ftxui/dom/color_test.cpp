// 版权所有 2022 Arthur Sonzogni。保留所有权利。
// 本源代码受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
#include <gtest/gtest.h>  // 用于 Test, EXPECT_EQ, Message, TestPartResult, TestInfo (仅指针), TEST
#include <string>  // 用于 allocator

#include "ftxui/dom/elements.hpp"  // 用于 operator|, text, bgcolor, color, Element
#include "ftxui/dom/node.hpp"      // 用于 Render
#include "ftxui/screen/color.hpp"   // 用于 Color, Color::Red, Color::RedLight
#include "ftxui/screen/screen.hpp"  // 用于 Screen, Pixel

// NOLINTBEGIN
namespace ftxui {

TEST(ColorTest, Foreground) {
  auto element = text("text") | color(Color::Red);
  Screen screen(5, 1);
  Render(screen, element);
  EXPECT_EQ(screen.PixelAt(0, 0).foreground_color, Color::Red);
  EXPECT_EQ(screen.PixelAt(0, 0).background_color, Color());
}

TEST(ColorTest, Background) {
  auto element = text("text") | bgcolor(Color::Red);
  Screen screen(5, 1);
  Render(screen, element);
  EXPECT_EQ(screen.PixelAt(0, 0).foreground_color, Color());
  EXPECT_EQ(screen.PixelAt(0, 0).background_color, Color::Red);
}

}  // namespace ftxui
// NOLINTEND
