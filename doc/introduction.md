@mainpage 介紹
@tableofcontents

<img src="https://github.com/ArthurSonzogni/FTXUI/assets/4759106/6925b6da-0a7e-49d9-883c-c890e1f36007" alt="Demo image"></img>

**FTXUI** 是一個簡單的跨平台 C++ 函式庫，用於基於終端的使用者介面！

# 功能
 * 函數式風格。靈感來自於
   [1](https://hackernoon.com/building-reactive-terminal-interfaces-in-c-d392ce34e649?gi=d9fb9ce35901)
   和 [React](https://reactjs.org/)
 * 無依賴
 * **跨平台**。
 * 簡潔優雅的語法（在我看來）
 * 鍵盤與滑鼠導航。
 * 支援 [UTF8](https://en.wikipedia.org/wiki/UTF-8) 和 [全形字元](https://en.wikipedia.org/wiki/Halfwidth_and_fullwidth_forms) (→ 測試)
 * 支援動畫。 [演示 1](https://arthursonzogni.github.io/FTXUI/examples/?file=component/menu_underline_animated_gallery)， [演示 2](https://arthursonzogni.github.io/FTXUI/examples/?file=component/button_style)
 * 支援繪圖。 [演示](https://arthursonzogni.github.io/FTXUI/examples/?file=component/canvas_animated)
 * 透過 [範例](#documentation) 和 [教學](#documentation) 學習
 * 多種建構系統和套件： 
 * 良好實踐：文件、測試、模糊測試、性能測試、自動化 CI、自動化打包等...

# 範例

```cpp
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

int main() {
  using namespace ftxui;

  // 建立一個包含三個文字元素的簡單文件。
  Element document = hbox({
    text("left")   | border,
    text("middle") | border | flex,
    text("right")  | border,
  });

  // 建立一個寬度全滿、高度適應文件內容的螢幕。
  auto screen = Screen::Create(
    Dimension::Full(),       // Width
    Dimension::Fit(document) // Height
  );

  // 將文件渲染到螢幕上。
  Render(screen, document);

  // 將螢幕內容印出到控制台。
  screen.Print();
}
```

預期輸出：

```
┌────┐┌────────────────────────────────────┐┌─────┐
│left││middle                              ││right│
└────┘└────────────────────────────────────┘└─────┘
```

# 支援平台

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

請查看 `ftxui/component` 模組的 [教學](module-component.html)。