// Copyright 2022 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <string>  // for allocator, string

#include "ftxui/dom/elements.hpp"   // for operator|, text, underlined, Element
#include "ftxui/dom/node.hpp"       // for Render
#include "ftxui/screen/screen.hpp"  // for Screen, Pixel
#include "gtest/gtest.h"  // for Test, AssertionResult, EXPECT_TRUE, Message, TEST, TestPartResult

// NOLINTBEGIN
namespace ftxui {

TEST(UnderlinedTest, Basic) {
  auto element = text("text") | underlined;
  Screen screen(5, 1);
  Render(screen, element);
  EXPECT_TRUE(screen.PixelAt(0, 0).underlined);
}

}  // namespace ftxui
// NOLINTEND
