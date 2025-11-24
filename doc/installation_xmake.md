@page installation_xmake XMake (XMake 安裝)
@tableofcontents

## XMake 套件 (非官方)

FTXUI 可從 [xmake-repo](https://github.com/xmake-io/xmake-repo/blob/dev/packages/f/ftxui/xmake.lua) 取得。

`xmake.lua` 程式碼範例：

```lua
add_requires("ftxui", {system = false})

target("demo")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("ftxui")
```

有關更多選項，請參閱 [XMake 文件](https://xmake.io)。

> [!note]
> 這是一個 **非官方** 套件。這表示它並非由 FTXUI 團隊維護，而是由社群維護。

---

> [!note]
> FTXUI 作者對 openSUSE 知之甚少。此頁面主要由 AI 生成。如果您有任何改進建議，請提交 PR。

---

<div class="section_buttons">

| Previous          |
|:------------------|
| [Getting Started](getting-started.html) |

</div>