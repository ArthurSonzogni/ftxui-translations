// 版權所有 2022 Arthur Sonzogni。保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，該條款可在
// LICENSE 檔案中找到。
#include "ftxui/screen/color.hpp"
#include <gtest/gtest.h>
#include "ftxui/screen/terminal.hpp"

namespace ftxui {

TEST(ColorTest, 列印透明) {
  Terminal::SetColorSupport(Terminal::Color::TrueColor);
  EXPECT_EQ(Color().Print(false), "39");
  EXPECT_EQ(Color().Print(true), "49");
}

TEST(ColorTest, 列印16色) {
  Terminal::SetColorSupport(Terminal::Color::TrueColor);
  EXPECT_EQ(Color(Color::Red).Print(false), "31");
  EXPECT_EQ(Color(Color::Red).Print(true), "41");
}

TEST(ColorTest, 列印256色) {
  Terminal::SetColorSupport(Terminal::Color::TrueColor);
  EXPECT_EQ(Color(Color::DarkRed).Print(false), "38;5;52");
  EXPECT_EQ(Color(Color::DarkRed).Print(true), "48;5;52");
}

TEST(ColorTest, 列印真彩色) {
  Terminal::SetColorSupport(Terminal::Color::TrueColor);
  EXPECT_EQ(Color::RGB(1, 2, 3).Print(false), "38;2;1;2;3");
  EXPECT_EQ(Color::RGB(1, 2, 3).Print(true), "48;2;1;2;3");
}

TEST(ColorTest, 回退到256色) {
  Terminal::SetColorSupport(Terminal::Color::Palette256);
  EXPECT_EQ(Color::RGB(1, 2, 3).Print(false), "38;5;16");
}

TEST(ColorTest, 回退到16色) {
  Terminal::SetColorSupport(Terminal::Color::Palette16);
  EXPECT_EQ(Color::RGB(1, 2, 3).Print(false), "30");
}

TEST(ColorTest, 字面量) {
  Terminal::SetColorSupport(Terminal::Color::TrueColor);
  using namespace ftxui::literals;
  EXPECT_EQ(Color(0xABCDEF_rgb).Print(false), "38;2;171;205;239");
}

TEST(ColorTest, 插值) {
  Terminal::SetColorSupport(Terminal::Color::TrueColor);

  EXPECT_EQ(Color::Interpolate(0.3f, Color(), Color()).Print(false), "39");
  EXPECT_EQ(Color::Interpolate(0.3f, Color::Red, Color()).Print(false), "31");
  EXPECT_EQ(Color::Interpolate(0.7f, Color::Red, Color()).Print(false), "39");
  EXPECT_EQ(Color::Interpolate(0.3f, Color(), Color::Red).Print(false), "39");
  EXPECT_EQ(Color::Interpolate(0.7f, Color(), Color::Red).Print(false), "31");

  EXPECT_EQ(Color::Interpolate(0.3f,                       //
                               Color::RGB(1, 2, 3),        //
                               Color::RGB(244, 244, 123))  //
                .Print(false),
            "38;2;141;141;71");
  EXPECT_EQ(Color::Interpolate(0.7f,                       //
                               Color::RGB(1, 2, 3),        //
                               Color::RGB(244, 244, 123))  //
                .Print(false),
            "38;2;207;207;104");
  EXPECT_EQ(Color::Interpolate(0.7f,                       //
                               Color(Color::Red),          //
                               Color::RGB(244, 244, 123))  //
                .Print(false),
            "38;2;216;207;104");
  EXPECT_EQ(Color::Interpolate(0.7f,                       //
                               Color::RGB(244, 244, 123),  //
                               Color(Color::Plum1))        //
                .Print(false),
            "38;2;251;198;225");
}

TEST(ColorTest, 色相飽和度明度) {
  Terminal::SetColorSupport(Terminal::Color::TrueColor);
  EXPECT_EQ(Color::HSV(0, 255, 255).Print(false), "38;2;255;0;0");
}

}  // namespace ftxui
