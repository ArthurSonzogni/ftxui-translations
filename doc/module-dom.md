@page module-dom ftxui / dom
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595347.png)

此模組定義了一組分層的 `ftxui::Element`。元素管理佈局，並能響應終端尺寸的變化。請注意以下範例，其中此模組用於創建一個帶有多個操作符的簡單佈局：

@subpage module-dom-examples 部分提供了一系列範例。

**範例：**
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

**元素清單**

所有元素的清單都已包含，並可透過包含
對應的標頭檔來存取：
```cpp
#include <ftxui/dom/elements.hpp>
```

\include{strip} "ftxui/dom/elements.hpp"

# text # {#dom-text}

最簡單的元件。它顯示一段文字。
```cpp
text("I am a piece of text");
```
```bash
I am a piece of text.
```

# vtext {#dom-vtext}

與 `ftxui::text` 相同，但垂直顯示。

程式碼：
```cpp
vtext("HELLO");
```

終端機輸出：
```bash
H
E
L
L
O
```

# paragraph {#dom-paragraph}

類似於 `ftxui::text`，但單詞會根據其容器的寬度自動換行到多行。

範例程式碼：
```cpp
paragraph("A very long text")
```

![ezgif com-gif-maker (4)](https://user-images.githubusercontent.com/4759106/147251370-983a06e7-6f41-4113-92b8-942f43d34d06.gif)

有關更詳細的範例，請參閱 [詳細範例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2paragraph_8cpp-example.html)。Paragraph 還包括以下所示的其他變體：
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

在元素周圍加上邊框。

程式碼：
```cpp
border(text("The element"))
```

終端機輸出：
```bash
┌───────────┐
│The element│
└───────────┘
```

> [!note]
> 您可以使用 pipe 運算符實現相同的行為。
> 
> 程式碼：
> ```cpp
> text("The element") | border
> ```

Border 也支援多種樣式，如下所示：
```cpp
namespace ftxui {
    Element border(Element);
    Element borderLight(Element);
    Element borderHeavy(Element);
    Element borderDouble(Element);
    Element borderRounded(Element);
    Element borderEmpty(Element);
    Decorator borderStyled(BorderStyle);
    Decorator borderWith(Cell);
}
```


# window # {#dom-window}

`ftxui::window` 是一個 `ftxui::border`，但帶有額外的標頭。要為元素添加視窗，請將其包裹並指定一個字串作為標頭。
程式碼：
```cpp
window("The window", text("The element"))
```

終端機輸出：
```bash
┌The window─┐
│The element│
└───────────┘
```

# separator {#dom-separator}

顯示一條垂直／水平線，以在視覺上將容器的內容
分成兩部分。

程式碼：
```cpp
border(
  hbox({
    text("Left"), 
    separator(),
    text("Right")
  })
)
```

終端機輸出：
```bash
┌────┬─────┐
│left│right│
└────┴─────┘
```


分隔線有多種樣式，如下所示：
```cpp
namespace ftxui {
    Element separator(void);
    Element separatorLight();
    Element separatorHeavy();
    Element separatorDouble();
    Element separatorEmpty();
    Element separatorStyled(BorderStyle);
    Element separator(Cell);
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

這是一個表示進度比例的視覺元素。

程式碼：
```cpp
border(gauge(0.5))
```

終端機輸出：
```bash
┌────────────────────────────────────────────────────────────────────────────┐
│██████████████████████████████████████                                      │
└────────────────────────────────────────────────────────────────────────────┘
```

量表（gauge）可以以多種方向顯示，如下所示：
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

另見：
```cpp
Element graph(GraphFunction);
```

# 顏色 {#dom-colors}

大多數終端控制台都可以顯示彩色文本和彩色背景。FTXUI 支援所有調色板：
```cpp
Decorator color(Color);
Decorator bgcolor(Color);
```


顏色 [圖庫](https://arthursonzogni.github.io/FTXUI/examples_2dom_2color_gallery_8cpp-example.html)：
![image](https://user-images.githubusercontent.com/4759106/147248595-04c7245a-5b85-4544-809d-a5984fc6f9e7.png)

## 16色調色板 {#dom-colors-palette-16}

大多數終端都支援以下顏色：
- Default

- Black
- GrayDark
- GrayLight

- White

- Blue
- BlueLight

- Cyan
- CyanLight

- Green
- GreenLight

- Magenta
- MagentaLight

- Red
- RedLight

- Yellow
- YellowLight

使用管線運算子（pipe operator）搭配上述顏色的範例：
```cpp
text("Blue foreground") | color(Color::Blue);
text("Blue background") | bgcolor(Color::Blue);
text("Black on white") | color(Color::Black) | bgcolor(Color::White);
```

## 256色調色板 {#dom-colors-palette-256}

在支援 256 種顏色的終端上。
@htmlonly
<script id="asciicast-OAUc3n6QrkmrLt7XEEb8AzbLt" src="https://asciinema.org/a/OAUc3n6QrkmrLt7XEEb8AzbLt.js" async></script>
@endhtmlonly

```cpp
text("HotPink") | color(Color::HotPink);
```

## TrueColor (真彩色) {#dom-colors-true-color}

在支援 TrueColor 的終端上，您可以直接使用 24 位 RGB 顏色空間：

使用以下構造函數來指定顏色的 **RGB** 或 **HSV** 值：

有兩種建構子：
```cpp
ftxui::Color::RGB(uint8_t red, uint8_t green, uint8_t blue);
ftxui::Color::HSV(uint8_t hue, uint8_t saturation, uint8_t value);
```

@htmlonly
<script id="asciicast-dk5r8IcCH0aFIIgWG0keSEHMG" src="https://asciinema.org/a/dk5r8IcCH0aFIIgWG0keSEHMG.js" async></script>
<script id="asciicast-xwzzghmqcqzIuyLwCpQFEqbEu" src="https://asciinema.org/a/xwzzghmqcqzIuyLwCpQFEqbEu.js" async></script>
@endhtmlonly

# 線性漸層 {#dom-linear-gradient}

FTXUI 支援線性漸層。無論是在前景還是背景上。

```cpp
Decorator color(const LinearGradient&);
Decorator bgcolor(const LinearGradient&);
```

`ftxui::LinearGradient` 由角度（以度為單位）和顏色停止點列表定義。
```cpp
auto gradient = LinearGradient()
  .Angle(45)
  .AddStop(0.0, Color::Red)
  .AddStop(0.5, Color::Green)
  .AddStop(1.0, Color::Blue);
```

你也可以使用簡化的建構子：
```cpp
LinearGradient(Color::Red, Color::Blue);
```
```cpp
LinearGradient(45, Color::Red, Color::Blue);
```

見 [demo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/linear_gradient_gallery)。


# 樣式 {#dom-style}
除了彩色文本和彩色背景之外。許多終端還支援文本效果，例如：`bold`（粗體）、`italic`（斜體）、`dim`（變暗）、`underlined`（底線）、`inverted`（反轉）、`blink`（閃爍）。

```cpp
Element bold(Element);
Element italic(Element);
Element dim(Element);
Element inverted(Element);
Element underlined(Element);
Element underlinedDouble(Element);
Element strikethrough(Element);
Element blink(Element);
Decorator color(Color);
Decorator bgcolor(Color);
Decorator colorgrad(LinearGradient);
Decorator bgcolorgrad(LinearGradient);
```

[範例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2style_gallery_8cpp-example.html)

![image](https://user-images.githubusercontent.com/4759106/147244118-380bf834-9e33-40df-9ff0-07c10f2598ef.png)

要使用這些效果，只需將你的元素以想要的效果包裝：
```cpp
underlined(bold(text("This text is bold and underlined")))
```

或者，使用 pipe 運算符將其鏈接到您的元素上：
```cpp
text("This text is bold") | bold | underlined
```

# 佈局 {#dom-layout}

使元素可以透過以下方式排列：
  - **水平** 使用 `ftxui::hbox`
  - **垂直** 使用 `ftxui::vbox`
  - **在網格內部** 使用 `ftxui::gridbox`
  - **沿一個方向換行** 使用 `ftxui::flexbox`。
  
使用 `ftxui::hbox`、`ftxui::vbox` 與 `ftxui::filler` 的[範例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2vbox_hbox_8cpp-example.html)。

![image](https://user-images.githubusercontent.com/4759106/147242524-7103b5d9-1a92-4e2d-ac70-b3d6740061e3.png)
  
  
使用 `ftxui::gridbox` 的[範例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2gridbox_8cpp-example.htmlp)：

![image](https://user-images.githubusercontent.com/4759106/147242972-0db1f2e9-0790-496f-86e6-ed2c604f7a73.png)

使用 flexbox 的[範例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/hflow.cpp)：

![image](https://user-images.githubusercontent.com/4759106/147243064-780ac7cc-605b-475f-94b8-cf7c4aed03a5.png)

查看此
[範例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2hflow_8cpp-example.html)
以及相關的
[demo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/flexbox)。

元素也可以使用 `ftxui::flex` 裝飾器變得靈活。

程式碼：
```cpp
  hbox({
    text("left") | border ,
    text("middle") | border | flex,
    text("right") | border,
  });
```
終端機輸出：
```bash
┌────┐┌─────────────────────────────────────────────────────┐┌─────┐
│left││middle                                               ││right│
└────┘└─────────────────────────────────────────────────────┘└─────┘
```

程式碼：
```cpp
  hbox({
    text("left") | border ,
    text("middle") | border | flex,
    text("right") | border | flex,
  });
```

終端機輸出：
```bash
┌────┐┌───────────────────────────────┐┌───────────────────────────────┐
│left││middle                         ││right                          │
└────┘└───────────────────────────────┘└───────────────────────────────┘
```

# 表格 {#dom-table}

讓你可以輕鬆地將資料格式化成整齊的表格視覺形式。

**基本範例：**
```cpp
auto table = Table({
  {"Planet", "Radius", "Mass"},
  {"Mercury", "2440", "0.330"},
  {"Venus", "6052", "4.87"},
  {"Earth", "6371", "5.97"},
  {"Mars", "3390", "0.642"},
});

table.SelectAll().Border(LIGHT);
table.SelectRow(0).Decorate(bold);
table.SelectRow(0).SeparatorVertical(LIGHT);
table.SelectRow(0).Border(DOUBLE);

auto document = table.Render();
```

[程式碼範例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2table_8cpp-example.html)：
  
![image](https://user-images.githubusercontent.com/4759106/147250766-77d8ec9e-cf2b-486d-9866-1fd9f1bd2e6b.png)

### 選取與樣式設定

你可以選取表格的部分內容，並套用裝飾器或邊框到它們身上。選取方法包括：
```cpp
ftxui::TableSelection::SelectAll();
ftxui::TableSelection::SelectCell(column, row);
ftxui::TableSelection::SelectRow(row_index);
ftxui::TableSelection::SelectRows(row_min, row_max);
ftxui::TableSelection::SelectColumn(column_index);
ftxui::TableSelection::SelectColumns(column_min, column_max);
ftxui::TableSelection::SelectRectangle(column_min, column_max, row_min, row_max);
```

完成選取之後，你可以套用：
```cpp
ftxui::TableSelection::Decorate(Decorator); // Apply a decorator to the whole selection (cells and borders).
ftxui::TableSelection::DecorateCells(Decorator); // Apply a decorator only to the cells.
ftxui::TableSelection::Border(BorderStyle); // Add a border around the selection.
ftxui::TableSelection::Separator(BorderStyle); // 
```

### 有顏色的邊框

你也可以將裝飾器專門套用於邊框與分隔線：
```cpp
// Apply a red border to the whole table.
table.SelectAll().Border(LIGHT, color(Color::Red));

// Apply a blue separator to the first row.
table.SelectRow(0).SeparatorVertical(LIGHT, color(Color::Blue));
```

以下方法可用於細緻的邊框裝飾：
```cpp
ftxui::TableSelection::DecorateBorder(Decorator); // Apply a decorator to all borders of the selection.
ftxui::TableSelection::DecorateBorderLeft(Decorator); // Apply a decorator to the left border of the selection.
ftxui::TableSelection::DecorateBorderRight(Decorator); // Apply a decorator to the right border of the selection.
ftxui::TableSelection::DecorateBorderTop(Decorator); // Apply a decorator to the top border of the selection.
ftxui::TableSelection::DecorateBorderBottom(Decorator); // Apply a decorator to the bottom border of the selection.
ftxui::TableSelection::DecorateSeparator(Decorator); // Apply a decorator to all separators of the selection.
ftxui::TableSelection::DecorateSeparatorVertical(Decorator); // Apply a decorator to all vertical separators of the selection.
ftxui::TableSelection::DecorateSeparatorHorizontal(Decorator); // Apply a decorator to all horizontal separators of the selection.
```


# 畫布 {#dom-canvas}

詳見 API [<ftxui/dom/canvas.hpp>](./canvas_8hpp_source.html)

```cpp
  auto c = Canvas(100, 100);
  c.DrawPointLine(10, 10, 80, 10, Color::Red);
  auto element = canvas(c);
```

繪圖可以在 `ftxui::Canvas` 上執行，使用盲文、區塊或簡單字元：
  
簡單的 [範例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/canvas.cpp)：
  
![image](https://user-images.githubusercontent.com/4759106/147245843-76cc62fb-ccb4-421b-aacf-939f9afb42fe.png)

複雜的[範例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/component/canvas_animated.cpp)：
  
![ezgif com-gif-maker (3)](https://user-images.githubusercontent.com/4759106/147250538-783a8246-98e0-4a25-b032-3bd3710549d1.gif)
