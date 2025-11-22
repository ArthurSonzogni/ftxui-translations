// 版权所有 2020 Arthur Sonzogni。保留所有权利。
// 本源代码的使用受 MIT 许可协议的约束，该协议可在 LICENSE 文件中找到。
#include <gtest/gtest.h>

#include "ftxui/dom/elements.hpp"   // 用于 gauge, gaugeUp
#include "ftxui/dom/node.hpp"       // 用于 Render
#include "ftxui/screen/screen.hpp"  // 用于 Screen

// NOLINTBEGIN
namespace ftxui {

TEST(GaugeTest, ZeroHorizontal) {
  auto root = gauge(0);
  Screen screen(11, 1);
  Render(screen, root);

  EXPECT_EQ("           ", screen.ToString());
}

TEST(GaugeTest, HalfHorizontal) {
  auto root = gauge(0.5);
  Screen screen(11, 1);
  Render(screen, root);

#if defined(FTXUI_MICROSOFT_TERMINAL_FALLBACK)
  EXPECT_EQ("█████▌     ", screen.ToString());
#else
  EXPECT_EQ("█████▍     ", screen.ToString());
#endif
}

TEST(GaugeTest, OneHorizontal) {
  auto root = gauge(1.0);
  Screen screen(11, 1);
  Render(screen, root);

  EXPECT_EQ("███████████", screen.ToString());
}

TEST(GaugeTest, ZeroVertical) {
  auto root = gaugeUp(0);
  Screen screen(1, 11);
  Render(screen, root);

  EXPECT_EQ(
      " \r\n"
      " \r\n"
      " \r\n"
      " \r\n"
      " \r\n"
      " \r\n"
      " \r\n"
      " \r\n"
      " \r\n"
      " \r\n"
      " ",
      screen.ToString());
}

TEST(GaugeTest, HalfVertical) {
  auto root = gaugeUp(0.5);
  Screen screen(1, 11);
  Render(screen, root);

  EXPECT_EQ(
      " \r\n"
      " \r\n"
      " \r\n"
      " \r\n"
      " \r\n"
      "▄\r\n"
      "█\r\n"
      "█\r\n"
      "█\r\n"
      "█\r\n"
      "█",
      screen.ToString());
}

TEST(GaugeTest, OneVertical) {
  auto root = gaugeUp(1.0);
  Screen screen(1, 11);
  Render(screen, root);

  EXPECT_EQ(
      "█\r\n"
      "█\r\n"
      "█\r\n"
      "█\r\n"
      "█\r\n"
      "█\r\n"
      "█\r\n"
      "█\r\n"
      "█\r\n"
      "█\r\n"
      "█",
      screen.ToString());
}

}  // namespace ftxui
// NOLINTEND
