// Copyright 2025 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.

#include "ftxui/component/component.hpp"  // for Horizontal, Vertical, Button, Tab
#include "ftxui/component/component_base.hpp"  // for ComponentBase, Component
#include "ftxui/component/event.hpp"  // for Event, Event::Tab, Event::TabReverse, Event::ArrowDown, Event::ArrowLeft, Event::ArrowRight, Event::ArrowUp
#include "gtest/gtest.h"  // for AssertionResult, Message, TestPartResult, EXPECT_EQ, EXPECT_FALSE, Test, EXPECT_TRUE, TEST

namespace ftxui {

TEST(DropdownTest, Empty) {
  std::vector<std::string> list = {};
  int index = 0;
  auto dropdown = Dropdown(list, &index);

  dropdown->OnEvent(Event::Return);

  auto screen = Screen(8, 8);
  auto document = dropdown->Render();
  Render(screen, document);

  EXPECT_EQ(screen.ToString(),
            "╭──────╮\r\n"
            "│↓     │\r\n"
            "├──────┤\r\n"
            "│      │\r\n"
            "│      │\r\n"
            "│      │\r\n"
            "│      │\r\n"
            "╰──────╯");
}

}  // namespace ftxui