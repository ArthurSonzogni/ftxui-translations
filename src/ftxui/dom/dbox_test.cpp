// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui se trouve
// dans le fichier LICENSE.
#include <gtest/gtest.h>
#include <string>  // for allocator

#include "ftxui/dom/elements.hpp"  // for filler, operator|, text, border, dbox, hbox, vbox, Element
#include "ftxui/dom/node.hpp"       // for Render
#include "ftxui/screen/screen.hpp"  // for Screen

// NOLINTBEGIN
namespace ftxui {

TEST(DBoxTest, Basic) {
  auto root = dbox({
      hbox({
          text("test") | border,
          filler(),
      }),
      vbox({
          text("test") | border,
          filler(),
      }),
  });

  Screen screen(8, 4);
  Render(screen, root);
  EXPECT_EQ(screen.ToString(),
            "╭────┬─╮\r\n"
            "│test│ │\r\n"
            "╰────┴─╯\r\n"
            "╰────╯  ");
}

}  // namespace ftxui
// NOLINTEND