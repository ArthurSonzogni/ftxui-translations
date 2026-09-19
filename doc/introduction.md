@mainpage 介紹
@tableofcontents

<img src="https://github.com/ArthurSonzogni/FTXUI/assets/4759106/6925b6da-0a7e-49d9-883c-c890e1f36007" alt="Demo image"></img>

**FTXUI** 是一個簡單的跨平台 C++ 函式庫，用於基於終端的使用者介面！

# 特色
 * 函式風格。靈感來自
   [1](https://hackernoon.com/building-reactive-terminal-interfaces-in-c-d392ce34e649?gi=d9fb9ce35901)
   和 [React](https://reactjs.org/)
 * 無相依套件
 * **跨平台**。
 * 簡單優雅的語法（在我看來）
 * 鍵盤與滑鼠導覽。
 * 支援 [UTF8](https://en.wikipedia.org/wiki/UTF-8) 與 [全形字元](https://en.wikipedia.org/wiki/Halfwidth_and_fullwidth_forms)（→ 测试）
 * 支援動畫。[Demo 1](https://arthursonzogni.github.io/FTXUI/examples/?file=component/menu_underline_animated_gallery)、[Demo 2](https://arthursonzogni.github.io/FTXUI/examples/?file=component/button_style)
 * 支援繪圖。[Demo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/canvas_animated)
 * 透過[安裝](installation.html)與[快速上手](getting-started.html)指南學習。
 * 多種建置系統與套件： 
 * 良好實踐：文件、測試、模糊測試（fuzzers）、效能測試、自動化 CI、自動化封裝等等...

# 範例

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

預期輸出：

```
┌────┐┌────────────────────────────────────┐┌─────┐
│left││middle                              ││right│
└────┘└────────────────────────────────────┘└─────┘
```

# 支援的平台

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

請查看 `ftxui/component`
模組的[教學](module-component.html)。
