@mainpage はじめに
@tableofcontents

<img src="https://github.com/ArthurSonzogni/FTXUI/assets/4759106/6925b6da-0a7e-49d9-883c-c890e1f36007" alt="Demo image"></img>

**FTXUI** は、ターミナルベースのユーザーインターフェースのためのシンプルでクロスプラットフォームなC++ライブラリです！

# 特徴
 * 関数型スタイル。以下から着想を得ています
   [1](https://hackernoon.com/building-reactive-terminal-interfaces-in-c-d392ce34e649?gi=d9fb9ce35901)
   および[React](https://reactjs.org/)
 * 依存関係なし
 * **クロスプラットフォーム**。
 * シンプルでエレガントな構文（私見では）
 * キーボード＆マウスによるナビゲーション。
 * [UTF8](https://en.wikipedia.org/wiki/UTF-8)と[全角文字](https://en.wikipedia.org/wiki/Halfwidth_and_fullwidth_forms)のサポート (→ 测试)
 * アニメーションのサポート。[デモ1](https://arthursonzogni.github.io/FTXUI/examples/?file=component/menu_underline_animated_gallery)、[デモ2](https://arthursonzogni.github.io/FTXUI/examples/?file=component/button_style)
 * 描画のサポート。[デモ](https://arthursonzogni.github.io/FTXUI/examples/?file=component/canvas_animated)
 * [インストール](installation.html)と[はじめに](getting-started.html)ガイドで学べます。
 * 複数のビルドシステムとパッケージ: 
 * 良い実践: ドキュメント、テスト、ファザー、パフォーマンステスト、自動化されたCI、自動化されたパッケージング、など...

# 例

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

期待される出力:

```
┌────┐┌────────────────────────────────────┐┌─────┐
│left││middle                              ││right│
└────┘└────────────────────────────────────┘└─────┘
```

# サポートされているプラットフォーム

- Linux
- MacOS
- Windows
- WebAssembly

<div class="section_buttons">
 
| 次へ |
|--------------------------------------:|
| [入門](getting-started.html) |
 
</div>

@defgroup screen ftxui/screen

ftxui/screen` モジュールの [チュートリアル](module-screen.html) をご確認ください。

@defgroup dom ftxui/dom

ftxui/dom` モジュールの [チュートリアル](module-dom.html) をご確認ください。

@defgroup component ftxui/component

`ftxui/component`モジュールの[チュートリアル](module-component.html)を確認してください。
