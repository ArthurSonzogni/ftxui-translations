@page module-screen ftxui / screen
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595348.png)

`ftxui::screen` 模組是低階基礎。它可以單獨使用，但主要設計為與
[ftxui::dom](module-dom.html) 和 [ftxui::component](module-component.html)
模組一起使用。

它提供了一個 @ref ftxui::Screen。

---

# ftxui::Screen

The @ref ftxui::Screen class represents a 2D grid of styled characters that can
be rendered to a terminal.  
It provides methods to create a screen, access cells, and render elements.

You can access the individual cells (@ref ftxui::Cell) of the screen using 
the @ref ftxui::Screen::CellAt method, which returns a reference
to the cell at the specified coordinates.

**Example**
```cpp
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>

void main() {
    auto screen = ftxui::Screen::Create(
        ftxui::Dimension::Full(),   // Use full terminal width
        ftxui::Dimension::Fixed(10) // Fixed height of 10 rows
    );

    // Access a specific cell at (10, 5)
    auto& cell = screen.CellAt(10, 5);

    // Set properties of the cell.
    cell.character = "X";
    cell.foreground_color = ftxui::Color::Red;
    cell.background_color = ftxui::Color::RGB(0, 255, 0);
    cell.bold = true; // Set bold style
    screen.Print(); // Print the screen to the terminal
}
```

> [!note]
> If the coordinates are out of bounds, a dummy cell is returned.

螢幕可以使用 @ref ftxui::Screen::Print() 列印到終端機，或使用 @ref ftxui::Screen::ToString() 轉換為 std::string。

<div class="tabbed">
 
- <b class="tab-title">Print()</b>
  ```cpp
  auto screen = ...;
  screen.Print();
  ```
- <b class="tab-title">ToString()</b>
  ```cpp
  auto screen = ...;
  std::cout << screen.ToString();
  ```
</div>
 
</div>

Note that you can reset the cursor position to the top-left corner of the
screen after printing by calling @ref ftxui::Screen::ResetPosition().

**Example**
```cpp
auto screen = ...;
while(true) {
  // Drawing operations:
  ...
  
  // Print the screen to the terminal. Then reset the cursor position and the
  // screen content.
  std::cout << screen.ToString();
  std::cout << screen.ResetPosition(/*clear=*/true);
  std::cout << std::flush;

  // Sleep for a short duration to control the refresh rate.
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
```

---

# ftxui::Dimension

@ref ftxui::Dimension 工具控制螢幕尺寸：

* `Dimension::Full()` — 使用完整的終端機寬度或高度
* `Dimension::Fit(element)` — 大小以適應渲染的 @ref ftxui::Element
* `Dimension::Fixed(n)` — 精確使用 `n` 列或行

這些值將傳遞給 `ftxui::Screen::Create()`。

@ref ftxui::Screen::Create() 提供兩個重載：

- `Screen::Create(Dimension)` 將寬度和高度都設定為相同類型的尺寸
- `Screen::Create(Dimension width, Dimension height)` 允許按軸進行不同的控制

```cpp
auto screen = ftxui::Screen::Create(
  ftxui::Dimension::Full(),      // width
  ftxui::Dimension::Fixed(10)    // height
);
```

Once created, render an element and display the result:

```cpp
ftxui::Render(screen, element);
screen.Print();
```

---

# ftxui::Cell

Each cell in the screen grid is a @ref ftxui::Cell, which holds:

- Unicode 字碼點。
    - `character`
- @ref ftxui::Color：
    - `foreground_color`
    - `background_color`
- 布林值：
    - `blink`
    - `bold`
    - `dim`
    - `italic`
    - `inverted` (交換前景和背景顏色)
    - `underlined`
    - `underlined_double`
    - `strikethrough`


```cpp
auto screen = ftxui::Screen::Create(
  ftxui::Dimension::Fixed(5),
  ftxui::Dimension::Fixed(5),
);

auto& cell = screen.CellAt(3, 3);
cell.character = "X";
cell.bold = true;
cell.foreground_color = ftxui::Color::Red;
cell.background_color = ftxui::Color::RGB(0, 255, 0);

screen.Print();
```

> [!note]
> `CellAt(x, y)` performs bounds checking and returns a reference to the cell
> at the specified coordinate. If out-of-bounds, a dummy cell reference is
> returned.


Each cell in the screen is a @ref ftxui::Cell. You can modify them using:

```cpp
auto& cell = screen.CellAt(x, y);
cell.character = "X";
cell.bold = true;
cell.foreground_color = Color::Red;
```

---

# ftxui::Color

The @ref ftxui::Color class is used to define foreground and background colors for each @ref ftxui::Cell.

它支援各種色彩空間和預定義調色板。如果終端機不支援請求的顏色，FTXUI 將
動態回退到終端機中最接近的可用顏色。

**Color Spaces**

- **預設**: `ftxui::Color::Default` (終端機的預設顏色)
- **16 色調色板** [演示](https://arthursonzogni.github.io/FTXUI/examples/?file=dom/color_gallery):
    - `ftxui::Color::Black`,
    - `ftxui::Color::Red`,
    - ...
- **256 色調色板** [演示](https://arthursonzogni.github.io/FTXUI/examples/?file=dom/color_palette256):
    - `ftxui::Color::Chartreuse1`,
    - `ftxui::Color::DarkViolet`,
    - ...
- **真彩色** (24 位) [演示](https://arthursonzogni.github.io/FTXUI/examples/?file=dom/color_truecolor_HSV:
    - `ftxui::Color::RGB(uint8_t red, uint8_t green, uint8_t blue)`
    - `ftxui::Color::HSV(uint8_t h, uint8_t s, uint8_t v)`。
    

> [!note]
> You can query the terminal capability using @ref ftxui::Terminal::ColorSupport();
>
> This can manually be set using @ref ftxui::Terminal::SetColorSupport().
