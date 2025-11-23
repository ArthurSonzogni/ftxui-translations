// Copyright 2022 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <array>                      // pour array
#include <cstddef>                    // pour size_t
#include <ftxui/component/mouse.hpp>  // pour Mouse, Mouse::Left, Mouse::Pressed, Mouse::Released
#include <ftxui/dom/direction.hpp>  // pour Direction, Direction::Down, Direction::Left, Direction::Right, Direction::Up
#include <ftxui/dom/elements.hpp>   // pour frame
#include <string>                   // pour string, to_string

#include "ftxui/component/component.hpp"  // pour Slider, Vertical, operator|=
#include "ftxui/component/component_base.hpp"  // pour ComponentBase
#include "ftxui/component/event.hpp"           // pour Event, Event::ArrowDown
#include "ftxui/dom/node.hpp"                  // pour Render
#include "ftxui/screen/screen.hpp"             // pour Screen
#include "gtest/gtest.h"  // pour AssertionResult, Message, TestPartResult, Test, EXPECT_EQ, EXPECT_TRUE, EXPECT_FALSE, TEST

// NOLINTBEGIN
namespace ftxui {

namespace {
Event MousePressed(int x, int y) {
  Mouse mouse;
  mouse.button = Mouse::Left;
  mouse.motion = Mouse::Pressed;
  mouse.shift = false;
  mouse.meta = false;
  mouse.control = false;
  mouse.x = x;
  mouse.y = y;
  return Event::Mouse("jjj", mouse);
}

Event MouseReleased(int x, int y) {
  Mouse mouse;
  mouse.button = Mouse::Left;
  mouse.motion = Mouse::Released;
  mouse.shift = false;
  mouse.meta = false;
  mouse.control = false;
  mouse.x = x;
  mouse.y = y;
  return Event::Mouse("jjj", mouse);
}
}  // namespace

TEST(SliderTest, Right) {
  int updated = 0;
  int value = 50;
  auto slider = Slider<int>({
      .value = &value,
      .min = 0,
      .max = 100,
      .increment = 10,
      .direction = Direction::Right,
      .on_change = [&]() { updated++; },
  });
  Screen screen(11, 1);
  Render(screen, slider->Render());
  EXPECT_EQ(value, 50);
  EXPECT_EQ(updated, 0);
  EXPECT_TRUE(slider->OnEvent(MousePressed(3, 0)));
  EXPECT_EQ(value, 30);
  EXPECT_EQ(updated, 1);
  EXPECT_TRUE(slider->OnEvent(MousePressed(9, 0)));
  EXPECT_EQ(value, 90);
  EXPECT_EQ(updated, 2);
  EXPECT_TRUE(slider->OnEvent(MousePressed(9, 2)));
  EXPECT_EQ(value, 90);
  EXPECT_EQ(updated, 2);
  EXPECT_TRUE(slider->OnEvent(MousePressed(5, 2)));
  EXPECT_EQ(value, 50);
  EXPECT_EQ(updated, 3);
  EXPECT_TRUE(slider->OnEvent(MouseReleased(5, 2)));
  EXPECT_FALSE(slider->OnEvent(MousePressed(5, 2)));
  EXPECT_EQ(value, 50);
}

TEST(SliderTest, Left) {
  int updated = 0;
  int value = 50;
  auto slider = Slider<int>({
      .value = &value,
      .min = 0,
      .max = 100,
      .increment = 10,
      .direction = Direction::Left,
      .on_change = [&]() { updated++; },
  });
  Screen screen(11, 1);
  Render(screen, slider->Render());
  EXPECT_EQ(value, 50);
  EXPECT_EQ(updated, 0);
  EXPECT_TRUE(slider->OnEvent(MousePressed(3, 0)));
  EXPECT_EQ(value, 70);
  EXPECT_EQ(updated, 1);
  EXPECT_TRUE(slider->OnEvent(MousePressed(9, 0)));
  EXPECT_EQ(value, 10);
  EXPECT_EQ(updated, 2);
  EXPECT_TRUE(slider->OnEvent(MousePressed(9, 2)));
  EXPECT_EQ(value, 10);
  EXPECT_EQ(updated, 2);
  EXPECT_TRUE(slider->OnEvent(MousePressed(5, 2)));
  EXPECT_EQ(value, 50);
  EXPECT_EQ(updated, 3);
  EXPECT_TRUE(slider->OnEvent(MouseReleased(5, 2)));
  EXPECT_FALSE(slider->OnEvent(MousePressed(5, 2)));
  EXPECT_EQ(value, 50);
}

TEST(SliderTest, Down) {
  int updated = 0;
  int value = 50;
  auto slider = Slider<int>({
      .value = &value,
      .min = 0,
      .max = 100,
      .increment = 10,
      .direction = Direction::Down,
      .on_change = [&]() { updated++; },
  });
  Screen screen(1, 11);
  Render(screen, slider->Render());
  EXPECT_EQ(value, 50);
  EXPECT_EQ(updated, 0);
  EXPECT_TRUE(slider->OnEvent(MousePressed(0, 3)));
  EXPECT_EQ(value, 30);
  EXPECT_EQ(updated, 1);
  EXPECT_TRUE(slider->OnEvent(MousePressed(0, 9)));
  EXPECT_EQ(value, 90);
  EXPECT_EQ(updated, 2);
  EXPECT_TRUE(slider->OnEvent(MousePressed(2, 9)));
  EXPECT_EQ(value, 90);
  EXPECT_EQ(updated, 2);
  EXPECT_TRUE(slider->OnEvent(MousePressed(2, 5)));
  EXPECT_EQ(value, 50);
  EXPECT_EQ(updated, 3);
  EXPECT_TRUE(slider->OnEvent(MouseReleased(2, 5)));
  EXPECT_FALSE(slider->OnEvent(MousePressed(2, 5)));
  EXPECT_EQ(value, 50);
  EXPECT_EQ(updated, 3);
}

TEST(SliderTest, Up) {
  int updated = 0;
  int value = 50;
  auto slider = Slider<int>({
      .value = &value,
      .min = 0,
      .max = 100,
      .increment = 10,
      .direction = Direction::Up,
      .on_change = [&]() { updated++; },
  });
  Screen screen(1, 11);
  Render(screen, slider->Render());
  EXPECT_EQ(value, 50);
  EXPECT_EQ(updated, 0);
  EXPECT_TRUE(slider->OnEvent(MousePressed(0, 3)));
  EXPECT_EQ(value, 70);
  EXPECT_EQ(updated, 1);
  EXPECT_TRUE(slider->OnEvent(MousePressed(0, 9)));
  EXPECT_EQ(value, 10);
  EXPECT_EQ(updated, 2);
  EXPECT_TRUE(slider->OnEvent(MousePressed(2, 9)));
  EXPECT_EQ(value, 10);
  EXPECT_EQ(updated, 2);
  EXPECT_TRUE(slider->OnEvent(MousePressed(2, 5)));
  EXPECT_EQ(value, 50);
  EXPECT_EQ(updated, 3);
  EXPECT_TRUE(slider->OnEvent(MouseReleased(2, 5)));
  EXPECT_FALSE(slider->OnEvent(MousePressed(2, 5)));
  EXPECT_EQ(value, 50);
}

TEST(SliderTest, Focus) {
  static std::array<int, 10> values;
  auto container = Container::Vertical({});
  for (size_t i = 0; i < values.size(); ++i) {
    container->Add(Slider(std::to_string(i), &values[i]));
  }
  container |= frame;

  Screen screen(10, 3);

  Render(screen, container->Render());
  EXPECT_EQ(screen.at(0, 0), "0");  // Sélectionner 0
  EXPECT_EQ(screen.at(0, 1), "1");
  EXPECT_EQ(screen.at(0, 2), "2");

  EXPECT_TRUE(container->OnEvent(Event::ArrowDown));
  Render(screen, container->Render());
  EXPECT_EQ(screen.at(0, 0), "0");
  EXPECT_EQ(screen.at(0, 1), "1");  // Sélectionner 1
  EXPECT_EQ(screen.at(0, 2), "2");

  EXPECT_TRUE(container->OnEvent(Event::ArrowDown));
  Render(screen, container->Render());
  EXPECT_EQ(screen.at(0, 0), "1");
  EXPECT_EQ(screen.at(0, 1), "2");  // Sélectionner 2
  EXPECT_EQ(screen.at(0, 2), "3");

  EXPECT_TRUE(container->OnEvent(Event::ArrowDown));  // Sélectionner 3
  EXPECT_TRUE(container->OnEvent(Event::ArrowDown));  // Sélectionner 4
  EXPECT_TRUE(container->OnEvent(Event::ArrowDown));  // Sélectionner 5
  EXPECT_TRUE(container->OnEvent(Event::ArrowDown));  // Sélectionner 6

  EXPECT_TRUE(container->OnEvent(Event::ArrowDown));
  Render(screen, container->Render());
  EXPECT_EQ(screen.at(0, 0), "6");
  EXPECT_EQ(screen.at(0, 1), "7");  // Sélectionner 7
  EXPECT_EQ(screen.at(0, 2), "8");

  EXPECT_TRUE(container->OnEvent(Event::ArrowDown));
  Render(screen, container->Render());
  EXPECT_EQ(screen.at(0, 0), "7");
  EXPECT_EQ(screen.at(0, 1), "8");  // Sélectionner 8
  EXPECT_EQ(screen.at(0, 2), "9");

  EXPECT_TRUE(container->OnEvent(Event::ArrowDown));
  Render(screen, container->Render());
  EXPECT_EQ(screen.at(0, 0), "7");
  EXPECT_EQ(screen.at(0, 1), "8");
  EXPECT_EQ(screen.at(0, 2), "9");  // Sélectionner 9

  EXPECT_FALSE(container->OnEvent(Event::ArrowDown));
}

}  // namespace ftxui
// NOLINTEND
