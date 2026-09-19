# ftxui {#ftxui}

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595251.jpg)

FTXUI は、それぞれが以前のモジュールに基づいて構築された3つのモジュールに編成されています。

1. [ftxui/screen](#module-screen) - 低レベルレンダリング
2. [ftxui/dom](#module-dom) - レイアウトと構成
3. [ftxui/component](#module-component) - ユーザーインタラクション

Alternatively, you can use the **`ftxui`** umbrella module which includes everything:

- **Header**: `#include <ftxui/ftxui.hpp>`
- **CMake target**: `ftxui::ftxui`
- **Bazel target**: `@ftxui//:ftxui`

---

# ftxui/screen

Defines:

- **`ftxui::Screen`**: a 2D grid of styled characters.
- **`ftxui::Cell`**: the unit of rendering.
- Helpers like `ftxui::Color` and `Dimension`.

Use for direct terminal drawing and styling.

<div class="section_buttons">
 
|                                  Next |
|--------------------------------------:|
| [Documentation](module-screen.html) |
 
</div>


---

# ftxui/dom

Provides:

- **`ftxui::Element`**: レイアウトとUIのためのツリー構造。
- 構成可能でレスポンシブな要素。
- `Screen` に描画するための `Render()`。

Ideal for structured, styled UIs.

<div class="section_buttons">

|                                  Next |
|--------------------------------------:|
| [Documentation](module-dom.html) |

</div>


---
# ftxui/component

Adds:

- **`ftxui::Component`**: ステートフルでインタラクティブなウィジェット。
- 組み込み: `Checkbox`, `Input`, `Menu`, `Button`。
- キーボード/カーソル入力と構成をサポート。

Use for interactive apps.

<div class="section_buttons">

|                                  Next |
|--------------------------------------:|
| [Documentation](module-component.html) |

</div>

---

Modules can be used independently, or together: `screen → dom → component`.
