# ftxui {#ftxui}

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595251.jpg)

FTXUI 分為三個模組，每個模組都建立在前一個的基礎之上：

1. [ftxui/screen](#module-screen) - 低階渲染
2. [ftxui/dom](#module-dom) - 佈局與組合
3. [ftxui/component](#module-component) - 使用者互動

另外，你也可以使用包含一切的**`ftxui`** 總括模組：

- **標頭檔**：`#include <ftxui/ftxui.hpp>`
- **CMake 目標**：`ftxui::ftxui`
- **Bazel 目標**：`@ftxui//:ftxui`

---

# ftxui/screen

定義：

- **`ftxui::Screen`**：一個由已設定樣式的字元組成的 2D 網格。
- **`ftxui::Cell`**：渲染的基本單位。
- 輔助類別，例如 `ftxui::Color` 與 `Dimension`。

用於直接的終端機繪圖與樣式設定。

<div class="section_buttons">
 
|                                  Next |
|--------------------------------------:|
| [Documentation](module-screen.html) |
 
</div>


---

# ftxui/dom

提供：

- **`ftxui::Element`**：用於佈局和 UI 的樹狀結構。
- 可組合和響應式元素。
- `Render()` 用於繪製到 `Screen` 上。

適合結構化、有樣式的 UI。

<div class="section_buttons">

|                                  Next |
|--------------------------------------:|
| [Documentation](module-dom.html) |

</div>


---
# ftxui/component

新增：

- **`ftxui::Component`**：有狀態、互動式小工具。
- 內建功能：`Checkbox`、`Input`、`Menu`、`Button`。
- 支援鍵盤/游標輸入和組合。

用於互動式應用程式。

<div class="section_buttons">

|                                  Next |
|--------------------------------------:|
| [Documentation](module-component.html) |

</div>

---

模組可以獨立使用，也可以一起使用：`screen → dom → component`。
