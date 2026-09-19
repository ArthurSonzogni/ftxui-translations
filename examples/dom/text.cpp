// Copyright 2026 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <iostream>  // for cout, endl
#include <memory>    // for shared_ptr
#include <string>    // for string, operator+

#include "ftxui/dom/elements.hpp"   // for text, Element, border, center
#include "ftxui/screen/screen.hpp"  // for Screen

int main() {
  using namespace ftxui;

  // `text()` 元素現在支援換行 `\n`。
  // 它會被渲染成多行。
  auto document =
      text(
          "FTXUI is a C++ library for terminal-based user interfaces.\n"
          "It is functional, responsive, and cross-platform.\n"
          "You can now use newlines directly within a single text() element,\n"
          "making it easier to display multi-line strings without needing\n"
          "to manually split them into a vbox() of multiple text() elements.") |
      border | center;

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  std::cout << screen.ToString() << std::endl;

  return 0;
}
