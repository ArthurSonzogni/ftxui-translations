@mainpage 介紹
@tableofcontents

<img src="https://github.com/ArthurSonzogni/FTXUI/assets/4759106/6925b6da-0a7e-49d9-883c-c890e1f36007" alt="Demo image"></img>

**FTXUI** 是一個簡單的跨平台 C++ 函式庫，用於基於終端的使用者介面！

# Feature
 * Functional style. Inspired by
   [1](https://hackernoon.com/building-reactive-terminal-interfaces-in-c-d392ce34e649?gi=d9fb9ce35901)
   and [React](https://reactjs.org/)
 * No dependencies
 * **Cross platform**.
 * Simple and elegant syntax (in my opinion)
 * Keyboard & mouse navigation.
 * Support for [UTF8](https://en.wikipedia.org/wiki/UTF-8) and [fullwidth chars](https://en.wikipedia.org/wiki/Halfwidth_and_fullwidth_forms) (→ 测试)
 * Support for animations. [Demo 1](https://arthursonzogni.github.io/FTXUI/examples/?file=component/menu_underline_animated_gallery), [Demo 2](https://arthursonzogni.github.io/FTXUI/examples/?file=component/button_style)
 * Support for drawing. [Demo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/canvas_animated)
 * Learn by [installation](installation.html), and [getting started](getting-started.html) guide.
 * Multiple build system and packages: 
 * Good practices: documentation, tests, fuzzers, performance tests, automated CI, automated packaging, etc...

# Example

```cpp
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

int main() {
  using namespace ftxui;

  // Create a simple document with three text elements.
  Element document = hbox({
    text("left")   | border,
    text("middle") | border | flex,
    text("right")  | border,
  });

  // Create a screen with full width and height fitting the document.
  auto screen = Screen::Create(
    Dimension::Full(),       // Width
    Dimension::Fit(document) // Height
  );

  // Render the document onto the screen.
  Render(screen, document);

  // Print the screen to the console.
  screen.Print();
}
```

Expected output:

```
┌────┐┌────────────────────────────────────┐┌─────┐
│left││middle                              ││right│
└────┘└────────────────────────────────────┘└─────┘
```

# Supported Platforms

- Linux
- MacOS
- Windows
- WebAssembly

<div class="section_buttons">
 
|                                  下一頁 |
|--------------------------------------:|
| [Getting Started](getting-started.html) |
 
</div>

@defgroup screen ftxui/screen

請查看 `ftxui/screen` 模組的 [教學](module-screen.html)。

@defgroup dom ftxui/dom

請查看 `ftxui/dom` 模組的 [教學](module-dom.html)。

@defgroup component ftxui/component

Please check the [tutorial](module-component.html) of the `ftxui/component`
module.
