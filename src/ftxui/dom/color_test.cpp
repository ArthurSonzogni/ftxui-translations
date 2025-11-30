// Copyright 2022 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <gtest/gtest.h>  // para Test, EXPECT_EQ, Message, TestPartResult, TestInfo (solo ptr), TEST
#include <string>  // para allocator

#include "ftxui/dom/elements.hpp"  // para operator|, text, bgcolor, color, Element
#include "ftxui/dom/node.hpp"      // para Render
#include "ftxui/screen/color.hpp"   // para Color, Color::Red, Color::RedLight
#include "ftxui/screen/screen.hpp"  // para Screen, Pixel

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
