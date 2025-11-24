// Copyright 2020 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、以下で見つけることができるMITライセンスによって管理されています。
// LICENSEファイル。
#include <ftxui/screen/screen.hpp>  // for Screen
#include <iostream>                 // for endl, cout, ostream
#include <memory>                   // for allocator

#include "ftxui/dom/elements.hpp"  // for text, hbox, separatorDouble, separatorHeavy, separatorLight, vbox, operator|, Element, Fit, borderDouble, borderHeavy, borderLight
#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/box.hpp"    // for ftxui

int main() {
  using namespace ftxui;

  auto document = vbox({
      vbox({
          text("separatorLight"),
          separatorLight(),
          hbox(text("left"), separatorLight(), text("right")),
      }) | borderLight,

      vbox({
          text("separatorDashed"),
          separatorDashed(),
          hbox(text("left"), separatorDashed(), text("right")),
      }) | borderDashed,

      vbox({
          text("separatorHeavy"),
          separatorHeavy(),
          hbox(text("left"), separatorHeavy(), text("right")),
      }) | borderHeavy,

      vbox({
          text("separatorDouble"),
          separatorDouble(),
          hbox(text("left"), separatorDouble(), text("right")),
      }) | borderDouble,
  });

  auto screen =
      Screen::Create(Dimension::Fit(document), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();
  std::cout << std::endl;
}
