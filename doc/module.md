# ftxui {#ftxui}

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595251.jpg)

FTXUI 分為三個模組，每個模組都建立在前一個的基礎之上：

1. [ftxui/screen](#module-screen) - 低階渲染
2. [ftxui/dom](#module-dom) - 佈局與組合
3. [ftxui/component](#module-component) - 使用者互動

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

- **`ftxui::Element`**：用於佈局和 UI 的樹狀結構。
- 可組合和響應式元素。
- `Render()` 用於繪製到 `Screen` 上。

Ideal for structured, styled UIs.

<div class="section_buttons">

|                                  Next |
|--------------------------------------:|
| [Documentation](module-dom.html) |

</div>


---
# ftxui/component

Adds:

- **`ftxui::Component`**：有狀態、互動式小工具。
- 內建功能：`Checkbox`、`Input`、`Menu`、`Button`。
- 支援鍵盤/游標輸入和組合。

Use for interactive apps.

<div class="section_buttons">

|                                  Next |
|--------------------------------------:|
| [Documentation](module-component.html) |

</div>

---

Modules can be used independently, or together: `screen → dom → component`.
