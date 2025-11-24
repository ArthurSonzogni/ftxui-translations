@page module-screen ftxui / screen
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595348.png)

`ftxui::screen` モジュールは低レベルの基盤です。単独で使用することもできますが、
主に [ftxui::dom](module-dom.html) および [ftxui::component](module-component.html)
モジュールと組み合わせて使用するように設計されています。

それは @ref ftxui::Screen を提供します。

It provides a @ref ftxui::Screen.

---

# ftxui::Screen

@ref ftxui::Screen クラスは、ターミナルにレンダリングできるスタイル付き文字の2Dグリッドを表します。
画面を作成し、ピクセルにアクセスし、要素をレンダリングするメソッドを提供します。

@ref ftxui::Screen::PixelAt メソッドを使用して、画面の個々のセル (@ref ftxui::Pixel) にアクセスできます。
このメソッドは、指定された座標にあるピクセルへの参照を返します。

**例**
```cpp
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>

void main() {
    auto screen = ftxui::Screen::Create(
        ftxui::Dimension::Full(),   // ターミナルの全幅を使用
        ftxui::Dimension::Fixed(10) // 高さを10行に固定
    );

    // (10, 5) の特定のピクセルにアクセス
    auto& pixel = screen.PixelAt(10, 5);

    // ピクセルのプロパティを設定
    pixel.character = U'X';
    pixel.foreground_color = ftxui::Color::Red;
    pixel.background_color = ftxui::Color::RGB(0, 255, 0);
    pixel.bold = true; // 太字スタイルを設定
    screen.Print(); // 画面をターミナルに出力
}
```

> [!note]
> 座標が範囲外の場合、ダミーピクセルが返されます。

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

画面の印刷後、@ref ftxui::Screen::ResetCursorPosition() を呼び出すことで、
カーソル位置を画面の左上隅にリセットできます。

**例**
```cpp
auto screen = ...;
while(true) {
  // 描画操作:
  ...
  
  // 画面をターミナルに出力します。その後、カーソル位置と画面内容をリセットします。
  std::cout << screen.ToString();
  std::cout << screen.ResetCursorPosition(/*clear=*/true);
  std::cout << std::flush;

  // リフレッシュレートを制御するために短時間スリープします。
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
  ftxui::Dimension::Full(),      // 幅
  ftxui::Dimension::Fixed(10)    // 高さ
);
```

作成後、要素をレンダリングして結果を表示します。

```cpp
ftxui::Render(screen, element);
screen.Print();
```

---

# ftxui::Pixel

画面グリッドの各セルは @ref ftxui::Pixel であり、以下を保持します。

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

auto& pixel = screen.PixelAt(3, 3);
pixel.character = U'X';
pixel.bold = true;
pixel.foreground_color = ftxui::Color::Red;
pixel.background_color = ftxui::Color::RGB(0, 255, 0);

screen.Print();
```

> [!note]
> `PixelAt(x, y)` は境界チェックを行い、指定された座標にあるピクセルへの参照を返します。
> 範囲外の場合、ダミーピクセル参照が返されます。


画面内の各セルは @ref ftxui::Pixel です。これらを以下のように変更できます。

```cpp
auto& pixel = screen.PixelAt(x, y);
pixel.character = U'X';
pixel.bold = true;
pixel.foreground_color = Color::Red;
```

---

# ftxui::Color

@ref ftxui::Color クラスは、各 @ref ftxui::Pixel の前景色と背景色を定義するために使用されます。

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
> @ref ftxui::Terminal::ColorSupport() を使用してターミナルの機能を確認できます。
>
> これは @ref ftxui::Terminal::SetColorSupport() を使用して手動で設定できます。
