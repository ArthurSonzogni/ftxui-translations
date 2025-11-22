@page installation_xmake XMake
@tableofcontents

## XMake 包 (非官方)

FTXUI 可在 [xmake-repo](https://github.com/xmake-io/xmake-repo/blob/dev/packages/f/ftxui/xmake.lua) 中获取

示例 `xmake.lua` 片段：

```lua
add_requires("ftxui", {system = false})

target("demo")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("ftxui")
```

有关更多选项，请参阅 [XMake 文档](https://xmake.io)。

> [!note]
> 这是一个**非官方**包。这意味着它不由 FTXUI 团队维护，而是由社区维护。

---

> [!note]
> FTXUI 作者对 openSUSE 知之甚少。此页面主要由 AI 生成。如果您有任何改进建议，请提交 PR。

---

<div class="section_buttons">

| Previous          |
|:------------------|
| [Getting Started](getting-started.html) |

</div>
