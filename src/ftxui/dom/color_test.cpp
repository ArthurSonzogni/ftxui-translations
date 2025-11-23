// Copyright 2022 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée
// dans le fichier LICENSE.
#include <gtest/gtest.h>  // pour Test, EXPECT_EQ, Message, TestPartResult, TestInfo (ptr seulement), TEST
#include <string>  // pour allocator

#include "ftxui/dom/elements.hpp"  // pour operator|, text, bgcolor, color, Element
#include "ftxui/dom/node.hpp"      // pour Render
#include "ftxui/screen/color.hpp"   // pour Color, Color::Red, Color::RedLight
#include "ftxui/screen/screen.hpp"  // pour Screen, Pixel

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