@page module-screen ftxui / screen
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595348.png)

`ftxui::screen` 模組是低階基礎。它可以單獨使用，但主要設計為與
[ftxui::dom](module-dom.html) 和 [ftxui::component](module-component.html)
模組一起使用。

它提供了一個 @ref ftxui::Screen。

---

# ftxui::Screen

@ref ftxui::Screen 類別表示一個由已設定樣式的字元組成的 2D 網格，可以
被渲染到終端機。  
它提供了建立畫面、存取儲存格與渲染元素的方法。

你可以使用
@ref ftxui::Screen::CellAt 方法存取畫面上個別的儲存格（@ref ftxui::Cell），
此方法會回傳指定座標處儲存格的參考。

**範例**
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
> 如果座標超出範圍，會回傳一個虛設（dummy）儲存格。

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

注意，你可以在列印後透過呼叫
@ref ftxui::Screen::ResetPosition() 將游標位置重設回畫面的左上角。

**範例**
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

建立完成後，渲染一個元素並顯示結果：

```cpp
ftxui::Render(screen, element);
screen.Print();
```

---

# ftxui::Cell

畫面網格中的每個儲存格都是一個 @ref ftxui::Cell，它保存了：

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
> `CellAt(x, y)` 會執行邊界檢查，並回傳指定座標處
> 儲存格的參考。如果超出範圍，則會回傳一個虛設
> 儲存格的參考。


畫面中的每個儲存格都是一個 @ref ftxui::Cell。你可以使用以下方式修改它們：

```cpp
auto& cell = screen.CellAt(x, y);
cell.character = "X";
cell.bold = true;
cell.foreground_color = Color::Red;
```

---

# ftxui::Color

@ref ftxui::Color 類別用於定義每個 @ref ftxui::Cell 的前景色與背景色。

它支援各種色彩空間和預定義調色板。如果終端機不支援請求的顏色，FTXUI 將
動態回退到終端機中最接近的可用顏色。

**色彩空間**

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
> 你可以使用 @ref ftxui::Terminal::ColorSupport() 查詢終端機的能力；
>
> 這也可以透過 @ref ftxui::Terminal::SetColorSupport() 手動設定。
