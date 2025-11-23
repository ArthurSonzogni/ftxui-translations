// Copyright 2022 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
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
