// Copyright 2025 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、以下で見つけることができるMITライセンスによって管理されています。
// LICENSEファイル。

#include "ftxui/component/component.hpp"  // Horizontal, Vertical, Button, Tab用
#include "ftxui/component/component_base.hpp"  // ComponentBase, Component用
#include "ftxui/component/event.hpp"  // Event、Event::Tab、Event::TabReverse、Event::ArrowDown、Event::ArrowLeft、Event::ArrowRight、Event::ArrowUp用
#include "gtest/gtest.h"  // AssertionResult、Message、TestPartResult、EXPECT_EQ、EXPECT_FALSE、Test、EXPECT_TRUE、TEST用

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