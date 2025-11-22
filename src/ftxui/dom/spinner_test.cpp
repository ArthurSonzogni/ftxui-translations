// 版权所有 2022 Arthur Sonzogni。保留所有权利。
// 本源代码的使用受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
#include <gtest/gtest.h>

#include "ftxui/dom/elements.hpp"   // for spinner
#include "ftxui/dom/node.hpp"       // for Render
#include "ftxui/screen/screen.hpp"  // for Screen

// NOLINTBEGIN
namespace ftxui {

TEST(SpinnerTest, Spinner1) {
  auto element = spinner(1, 0);
  Screen screen(4, 1);
  Render(screen, element);
  EXPECT_EQ(screen.ToString(), ".   ");
}

TEST(SpinnerTest, Spinner2) {
  auto element = spinner(1, 1);
  Screen screen(4, 1);
  Render(screen, element);
  EXPECT_EQ(screen.ToString(), "..  ");
}

TEST(SpinnerTest, Spinner3) {
  auto element = spinner(1, 2);
  Screen screen(4, 1);
  Render(screen, element);
  EXPECT_EQ(screen.ToString(), "... ");
}

TEST(SpinnerTest, Spinner4) {
  auto element = spinner(1, 3);
  Screen screen(4, 1);
  Render(screen, element);
  EXPECT_EQ(screen.ToString(), ".   ");
}

}  // namespace ftxui
// NOLINTEND
