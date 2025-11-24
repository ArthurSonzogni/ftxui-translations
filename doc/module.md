# ftxui {#ftxui}

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595251.jpg)

FTXUI 分為三個模組，每個模組都建立在前一個的基礎之上：

1. [ftxui/screen](#module-screen) - 低階渲染
2. [ftxui/dom](#module-dom) - 佈局與組合
3. [ftxui/component](#module-component) - 使用者互動

---

# ftxui/screen

定義：

- **`ftxui::Screen`**：樣式化字元的 2D 網格。
- **`ftxui::Pixel`**：渲染單位。
- 輔助工具，例如 `ftxui::Color` 和 `Dimension`。

用於直接終端繪圖和樣式設定。

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

適用於結構化、樣式化的 UI。

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

適用於互動式應用程式。

<div class="section_buttons">

|                                  Next |
|--------------------------------------:|
| [Documentation](module-component.html) |

</div>

---

模組可以單獨使用，也可以一起使用：`screen → dom → component`。