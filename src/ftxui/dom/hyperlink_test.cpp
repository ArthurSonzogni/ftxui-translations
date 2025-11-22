// 版权所有 2023 Arthur Sonzogni. 保留所有权利。
// 本源代码受 MIT 许可证约束，可在 LICENSE 文件中找到。
#include <gtest/gtest.h>  // 用于 Test, EXPECT_EQ, Message, TestPartResult, TestInfo (仅限指针), TEST
#include <string>  // 用于 allocator, string

#include "ftxui/dom/elements.hpp"  // 用于 text, hyperlink, operator|, Element, hbox
#include "ftxui/dom/node.hpp"      // 用于 Render
#include "ftxui/screen/screen.hpp"  // 用于 Screen, Pixel

namespace ftxui {

TEST(HyperlinkTest, Basic) {
  auto element = hbox({
      text("text 1") | hyperlink("https://a.com"),
      text("text 2") | hyperlink("https://b.com"),
      text("text 3"),
      text("text 4") | hyperlink("https://c.com"),
  });

  Screen screen(6 * 4, 1);
  Render(screen, element);

  EXPECT_EQ(screen.PixelAt(0, 0).hyperlink, 1u);
  EXPECT_EQ(screen.PixelAt(5, 0).hyperlink, 1u);
  EXPECT_EQ(screen.PixelAt(6, 0).hyperlink, 2u);
  EXPECT_EQ(screen.PixelAt(11, 0).hyperlink, 2u);

  std::string output = screen.ToString();
  EXPECT_EQ(output,
            "\x1B]8;;https://a.com\x1B\\"
            "text 1"
            "\x1B]8;;https://b.com\x1B\\"
            "text 2"
            "\x1B]8;;\x1B\\"
            "text 3"
            "\x1B]8;;https://c.com\x1B\\"
            "text 4"
            "\x1B]8;;\x1B\\");
}

}  // namespace ftxui
