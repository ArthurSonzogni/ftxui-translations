// Droits d'auteur 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <functional>  // pour la fonction
#include <string>      // pour string, basic_string
#include <vector>      // pour vector

#include "ftxui/component/component.hpp"          // pour Menu, Toggle
#include "ftxui/component/component_base.hpp"     // pour ComponentBase
#include "ftxui/component/component_options.hpp"  // pour MenuOption
#include "ftxui/component/event.hpp"  // pour Event, Event::ArrowLeft, Event::ArrowRight, Event::Return, Event::Tab, Event::TabReverse
#include "ftxui/util/ref.hpp"         // pour Ref
#include "gtest/gtest.h"  // pour AssertionResult, Message, TestPartResult, EXPECT_EQ, Test, EXPECT_TRUE, EXPECT_FALSE, TEST

// NOLINTBEGIN
namespace ftxui {

TEST(ToggleTest, leftRightArrow) {
  std::vector<std::string> entries = {"On", "Off"};
  int selected = 0;
  auto toggle = Toggle(&entries, &selected);

  // Avec la touche flèche.
  EXPECT_EQ(selected, 0);
  toggle->OnEvent(Event::ArrowRight);
  EXPECT_EQ(selected, 1);
  toggle->OnEvent(Event::ArrowRight);
  EXPECT_EQ(selected, 1);
  toggle->OnEvent(Event::ArrowLeft);
  EXPECT_EQ(selected, 0);
  toggle->OnEvent(Event::ArrowLeft);
  EXPECT_EQ(selected, 0);

  // Avec des caractères de type vim.
  EXPECT_EQ(selected, 0);
  toggle->OnEvent(Event::Character('l'));
  EXPECT_EQ(selected, 1);
  toggle->OnEvent(Event::Character('l'));
  EXPECT_EQ(selected, 1);
  toggle->OnEvent(Event::Character('h'));
  EXPECT_EQ(selected, 0);
  toggle->OnEvent(Event::Character('h'));
  EXPECT_EQ(selected, 0);

  // Avec plus d'entrées
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

  EXPECT_FALSE(toggle->OnEvent(Event::ArrowLeft));  // Atteint l'extrême gauche.
  EXPECT_EQ(counter, 0);

  EXPECT_TRUE(toggle->OnEvent(Event::ArrowRight));  // [0] -> [1]
  EXPECT_EQ(counter, 1);
  EXPECT_TRUE(toggle->OnEvent(Event::ArrowRight));  // [1] -> [2]
  EXPECT_EQ(counter, 2);

  EXPECT_FALSE(toggle->OnEvent(Event::ArrowRight));  // Atteint l'extrême droite.
  EXPECT_EQ(counter, 2);

  EXPECT_TRUE(toggle->OnEvent(Event::ArrowLeft));  // [2] -> [1]
  EXPECT_EQ(counter, 3);
  EXPECT_TRUE(toggle->OnEvent(Event::ArrowLeft));  // [1] -> [0]
  EXPECT_EQ(counter, 4);

  EXPECT_FALSE(toggle->OnEvent(Event::ArrowLeft));  // Atteint l'extrême gauche.
  EXPECT_EQ(counter, 4);
}

TEST(ToggleTest, OnEnter) {
  std::vector<std::string> entries = {"1", "2", "3"};
  int selected = 0;
  int counter = 0;

  auto option = MenuOption::Toggle();
  option.on_enter = [&] { counter++; };
  auto toggle = Menu(&entries, &selected, option);

  EXPECT_FALSE(toggle->OnEvent(Event::ArrowLeft));  // Atteint l'extrême gauche.
  EXPECT_TRUE(toggle->OnEvent(Event::Return));
  EXPECT_EQ(counter, 1);

  EXPECT_TRUE(toggle->OnEvent(Event::ArrowRight));  // [0] -> [1]
  EXPECT_TRUE(toggle->OnEvent(Event::Return));
  EXPECT_EQ(counter, 2);
  EXPECT_TRUE(toggle->OnEvent(Event::ArrowRight));  // [1] -> [2]
  EXPECT_TRUE(toggle->OnEvent(Event::Return));
  EXPECT_EQ(counter, 3);

  EXPECT_FALSE(toggle->OnEvent(Event::ArrowRight));  // Atteint l'extrême droite.
  EXPECT_TRUE(toggle->OnEvent(Event::Return));
  EXPECT_EQ(counter, 4);

  EXPECT_TRUE(toggle->OnEvent(Event::ArrowLeft));  // [2] -> [1]
  EXPECT_TRUE(toggle->OnEvent(Event::Return));
  EXPECT_EQ(counter, 5);
  EXPECT_TRUE(toggle->OnEvent(Event::ArrowLeft));  // [1] -> [0]
  EXPECT_TRUE(toggle->OnEvent(Event::Return));
  EXPECT_EQ(counter, 6);

  EXPECT_FALSE(toggle->OnEvent(Event::ArrowLeft));  // Atteint l'extrême gauche.
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