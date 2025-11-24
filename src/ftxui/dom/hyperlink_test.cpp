// 版權所有 2023 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，該條款可在 LICENSE 檔案中找到。
#include <gtest/gtest.h>  // for Test, EXPECT_EQ, Message, TestPartResult, TestInfo (僅限指標), TEST
#include <string>  // for allocator, string

#include "ftxui/dom/elements.hpp"  // for text, hyperlink, operator|, Element, hbox
#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/screen.hpp"  // for Screen, Pixel

namespace ftxui { // 命名空間 ftxui

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
            "\x1B]8;;https://a.com\x1B\"
            "text 1"
            "\x1B]8;;https://b.com\x1B\"
            "text 2"
            "\x1B]8;;\x1B\"
            "text 3"
            "\x1B]8;;https://c.com\x1B\"
            "text 4"
            "\x1B]8;;\x1B\"");
}

}  // 命名空間 ftxui