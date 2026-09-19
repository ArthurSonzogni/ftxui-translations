@page module-dom ftxui / dom
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595347.png)

このモジュールは、`ftxui::Element`の階層セットを定義します。要素はレイアウトを管理し、ターミナルの寸法変更に反応できます。このモジュールが多数の演算子を使用してシンプルなレイアウトを作成するために使用される以下の例に注目してください。

@subpage module-dom-examples セクションには、例のコレクションが用意されています。

**Example:**
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

**List of elements**

The list of all elements are included and can be accessed by including the
corresponding header file:
```cpp
#include <ftxui/dom/elements.hpp>
```

\include{strip} "ftxui/dom/elements.hpp"

# text # {#dom-text}

The most simple widget. It displays a text.
```cpp
text("I am a piece of text");
```
```bash
I am a piece of text.
```

# vtext {#dom-vtext}

`ftxui::text`と同じですが、垂直に表示されます。

Code:
```cpp
vtext("HELLO");
```

Terminal output:
```bash
H
E
L
L
O
```

# paragraph {#dom-paragraph}

`ftxui::text`に似ていますが、個々の単語はコンテナの幅に応じて複数行に折り返されます。

Sample Code:
```cpp
paragraph("A very long text")
```

![ezgif com-gif-maker (4)](https://user-images.githubusercontent.com/4759106/147251370-983a06e7-6f41-4113-92b8-942f43d34d06.gif)

より詳細な例については、[詳細な例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2paragraph_8cpp-example.html)を参照してください。Paragraphには、以下に示す他のバリアントも含まれています。
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

Adds a border around an element.

Code:
```cpp
border(text("The element"))
```

Terminal output:
```bash
┌───────────┐
│The element│
└───────────┘
```

> [!note]
> パイプ演算子を使用しても同じ動作を実現できます。
>
> コード:
> ```cpp
> text("The element") | border
> ```

Borderには、以下に示すさまざまなスタイルもあります。
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

`ftxui::window`は`ftxui::border`ですが、追加のヘッダーがあります。要素の周囲にウィンドウを追加するには、それをラップし、ヘッダーとして文字列を指定します。
コード:
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

Displays a vertical/horizontal line to visually split the content of a
container in two.

Code:
```cpp
border(
  hbox({
    text("Left"), 
    separator(),
    text("Right")
  })
)
```

Terminal output:
```bash
┌────┬─────┐
│left│right│
└────┴─────┘
```


Separatorsには、以下に示すさまざまなフレーバーがあります。
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

This is a visual element that represents a ratio of progress.

Code:
```cpp
border(gauge(0.5))
```

Terminal output:
```bash
┌────────────────────────────────────────────────────────────────────────────┐
│██████████████████████████████████████                                      │
└────────────────────────────────────────────────────────────────────────────┘
```

Gauges can be displayed in many orientations as shown below:
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

See:
```cpp
Element graph(GraphFunction);
```

# Colors {#dom-colors}

ほとんどのターミナルコンソールは、色付きのテキストと色付きの背景を表示できます。FTXUIはすべてのカラーパレットをサポートしています。
```cpp
Decorator color(Color);
Decorator bgcolor(Color);
```


カラー[ギャラリー](https://arthursonzogni.github.io/FTXUI/examples_2dom_2color_gallery_8cpp-example.html):
![image](https://user-images.githubusercontent.com/4759106/147248595-04c7245a-5b85-4544-809d-a5984fc6f9e7.png)

## Palette16 #{#dom-colors-palette-16}

ほとんどのターミナルでは、以下の色がサポートされています。
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

Example use of the above colors using the pipe operator:
```cpp
text("Blue foreground") | color(Color::Blue);
text("Blue background") | bgcolor(Color::Blue);
text("Black on white") | color(Color::Black) | bgcolor(Color::White);
```

## Palette256 #{#dom-colors-palette-256}

256色をサポートするターミナルで。
@htmlonly
<script id="asciicast-OAUc3n6QrkmrLt7XEEb8AzbLt" src="https://asciinema.org/a/OAUc3n6QrkmrLt7XEEb8AzbLt.js" async></script>
@endhtmlonly

```cpp
text("HotPink") | color(Color::HotPink);
```

## TrueColor #{#dom-colors-true-color}

TrueColorをサポートするターミナルでは、24ビットRGB色空間を直接使用できます。

以下のコンストラクタを使用して、色の**RGB**または**HSV**値を指定します。

There are two constructors:
```cpp
ftxui::Color::RGB(uint8_t red, uint8_t green, uint8_t blue);
ftxui::Color::HSV(uint8_t hue, uint8_t saturation, uint8_t value);
```

@htmlonly
<script id="asciicast-dk5r8IcCH0aFIIgWG0keSEHMG" src="https://asciinema.org/a/dk5r8IcCH0aFIIgWG0keSEHMG.js" async></script>
<script id="asciicast-xwzzghmqcqzIuyLwCpQFEqbEu" src="https://asciinema.org/a/xwzzghmqcqzIuyLwCpQFEqbEu.js" async></script>
@endhtmlonly

# LinearGradient #{#dom-linear-gradient}

FTXUIは線形グラデーションをサポートしています。前景または背景のいずれかです。

```cpp
Decorator color(const LinearGradient&);
Decorator bgcolor(const LinearGradient&);
```

`ftxui::LinearGradient`は、角度（度数）と色の停止点のリストによって定義されます。
```cpp
auto gradient = LinearGradient()
  .Angle(45)
  .AddStop(0.0, Color::Red)
  .AddStop(0.5, Color::Green)
  .AddStop(1.0, Color::Blue);
```

You can also use simplified constructors:
```cpp
LinearGradient(Color::Red, Color::Blue);
```
```cpp
LinearGradient(45, Color::Red, Color::Blue);
```

[デモ](https://arthursonzogni.github.io/FTXUI/examples/?file=component/linear_gradient_gallery)を参照してください。


# Style {#dom-style}
色付きのテキストと色付きの背景に加えて、多くのターミナルは`bold`、`italic`、`dim`、`underlined`、`inverted`、`blink`などのテキスト効果をサポートしています。

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

[例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2style_gallery_8cpp-example.html)

![image](https://user-images.githubusercontent.com/4759106/147244118-380bf834-9e33-40df-9ff0-07c10f2598ef.png)

To use these effects, simply wrap your elements with your desired effect:
```cpp
underlined(bold(text("This text is bold and underlined")))
```

Alternatively, use the pipe operator to chain it on your element:
```cpp
text("This text is bold") | bold | underlined
```

# Layout {#dom-layout}

要素を次の方法で配置できます。
  - `ftxui::hbox`で**水平に**
  - `ftxui::vbox`で**垂直に**
  - `ftxui::gridbox`で**グリッド内に**
  - `ftxui::flexbox`を使用して**一方向に折り返し**
  
`ftxui::hbox`、`ftxui::vbox`、`ftxui::filler`を使用した[例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2vbox_hbox_8cpp-example.html)。

![image](https://user-images.githubusercontent.com/4759106/147242524-7103b5d9-1a92-4e2d-ac70-b3d6740061e3.png)
  
  
`ftxui::gridbox`を使用した[例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2gridbox_8cpp-example.htmlp)。

![image](https://user-images.githubusercontent.com/4759106/147242972-0db1f2e9-0790-496f-86e6-ed2c604f7a73.png)

flexboxを使用した[例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/hflow.cpp)。

![image](https://user-images.githubusercontent.com/4759106/147243064-780ac7cc-605b-475f-94b8-cf7c4aed03a5.png)

この[例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2hflow_8cpp-example.html)と関連する[デモ](https://arthursonzogni.github.io/FTXUI/examples/?file=component/flexbox)を確認してください。

要素は`ftxui::flex`デコレータを使用して柔軟にすることもできます。

Code:
```cpp
  hbox({
    text("left") | border ,
    text("middle") | border | flex,
    text("right") | border,
  });
```
Terminal output:
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
    text("right") | border | flex,
  });
```

Terminal output:
```bash
┌────┐┌───────────────────────────────┐┌───────────────────────────────┐
│left││middle                         ││right                          │
└────┘└───────────────────────────────┘└───────────────────────────────┘
```

# Table {#dom-table}

Enables easy formatting of data into a neat table like visual form.

**Basic example:**
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

[コード例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2table_8cpp-example.html):
  
![image](https://user-images.githubusercontent.com/4759106/147250766-77d8ec9e-cf2b-486d-9866-1fd9f1bd2e6b.png)

### Selection and Styling

You can select parts of the table and apply decorators or borders to them. Selection methods include:
```cpp
ftxui::TableSelection::SelectAll();
ftxui::TableSelection::SelectCell(column, row);
ftxui::TableSelection::SelectRow(row_index);
ftxui::TableSelection::SelectRows(row_min, row_max);
ftxui::TableSelection::SelectColumn(column_index);
ftxui::TableSelection::SelectColumns(column_min, column_max);
ftxui::TableSelection::SelectRectangle(column_min, column_max, row_min, row_max);
```

Once a selection is made, you can apply:
```cpp
ftxui::TableSelection::Decorate(Decorator); // Apply a decorator to the whole selection (cells and borders).
ftxui::TableSelection::DecorateCells(Decorator); // Apply a decorator only to the cells.
ftxui::TableSelection::Border(BorderStyle); // Add a border around the selection.
ftxui::TableSelection::Separator(BorderStyle); // 
```

### Colored borders

You can also apply decorators specifically to borders and separators:
```cpp
// Apply a red border to the whole table.
table.SelectAll().Border(LIGHT, color(Color::Red));

// Apply a blue separator to the first row.
table.SelectRow(0).SeparatorVertical(LIGHT, color(Color::Blue));
```

The following methods are available for fine-grained border decoration:
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


# Canvas {#dom-canvas}

API [<ftxui/dom/canvas.hpp>](./canvas_8hpp_source.html)を参照してください。

```cpp
  auto c = Canvas(100, 100);
  c.DrawPointLine(10, 10, 80, 10, Color::Red);
  auto element = canvas(c);
```

描画は`ftxui::Canvas`で、点字、ブロック、または単純な文字を使用して実行できます。
  
単純な[例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/canvas.cpp):
  
![image](https://user-images.githubusercontent.com/4759106/147245843-76cc62fb-ccb4-421b-aacf-939f9afb42fe.png)

複雑な[例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/component/canvas_animated.cpp):
  
![ezgif com-gif-maker (3)](https://user-images.githubusercontent.com/4759106/147250538-783a8246-98e0-4a25-b032-3bd3710549d1.gif)
