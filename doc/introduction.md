@mainpage 简介
@tableofcontents

<img src="https://github.com/ArthurSonzogni/FTXUI/assets/4759106/6925b6da-0a7e-49d9-883c-c890e1f36007" alt="Demo image"></img>

**FTXUI** 是一个用于终端用户界面的简单跨平台 C++ 库！

# 特性
 * 函数式风格。灵感来自于
   [1](https://hackernoon.com/building-reactive-terminal-interfaces-in-c-d392ce34e649?gi=d9fb9ce35901)
   和 [React](https://reactjs.org/)
 * 无依赖
 * **跨平台**。
 * 简单优雅的语法（我个人认为）
 * 键盘和鼠标导航。
 * 支持 [UTF8](https://en.wikipedia.org/wiki/UTF-8) 和 [全角字符](https://en.wikipedia.org/wiki/Halfwidth_and_fullwidth_forms) (→ 测试)
 * 支持动画。 [演示 1](https://arthursonzogni.github.io/FTXUI/examples/?file=component/menu_underline_animated_gallery)， [演示 2](https://arthursonzogni.github.io/FTXUI/examples/?file=component/button_style)
 * 支持绘图。 [演示](https://arthursonzogni.github.io/FTXUI/examples/?file=component/canvas_animated)
 * 通过 [示例](#documentation) 和 [教程](#documentation) 学习
 * 多种构建系统和包：
 * 良好实践：文档、测试、模糊测试、性能测试、自动化 CI、自动化打包等...

# 示例

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

预期输出：

```
┌────┐┌────────────────────────────────────┐┌─────┐
│left││middle                              ││right│
└────┘└────────────────────────────────────┘└─────┘
```

# 支持的平台

- Linux
- MacOS
- Windows
- WebAssembly

<div class="section_buttons">
 
|                                  下一页 |
|--------------------------------------:|
| [开始](getting-started.html) |
 
</div>

@defgroup screen ftxui/screen

请查看 `ftxui/screen` 模块的 [教程](module-screen.html)。

@defgroup dom ftxui/dom

请查看 `ftxui/dom` 模块的 [教程](module-dom.html)。

@defgroup component ftxui/component

请查看 `ftxui/component` 模块的 [教程](module-component.html)。