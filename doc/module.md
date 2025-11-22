# ftxui {#ftxui}

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595251.jpg)

FTXUI 由三个模块组成，每个模块都建立在前一个模块的基础上：

1. [ftxui/screen](#module-screen) - 低级渲染
2. [ftxui/dom](#module-dom) - 布局和组合
3. [ftxui/component](#module-component) - 用户交互

---

# ftxui/screen

定义：

- **`ftxui::Screen`**：一个样式化字符的 2D 网格。
- **`ftxui::Pixel`**：渲染单位。
- 辅助工具，例如 `ftxui::Color` 和 `Dimension`。

用于直接终端绘图和样式设置。

<div class="section_buttons">
 
|                                  下一页 |
|--------------------------------------:|
| [文档](module-screen.html) |
 
</div>


---

# ftxui/dom

提供：

- **`ftxui::Element`**：用于布局和 UI 的树状结构。
- 可组合和响应式元素。
- `Render()` 用于在 `Screen` 上绘图。

结构化、样式化 UI 的理想选择。

<div class="section_buttons">

|                                  下一页 |
|--------------------------------------:|
| [文档](module-dom.html) |

</div>


---
# ftxui/component

添加：

- **`ftxui::Component`**：有状态的交互式小部件。
- 内置组件：`Checkbox`、`Input`、`Menu`、`Button`。
- 支持键盘/光标输入和组合。

用于交互式应用程序。

<div class="section_buttons">

|                                  下一页 |
|--------------------------------------:|
| [文档](module-component.html) |

</div>

---

模块可以独立使用，也可以一起使用：`screen → dom → component`.
