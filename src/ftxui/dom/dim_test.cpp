// Copyright 2022 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
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
