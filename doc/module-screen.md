![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595348.png)

`ftxui::screen` モジュールは低レベルの基盤です。単独で使用することもできますが、
主に [ftxui::dom](module-dom.html) および [ftxui::component](module-component.html)
モジュールと組み合わせて使用するように設計されています。

それは @ref ftxui::Screen を提供します。

@ref ftxui::Screen を提供します。

---

# ftxui::Screen

@ref ftxui::Screen クラスは、ターミナルにレンダリングできるスタイル付き
文字の2Dグリッドを表します。  
スクリーンの作成、セルへのアクセス、要素のレンダリングを行うための
メソッドを提供します。

@ref ftxui::Screen::CellAt メソッドを使用して、スクリーンの個々のセル
（@ref ftxui::Cell）にアクセスできます。このメソッドは、指定された
座標のセルへの参照を返します。

**例**
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
> 座標が範囲外の場合、ダミーのセルが返されます。

画面は @ref ftxui::Screen::Print() を使用してターミナルに出力するか、
@ref ftxui::Screen::ToString() で `std::string` に変換できます。

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

@ref ftxui::Screen::ResetPosition() を呼び出すことで、印刷後に
カーソル位置をスクリーンの左上隅にリセットできることに注意してください。

**例**
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

@ref ftxui::Dimension ユーティリティは画面サイズを制御します。

* `Dimension::Full()` — ターミナルの全幅または全高を使用
* `Dimension::Fit(element)` — レンダリングされた @ref ftxui::Element に合わせてサイズ調整
* `Dimension::Fixed(n)` — 厳密に `n` 列または `n` 行を使用

これらの値は `ftxui::Screen::Create()` に渡されます。

@ref ftxui::Screen::Create() は2つのオーバーロードを提供します。

- `Screen::Create(Dimension)` は幅と高さを同じ種類のディメンションに設定します
- `Screen::Create(Dimension width, Dimension height)` は各軸で異なる制御を可能にします

```cpp
auto screen = ftxui::Screen::Create(
  ftxui::Dimension::Full(),      // width
  ftxui::Dimension::Fixed(10)    // height
);
```

作成したら、要素をレンダリングして結果を表示します:

```cpp
ftxui::Render(screen, element);
screen.Print();
```

---

# ftxui::Cell

スクリーングリッド内の各セルは@ref ftxui::Cellであり、以下を保持します:

- Unicode コードポイント。
    - `character`
- @ref ftxui::Color:
    - `foreground_color`
    - `background_color`
- 真偽値:
    - `blink`
    - `bold`
    - `dim`
    - `italic`
    - `inverted` (前景色と背景色を入れ替えます)
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
> `CellAt(x, y)`は範囲チェックを行い、指定された座標のセルへの
> 参照を返します。範囲外の場合、ダミーのセル参照が返されます。


スクリーン内の各セルは@ref ftxui::Cellです。以下を使って変更できます:

```cpp
auto& cell = screen.CellAt(x, y);
cell.character = "X";
cell.bold = true;
cell.foreground_color = Color::Red;
```

---

# ftxui::Color

@ref ftxui::Color クラスは、各@ref ftxui::Cellの前景色と背景色を
定義するために使用されます。

さまざまな色空間と事前定義されたパレットをサポートしています。FTXUI は、
要求された色がターミナルでサポートされていない場合、ターミナルで利用可能な
最も近い色に動的にフォールバックします。

**色空間**

- **Default**: `ftxui::Color::Default` (ターミナルのデフォルト色)
- **16色パレット** [デモ](https://arthursonzogni.github.io/FTXUI/examples/?file=dom/color_gallery):
    - `ftxui::Color::Black`,
    - `ftxui::Color::Red`,
    - ...
- **256色パレット** [デモ](https://arthursonzogni.github.io/FTXUI/examples/?file=dom/color_palette256):
    - `ftxui::Color::Chartreuse1`,
    - `ftxui::Color::DarkViolet`,
    - ...
- **True color** (24bit) [デモ](https://arthursonzogni.github.io/FTXUI/examples/?file=dom/color_truecolor_HSV:
    - `ftxui::Color::RGB(uint8_t red, uint8_t green, uint8_t blue)`
    - `ftxui::Color::HSV(uint8_t h, uint8_t s, uint8_t v)`.
    

> [!note]
> @ref ftxui::Terminal::ColorSupport(); を使ってターミナルの機能を
> クエリできます。
>
> これは@ref ftxui::Terminal::SetColorSupport() を使って手動で
> 設定することもできます。
