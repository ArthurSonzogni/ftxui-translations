// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <gtest/gtest.h>

#include "ftxui/dom/elements.hpp"   // for gauge, gaugeUp
#include "ftxui/dom/node.hpp"       // for Render
#include "ftxui/screen/screen.hpp"  // for Screen

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
