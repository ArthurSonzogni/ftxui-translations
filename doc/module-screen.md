@page module-screen ftxui / screen
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595348.png)

`ftxui::screen` 模块是底层基础。它可以独立使用，但主要设计用于与
[ftxui::dom](module-dom.html) 和 [ftxui::component](module-component.html)
模块结合使用。

It provides a @ref ftxui::Screen.

---

### ftxui::Screen

@ref ftxui::Screen 类表示一个 2D 样式字符网格，可以渲染到终端。
它提供了创建屏幕、访问像素和渲染元素的方法。

您可以使用 @ref ftxui::Screen::PixelAt 方法访问屏幕的单个单元格
(@ref ftxui::Pixel)，该方法返回指定坐标处像素的引用。

**Example**
```cpp
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>

void main() {
    auto screen = ftxui::Screen::Create(
        ftxui::Dimension::Full(),   // Use full terminal width
        ftxui::Dimension::Fixed(10) // Fixed height of 10 rows
    );

    // Access a specific pixel at (10, 5)
    auto& pixel = screen.PixelAt(10, 5);

    // Set properties of the pixel.
    pixel.character = U'X';
    pixel.foreground_color = ftxui::Color::Red;
    pixel.background_color = ftxui::Color::RGB(0, 255, 0);
    pixel.bold = true; // Set bold style
    screen.Print(); // Print the screen to the terminal
}
```

> [!note]
> 如果坐标超出边界，则返回一个虚拟像素。

屏幕可以使用 @ref ftxui::Screen::Print() 打印到终端，或使用
@ref ftxui::Screen::ToString() 转换为 std::string。

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

请注意，您可以在打印后通过调用 @ref ftxui::Screen::ResetCursorPosition()
将光标位置重置到屏幕的左上角。

**Example**
```cpp
auto screen = ...;
while(true) {
  // Drawing operations:
  ...
  
  // Print the screen to the terminal. Then reset the cursor position and the
  // screen content.
  std::cout << screen.ToString();
  std::cout << screen.ResetCursorPosition(/*clear=*/true);
  std::cout << std::flush;

  // Sleep for a short duration to control the refresh rate.
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
```

---

### ftxui::Dimension

@ref ftxui::Dimension 工具控制屏幕尺寸：

* `Dimension::Full()` — 使用完整的终端宽度或高度
* `Dimension::Fit(element)` — 尺寸以适应渲染的 @ref ftxui::Element
* `Dimension::Fixed(n)` — 精确使用 `n` 列或行

这些值将传递给 `ftxui::Screen::Create()`。

@ref ftxui::Screen::Create() 提供了两个重载：

- `Screen::Create(Dimension)` 将宽度和高度都设置为相同类型的尺寸
- `Screen::Create(Dimension width, Dimension height)` 允许对每个轴进行不同的控制

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

### ftxui::Pixel

屏幕网格中的每个单元格都是一个 @ref ftxui::Pixel，它包含：

- Unicode 码点。
    - `character`
- @ref ftxui::Color：
    - `foreground_color`
    - `background_color`
- 布尔值：
    - `blink`
    - `bold`
    - `dim`
    - `italic`
    - `inverted` (交换前景色和背景色)
    - `underlined`
    - `underlined_double`
    - `strikethrough`


```cpp
auto screen = ftxui::Screen::Create(
  ftxui::Dimension::Fixed(5),
  ftxui::Dimension::Fixed(5),
);

auto& pixel = screen.PixelAt(3, 3);
pixel.character = U'X';
pixel.bold = true;
pixel.foreground_color = ftxui::Color::Red;
pixel.background_color = ftxui::Color::RGB(0, 255, 0);

screen.Print();
```

> [!note]
> `PixelAt(x, y)` 执行边界检查并返回指定坐标处像素的引用。
> 如果超出边界，则返回一个虚拟像素引用。


屏幕中的每个单元格都是一个 @ref ftxui::Pixel。您可以使用以下方式修改它们：

```cpp
auto& pixel = screen.PixelAt(x, y);
pixel.character = U'X';
pixel.bold = true;
pixel.foreground_color = Color::Red;
```

---

### ftxui::Color

@ref ftxui::Color 类用于定义每个 @ref ftxui::Pixel 的前景色和背景色。

它支持各种颜色空间和预定义调色板。如果终端不支持请求的颜色，FTXUI 将
动态回退到终端中最接近的可用颜色。

**颜色空间**

- **默认**：`ftxui::Color::Default` (终端的默认颜色)
- **16 色调色板** [Demo](https://arthursonzogni.github.io/FTXUI/examples/?file=dom/color_gallery)：
    - `ftxui::Color::Black`,
    - `ftxui::Color::Red`,
    - ...
- **256 色调色板** [Demo](https://arthursonzogni.github.io/FTXUI/examples/?file=dom/color_palette256)：
    - `ftxui::Color::Chartreuse1`,
    - `ftxui::Color::DarkViolet`,
    - ...
- **真彩色** (24 位) [Demo](https://arthursonzogni.github.io/FTXUI/examples/?file=dom/color_truecolor_HSV：
    - `ftxui::Color::RGB(uint8_t red, uint8_t green, uint8_t blue)`
    - `ftxui::Color::HSV(uint8_t h, uint8_t s, uint8_t v)`。
    

> [!note]
> 您可以使用 @ref ftxui::Terminal::ColorSupport() 查询终端功能；
>
> 这可以使用 @ref ftxui::Terminal::SetColorSupport() 手动设置。
