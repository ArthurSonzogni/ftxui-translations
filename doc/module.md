# ftxui {#ftxui}

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595251.jpg)

FTXUI は、それぞれが以前のモジュールに基づいて構築された3つのモジュールに編成されています。

1. [ftxui/screen](#module-screen) - 低レベルレンダリング
2. [ftxui/dom](#module-dom) - レイアウトと構成
3. [ftxui/component](#module-component) - ユーザーインタラクション

---

# ftxui/screen

定義:

- **`ftxui::Screen`**: スタイル付き文字の2Dグリッド。
- **`ftxui::Pixel`**: レンダリングの単位。
- `ftxui::Color` や `Dimension` のようなヘルパー。

直接的なターミナル描画とスタイル設定に使用します。

<div class="section_buttons">
 
|                                  Next |
|--------------------------------------:|
| [Documentation](module-screen.html) |
 
</div>


---

# ftxui/dom

提供:

- **`ftxui::Element`**: レイアウトとUIのためのツリー構造。
- 構成可能でレスポンシブな要素。
- `Screen` に描画するための `Render()`。

構造化されたスタイル付きUIに最適です。

<div class="section_buttons">

|                                  Next |
|--------------------------------------:|
| [Documentation](module-dom.html) |

</div>


---
# ftxui/component

追加:

- **`ftxui::Component`**: ステートフルでインタラクティブなウィジェット。
- 組み込み: `Checkbox`, `Input`, `Menu`, `Button`。
- キーボード/カーソル入力と構成をサポート。

インタラクティブなアプリケーションに使用します。

<div class="section_buttons">

|                                  Next |
|--------------------------------------:|
| [Documentation](module-component.html) |

</div>

---

モジュールは個別に、または `screen → dom → component` のように組み合わせて使用できます。