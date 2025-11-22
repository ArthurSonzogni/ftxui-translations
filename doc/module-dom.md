@page module-dom ftxui / dom
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595347.png)

该模块定义了一组分层的 `ftxui::Element`。元素管理布局，并且可以响应终端尺寸变化。请注意以下示例，其中此模块用于创建具有多个运算符的简单布局：

@subpage module-dom-examples 部分提供了一个示例集合。

**示例：**
```cpp
namespace ftxui {
    ...

// Define the document
Element document = vbox({
  text("The window") | bold | color(Color::Blue),
  gauge(0.5)
  text("The footer")
});

// Add a border, by calling the `ftxui::border` decorator function.
document = border(document);

// Add another border, using the pipe operator.
document = document | border.

// Add another border, using the |= operator.
document |= border

...
}
```

**元素列表**

所有元素都已包含在内，可以通过包含相应的头文件来访问：
```cpp
#include <ftxui/dom/elements.hpp>
```

\include{strip} "ftxui/dom/elements.hpp"

# text # {#dom-text}

最简单的组件。它显示一段文本。
```cpp
text("I am a piece of text");
```
```bash
I am a piece of text.
```

# vtext {#dom-vtext}

与 `ftxui::text` 相同，但垂直显示。

代码：
```cpp
终端输出：```bash
H
E
L
L
O
```

# paragraph {#dom-paragraph}

类似于 `ftxui::text`，但单个单词会根据其容器的宽度在多行上换行。

示例代码：
```cpp
paragraph("A very long text")
```

![ezgif com-gif-maker (4)](https://user-images.githubusercontent.com/4759106/147251370-983a06e7-6f41-4113-92b8-942f43d34d06.gif)

有关更详细的示例，请参阅[详细示例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2paragraph_8cpp-example.html)。Paragraph 还包括许多其他变体，如下所示：
```cpp
namespace ftxui {
    Element paragraph(std::string text);
    Element paragraphAlignLeft(std::string text);
    Element paragraphAlignRight(std::string text);
    Element paragraphAlignCenter(std::string text);
    Element paragraphAlignJustify(std::string text);
}
```


# border {#dom-border}

在元素周围添加边框。

代码：
```cpp
终端输出：```bash
┌───────────┐
│The element│
└───────────┘
```

> [!note]
> 你可以使用管道操作符实现相同的行为。
>
> 代码：
> ```cpp
> text("The element") | border
> ```

Border 还有多种样式，如下所示：
```cpp
namespace ftxui {
    Element border(Element);
    Element borderLight(Element);
    Element borderHeavy(Element);
    Element borderDouble(Element);
    Element borderRounded(Element);
    Element borderEmpty(Element);
    Decorator borderStyled(BorderStyle);
    Decorator borderWith(Pixel);
}
```


# window # {#dom-window}

# window # {#dom-window}

`ftxui::window` 是一个 `ftxui::border`，但带有一个额外的标题。要在一个元素周围添加一个窗口，请将其包装并指定一个字符串作为标题。
代码：
```cpp
window("The window", text("The element"))
```

Terminal output:
```bash
┌The window─┐
│The element│
└───────────┘
```

# separator {#dom-separator}

显示垂直/水平线以在视觉上将容器的内容分成两部分。

代码：
```cpp
border(
  hbox({
    text("Left"), 
    separator(),
    text("Right")
终端输出：
```bash
┌────┬─────┐
│left│right│
└────┴─────┘
```


分隔符有多种样式，如下所示：
```cpp
namespace ftxui {
    Element separator(void);
    Element separatorLight();
    Element separatorHeavy();
    Element separatorDouble();
    Element separatorEmpty();
    Element separatorStyled(BorderStyle);
    Element separator(Pixel);
    Element separatorCharacter(std::string);
    Element separatorHSelector(float left,
                               float right,
                               Color background,
                               Color foreground);
    Element separatorVSelector(float up,
                               float down,
                               Color background,
                               Color foreground);
}
```

# gauge {#dom-gauge}

这是一个表示进度比率的视觉元素。

代码：
```cpp
终端输出：
```bash
┌────────────────────────────────────────────────────────────────────────────┐
│██████████████████████████████████████                                      │
└────────────────────────────────────────────────────────────────────────────┘
```

量规可以多种方向显示，如下所示：
```cpp
namespace {
    Element gauge(float ratio);
    Element gaugeLeft(float ratio);
    Element gaugeRight(float ratio);
    Element gaugeUp(float ratio);
    Element gaugeDown(float ratio);
    Element gaugeDirection(float ratio, GaugeDirection);
}
```

# graph {#dom-graph}

@htmlonly
<script id="asciicast-223726" src="https://asciinema.org/a/223726.js" async></script>
@endhtmlonly

参见：
```cpp
Element graph(GraphFunction);
```

# Colors {#dom-colors}

大多数终端控制台可以显示彩色文本和彩色背景。FTXUI 支持所有调色板：

