// Copyright 2023 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <gtest/gtest.h>  // para Test, EXPECT_EQ, Message, TestPartResult, TestInfo (solo puntero), TEST
#include <ftxui/dom/linear_gradient.hpp>  // para LinearGradient::Stop, LinearGradient

#include "ftxui/dom/elements.hpp"  // para operator|, text, bgcolor, color, Element
#include "ftxui/dom/node.hpp"      // para Render
#include "ftxui/screen/color.hpp"   // para Color, Color::RedLight, Color::Red
#include "ftxui/screen/screen.hpp"  // para Screen, Pixel

// NOLINTBEGIN
namespace ftxui {

TEST(ColorTest, API_default) {
  LinearGradient gradient;
  EXPECT_EQ(gradient.angle, 0);
  EXPECT_EQ(gradient.stops.size(), 0);
}

TEST(ColorTest, API_builder) {
  auto gradient = LinearGradient()  //
                      .Angle(45)
                      .Stop(Color::Red)
                      .Stop(Color::RedLight, 0.5)
                      .Stop(Color::RedLight);
  EXPECT_EQ(gradient.angle, 45);
  EXPECT_EQ(gradient.stops.size(), 3);
  EXPECT_EQ(gradient.stops[0].color, Color::Red);
  EXPECT_EQ(gradient.stops[0].position, std::nullopt);
  EXPECT_EQ(gradient.stops[1].color, Color::RedLight);
  EXPECT_EQ(gradient.stops[1].position, 0.5);
  EXPECT_EQ(gradient.stops[2].color, Color::RedLight);
  EXPECT_EQ(gradient.stops[2].position, std::nullopt);
}

TEST(ColorTest, API_constructor_bicolor) {
  auto gradient = LinearGradient(Color::Red, Color::RedLight);
  EXPECT_EQ(gradient.angle, 0);
  EXPECT_EQ(gradient.stops.size(), 2);
  EXPECT_EQ(gradient.stops[0].color, Color::Red);
  EXPECT_EQ(gradient.stops[0].position, std::nullopt);
  EXPECT_EQ(gradient.stops[1].color, Color::RedLight);
  EXPECT_EQ(gradient.stops[1].position, std::nullopt);
}

TEST(ColorTest, API_constructor_bicolor_angle) {
  auto gradient = LinearGradient(45, Color::Red, Color::RedLight);
  EXPECT_EQ(gradient.angle, 45);
  EXPECT_EQ(gradient.stops.size(), 2);
  EXPECT_EQ(gradient.stops[0].color, Color::Red);
  EXPECT_EQ(gradient.stops[0].position, std::nullopt);
  EXPECT_EQ(gradient.stops[1].color, Color::RedLight);
  EXPECT_EQ(gradient.stops[1].position, std::nullopt);
}

TEST(ColorTest, GradientForeground) {
  auto element =
      text("text") | color(LinearGradient(Color::RedLight, Color::Red));
  Screen screen(5, 1);
  Render(screen, element);

  Color gradient_begin = Color::Interpolate(0, Color::RedLight, Color::Red);
  Color gradient_end = Color::Interpolate(1, Color::RedLight, Color::Red);

  EXPECT_EQ(screen.PixelAt(0, 0).foreground_color, gradient_begin);
  EXPECT_EQ(screen.PixelAt(0, 0).background_color, Color());

  EXPECT_EQ(screen.PixelAt(4, 0).foreground_color, gradient_end);
  EXPECT_EQ(screen.PixelAt(4, 0).background_color, Color());
}

TEST(ColorTest, GradientBackground) {
  auto element =
      text("text") | bgcolor(LinearGradient(Color::RedLight, Color::Red));
  Screen screen(5, 1);
  Render(screen, element);

  Color gradient_begin = Color::Interpolate(0, Color::RedLight, Color::Red);
  Color gradient_end = Color::Interpolate(1, Color::RedLight, Color::Red);

  EXPECT_EQ(screen.PixelAt(0, 0).foreground_color, Color());
  EXPECT_EQ(screen.PixelAt(0, 0).background_color, gradient_begin);

  EXPECT_EQ(screen.PixelAt(4, 0).foreground_color, Color());
  EXPECT_EQ(screen.PixelAt(4, 0).background_color, gradient_end);
}

}  // namespace ftxui
// NOLINTEND
