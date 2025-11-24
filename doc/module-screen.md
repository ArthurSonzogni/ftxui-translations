@page module-screen ftxui / screen
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595348.png)

`ftxui::screen` 模組是低階基礎。它可以單獨使用，但主要設計為與
[ftxui::dom](module-dom.html) 和 [ftxui::component](module-component.html)
模組一起使用。

它提供了一個 @ref ftxui::Screen。

---

# ftxui::Screen

@ref ftxui::Screen 類別表示一個 2D 樣式字元網格，可以渲染到終端機。
它提供了建立螢幕、存取像素和渲染元素的方法。

您可以使用 @ref ftxui::Screen::PixelAt 方法存取螢幕的個別單元格 (@ref ftxui::Pixel)，
該方法會返回指定座標處像素的參考。

**範例**
```cpp
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>

void main() {
    auto screen = ftxui::Screen::Create(
        ftxui::Dimension::Full(),   // 使用完整的終端機寬度
        ftxui::Dimension::Fixed(10) // 固定高度為 10 行
    );

    // 存取 (10, 5) 處的特定像素
    auto& pixel = screen.PixelAt(10, 5);

    // 設定像素的屬性。
    pixel.character = U'X';
    pixel.foreground_color = ftxui::Color::Red;
    pixel.background_color = ftxui::Color::RGB(0, 255, 0);
    pixel.bold = true; // 設定粗體樣式
    screen.Print(); // 將螢幕列印到終端機
}
```

> [!note]
> 如果座標超出範圍，則返回一個虛擬像素。

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

請注意，您可以在列印後透過呼叫 @ref ftxui::Screen::ResetCursorPosition() 將游標位置重設為螢幕的左上角。

**範例**
```cpp
auto screen = ...;
while(true) {
  // 繪圖操作：
  ...
  
  // 將螢幕列印到終端機。然後重設游標位置和螢幕內容。
  std::cout << screen.ToString();
  std::cout << screen.ResetCursorPosition(/*clear=*/true);
  std::cout << std::flush;

  // 睡眠一小段時間以控制刷新率。
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
  ftxui::Dimension::Full(),      // 寬度
  ftxui::Dimension::Fixed(10)    // 高度
);
```

建立後，渲染一個元素並顯示結果：

```cpp
ftxui::Render(screen, element);
screen.Print();
```

---

# ftxui::Pixel

螢幕網格中的每個單元格都是一個 @ref ftxui::Pixel，它包含：

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

auto& pixel = screen.PixelAt(3, 3);
pixel.character = U'X';
pixel.bold = true;
pixel.foreground_color = ftxui::Color::Red;
pixel.background_color = ftxui::Color::RGB(0, 255, 0);

screen.Print();
```

> [!note]
> `PixelAt(x, y)` 執行邊界檢查並返回指定座標處像素的參考。
> 如果超出邊界，則返回一個虛擬像素參考。


螢幕中的每個單元格都是一個 @ref ftxui::Pixel。您可以使用以下方式修改它們：

```cpp
auto& pixel = screen.PixelAt(x, y);
pixel.character = U'X';
pixel.bold = true;
pixel.foreground_color = Color::Red;
```

---

# ftxui::Color

@ref ftxui::Color 類別用於為每個 @ref ftxui::Pixel 定義前景和背景顏色。

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
> 您可以使用 @ref ftxui::Terminal::ColorSupport() 查詢終端機功能；
> 這可以透過 @ref ftxui::Terminal::SetColorSupport() 手動設定。