颜色[画廊]：
![image](https://user-images.githubusercontent.com/4759106/147248595-04c7245a-5b85-4544-809d-a5984fc6f9e7.png)

## Palette16 #{#dom-colors-palette-16}

在大多数终端上支持以下颜色：
- 默认

- 黑色
- 深灰色
- 浅灰色

- 白色

- 蓝色
- 浅蓝色

- 青色
- 浅青色

- 绿色
- 浅绿色

- 品红色
- 浅品红色

- 红色
- 浅红色

- 黄色
- 浅黄色

使用管道操作符使用上述颜色的示例：
```cpp
text("Blue foreground") | color(Color::Blue);
text("Blue background") | bgcolor(Color::Blue);
text("Black on white") | color(Color::Black) | bgcolor(Color::White);
```

## Palette256 #{#dom-colors-palette-256}

在支持 256 色的终端上。 
@htmlonly
<script id="asciicast-OAUc3n6QrkmrLt7XEEb8AzbLt" src="https://asciinema.org/a/OAUc3n6QrkmrLt7XEEb8AzbLt.js" async></script>
@endhtmlonly

```cpp
text("HotPink") | color(Color::HotPink);
```

## TrueColor #{#dom-colors-true-color}

在支持 TrueColor 的终端上，你可以直接使用 24 位 RGB 颜色空间：

使用以下构造函数指定颜色的 **RGB** 或 **HSV** 值：

有两个构造函数：
```cpp
ftxui::Color::RGB(uint8_t red, uint8_t green, uint8_t blue);
ftxui::Color::HSV(uint8_t hue, uint8_t saturation, uint8_t value);
```

@htmlonly
<script id="asciicast-dk5r8IcCH0aFIIgWG0keSEHMG" src="https://asciinema.org/a/dk5r8IcCH0aFIIgWG0keSEHMG.js" async></script>
<script id="asciicast-xwzzghmqcqzIuyLwCpQFEqbEu" src="https://asciinema.org/a/xwzzghmqcqzIuyLwCpQFEqbEu.js" async></script>
@endhtmlonly

# LinearGradient {#dom-linear-gradient}

FTXUI 支持线性渐变。可以在前景或背景上实现。

`ftxui::LinearGradient` 由一个角度（度）和颜色停止列表定义。

你还可以使用简化的构造函数：

参见 [演示](https://arthursonzogni.github.io/FTXUI/examples/?file=component/linear_gradient_gallery)。


# Style {#dom-style}
除了彩色文本和彩色背景。许多终端支持文本效果，例如：`bold`、`italic`、`dim`、`underlined`、`inverted`、`blink`。

[示例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2style_gallery_8cpp-example.html)

要使用这些效果，只需用所需的效果包装你的元素：
```cpp
underlined(bold(text("This text is bold and underlined")))
```

或者，使用管道操作符将其链接到你的元素上：
```cpp
text("This text is bold") | bold | underlined
```

# Layout {#dom-layout}

使元素能够以以下方式排列：
  - **水平**使用 `ftxui::hbox`
  - **垂直**使用 `ftxui::vbox`
  - **在网格内**使用 `ftxui::gridbox`
  - **沿一个方向换行**使用 `ftxui::flexbox`。
  
[示例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2vbox_hbox_8cpp-example.html)使用 `ftxui::hbox`、`ftxui::vbox` 和 `ftxui::filler`。

[示例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2gridbox_8cpp-example.htmlp)使用 `ftxui::gridbox`：

[示例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/hflow.cpp)使用 flexbox：

查看此[示例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2hflow_8cpp-example.html)和相关的[演示](https://arthursonzogni.github.io/FTXUI/examples/?file=component/flexbox)。

元素也可以使用 `ftxui::flex` 装饰器变得灵活。

Code:
```cpp
  hbox({
    text("left") | border ,
    text("middle") | border | flex,
    终端输出：
```bash
┌────┐┌─────────────────────────────────────────────────────┐┌─────┐
│left││middle                                               ││right│
└────┘└─────────────────────────────────────────────────────┘└─────┘
```

Code:
```cpp
  hbox({
    text("left") | border ,
    text("middle") | border | flex,
    终端输出：
```bash
┌────┐┌───────────────────────────────┐┌───────────────────────────────┐
│left││middle                         ││right                          │
└────┘└───────────────────────────────┘└───────────────────────────────┘
```

# Table {#dom-table}

使数据能够轻松格式化为整洁的表格状视觉形式。

[代码示例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2table_8cpp-example.html)：
  
![image](https://user-images.githubusercontent.com/4759106/147250766-77d8ec9e-cf2b-486d-9866-1fd9f1bd2e6b.png)

# Canvas {#dom-canvas}

请参阅 API [<ftxui/dom/canvas.hpp>](./canvas_8hpp_source.html)

绘图可以在 `ftxui::Canvas` 上执行，使用盲文、块或简单字符：

简单[示例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/canvas.cpp)：

复杂[示例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/component/canvas_animated.cpp)：
  
![ezgif com-gif-maker (3)](https://user-images.githubusercontent.com/4759106/147250538-783a8246-98e0-4a25-b032-3bd3710549d1.gif)
