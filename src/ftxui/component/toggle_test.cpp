// 版权所有 2020 Arthur Sonzogni。保留所有权利。
// 本源代码的使用受 MIT 许可证的约束，该许可证可在
// LICENSE 文件中找到。
#include <functional>  // 用于函数
#include <string>      // 用于字符串，基本字符串
#include <vector>      // 用于向量

#include "ftxui/component/component.hpp"          // 用于菜单，切换
#include "ftxui/component/component_base.hpp"     // 用于组件基类
#include "ftxui/component/component_options.hpp"  // 用于菜单选项
#include "ftxui/component/event.hpp"  // 用于事件，事件::左箭头，事件::右箭头，事件::回车，事件::Tab，事件::反向Tab
#include "ftxui/util/ref.hpp"         // 用于引用
#include "gtest/gtest.h"  // 用于断言结果，消息，测试结果部分，EXPECT_EQ，测试，EXPECT_TRUE，EXPECT_FALSE，TEST

// NOLINTBEGIN
namespace ftxui {

TEST(ToggleTest, leftRightArrow) {
  std::vector<std::string> entries = {"On", "Off"};
  int selected = 0;
  auto toggle = Toggle(&entries, &selected);

  // 使用箭头键。
  EXPECT_EQ(selected, 0);
  toggle->OnEvent(Event::ArrowRight);
  EXPECT_EQ(selected, 1);
  toggle->OnEvent(Event::ArrowRight);
  EXPECT_EQ(selected, 1);
  toggle->OnEvent(Event::ArrowLeft);
  EXPECT_EQ(selected, 0);
  toggle->OnEvent(Event::ArrowLeft);
  EXPECT_EQ(selected, 0);

  // 使用Vim式字符。
  EXPECT_EQ(selected, 0);
  toggle->OnEvent(Event::Character('l'));
  EXPECT_EQ(selected, 1);
  toggle->OnEvent(Event::Character('l'));
  EXPECT_EQ(selected, 1);
  toggle->OnEvent(Event::Character('h'));
  EXPECT_EQ(selected, 0);
  toggle->OnEvent(Event::Character('h'));
  EXPECT_EQ(selected, 0);

  // 更多条目
  entries = {"1", "2", "3"};
  EXPECT_EQ(selected, 0);
  toggle->OnEvent(Event::ArrowRight);
  EXPECT_EQ(selected, 1);
  toggle->OnEvent(Event::ArrowRight);
  EXPECT_EQ(selected, 2);
  toggle->OnEvent(Event::ArrowRight);
  EXPECT_EQ(selected, 2);
  toggle->OnEvent(Event::ArrowLeft);
  EXPECT_EQ(selected, 1);
  toggle->OnEvent(Event::ArrowLeft);
  EXPECT_EQ(selected, 0);
  toggle->OnEvent(Event::ArrowLeft);
  EXPECT_EQ(selected, 0);
}

TEST(ToggleTest, Tab) {
  std::vector<std::string> entries = {"1", "2", "3"};
  int selected = 0;
  auto toggle = Toggle(&entries, &selected);

  EXPECT_EQ(selected, 0);
  toggle->OnEvent(Event::Tab);
  EXPECT_EQ(selected, 1);
  toggle->OnEvent(Event::Tab);
  EXPECT_EQ(selected, 2);
  toggle->OnEvent(Event::Tab);
  EXPECT_EQ(selected, 0);
  toggle->OnEvent(Event::Tab);
  EXPECT_EQ(selected, 1);
  toggle->OnEvent(Event::Tab);
  EXPECT_EQ(selected, 2);
  toggle->OnEvent(Event::TabReverse);
  EXPECT_EQ(selected, 1);
  toggle->OnEvent(Event::TabReverse);
  EXPECT_EQ(selected, 0);
  toggle->OnEvent(Event::TabReverse);
  EXPECT_EQ(selected, 2);
  toggle->OnEvent(Event::TabReverse);
  EXPECT_EQ(selected, 1);
  toggle->OnEvent(Event::TabReverse);
}

TEST(ToggleTest, OnChange) {
  std::vector<std::string> entries = {"1", "2", "3"};
  int selected = 0;
  int counter = 0;
  auto option = MenuOption::Toggle();
  option.on_change = [&] { counter++; };

  auto toggle = Menu(&entries, &selected, option);

  EXPECT_FALSE(toggle->OnEvent(Event::ArrowLeft));  // 已到达最左侧。
  EXPECT_EQ(counter, 0);

  EXPECT_TRUE(toggle->OnEvent(Event::ArrowRight));  // [0] -> [1]
  EXPECT_EQ(counter, 1);
  EXPECT_TRUE(toggle->OnEvent(Event::ArrowRight));  // [1] -> [2]
  EXPECT_EQ(counter, 2);

  EXPECT_FALSE(toggle->OnEvent(Event::ArrowRight));  // 已到达最右侧。
  EXPECT_EQ(counter, 2);

  EXPECT_TRUE(toggle->OnEvent(Event::ArrowLeft));  // [2] -> [1]
  EXPECT_EQ(counter, 3);
  EXPECT_TRUE(toggle->OnEvent(Event::ArrowLeft));  // [1] -> [0]
  EXPECT_EQ(counter, 4);

  EXPECT_FALSE(toggle->OnEvent(Event::ArrowLeft));  // 已到达最左侧。
  EXPECT_EQ(counter, 4);
}

TEST(ToggleTest, OnEnter) {
  std::vector<std::string> entries = {"1", "2", "3"};
  int selected = 0;
  int counter = 0;

  auto option = MenuOption::Toggle();
  option.on_enter = [&] { counter++; };
  auto toggle = Menu(&entries, &selected, option);

  EXPECT_FALSE(toggle->OnEvent(Event::ArrowLeft));  // 已到达最左侧。
  EXPECT_TRUE(toggle->OnEvent(Event::Return));
  EXPECT_EQ(counter, 1);

  EXPECT_TRUE(toggle->OnEvent(Event::ArrowRight));  // [0] -> [1]
  EXPECT_TRUE(toggle->OnEvent(Event::Return));
  EXPECT_EQ(counter, 2);
  EXPECT_TRUE(toggle->OnEvent(Event::ArrowRight));  // [1] -> [2]
  EXPECT_TRUE(toggle->OnEvent(Event::Return));
  EXPECT_EQ(counter, 3);

  EXPECT_FALSE(toggle->OnEvent(Event::ArrowRight));  // 已到达最右侧。
  EXPECT_TRUE(toggle->OnEvent(Event::Return));
  EXPECT_EQ(counter, 4);

  EXPECT_TRUE(toggle->OnEvent(Event::ArrowLeft));  // [2] -> [1]
  EXPECT_TRUE(toggle->OnEvent(Event::Return));
  EXPECT_EQ(counter, 5);
  EXPECT_TRUE(toggle->OnEvent(Event::ArrowLeft));  // [1] -> [0]
  EXPECT_TRUE(toggle->OnEvent(Event::Return));
  EXPECT_EQ(counter, 6);

  EXPECT_FALSE(toggle->OnEvent(Event::ArrowLeft));  // 已到达最左侧。
  EXPECT_TRUE(toggle->OnEvent(Event::Return));
  EXPECT_EQ(counter, 7);
}

TEST(ToggleTest, RemoveEntries) {
  int focused_entry = 0;
  int selected = 0;
  std::vector<std::string> entries = {"1", "2", "3"};
  auto option = MenuOption::Toggle();
  option.focused_entry = &focused_entry;
  auto toggle = Menu(&entries, &selected, option);

  EXPECT_EQ(selected, 0);
  EXPECT_EQ(focused_entry, 0);

  toggle->OnEvent(Event::ArrowRight);
  toggle->OnEvent(Event::ArrowRight);

  EXPECT_EQ(selected, 2);
  EXPECT_EQ(focused_entry, 2);

  entries.resize(2);

  EXPECT_EQ(selected, 2);
  EXPECT_EQ(focused_entry, 2);

  (void)toggle->Render();

  EXPECT_EQ(selected, 1);
  EXPECT_EQ(focused_entry, 1);
}

}  // namespace ftxui
// NOLINTEND
