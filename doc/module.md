# ftxui {#ftxui}

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595251.jpg)

FTXUI は、それぞれが以前のモジュールに基づいて構築された3つのモジュールに編成されています。

1. [ftxui/screen](#module-screen) - 低レベルレンダリング
2. [ftxui/dom](#module-dom) - レイアウトと構成
3. [ftxui/component](#module-component) - ユーザーインタラクション

あるいは、すべてを含む**`ftxui`**アンブレラモジュールを使用することもできます:

- **ヘッダー**: `#include <ftxui/ftxui.hpp>`
- **CMakeターゲット**: `ftxui::ftxui`
- **Bazelターゲット**: `@ftxui//:ftxui`

---

# ftxui/screen

定義するもの:

- **`ftxui::Screen`**: スタイル付き文字の2Dグリッド。
- **`ftxui::Cell`**: レンダリングの単位。
- `ftxui::Color`や`Dimension`のようなヘルパー。

ターミナルへの直接描画とスタイリングに使用します。

<div class="section_buttons">
 
|                                  次へ |
|--------------------------------------:|
| [ドキュメント](module-screen.html) |
 
</div>


---

# ftxui/dom

提供するもの:

- **`ftxui::Element`**: レイアウトとUIのためのツリー構造。
- 構成可能でレスポンシブな要素。
- `Screen` に描画するための `Render()`。

構造化されたスタイル付きUIに最適です。

<div class="section_buttons">

|                                  次へ |
|--------------------------------------:|
| [ドキュメント](module-dom.html) |

</div>


---
# ftxui/component

追加するもの:

- **`ftxui::Component`**: ステートフルでインタラクティブなウィジェット。
- 組み込み: `Checkbox`, `Input`, `Menu`, `Button`。
- キーボード/カーソル入力と構成をサポート。

インタラクティブなアプリに使用します。

<div class="section_buttons">

|                                  次へ |
|--------------------------------------:|
| [ドキュメント](module-component.html) |

</div>

---

モジュールは独立して、または組み合わせて使用できます: `screen → dom → component`。
