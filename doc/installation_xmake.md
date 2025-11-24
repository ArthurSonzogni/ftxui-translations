@page installation_xmake XMake
@tableofcontents

## XMake パッケージ (非公式)

FTXUI は [xmake-repo](https://github.com/xmake-io/xmake-repo/blob/dev/packages/f/ftxui/xmake.lua) で入手可能です。

`xmake.lua` のスニペット例:

```lua
add_requires("ftxui", {system = false})

target("demo")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("ftxui")
```

詳細なオプションについては、[XMake ドキュメント](https://xmake.io) を参照してください。

> [!note]
> これは**非公式**パッケージです。つまり、FTXUI チームによってメンテナンスされているのではなく、コミュニティによってメンテナンスされています。

---

> [!note]
> FTXUI の作者は openSUSE についてあまり詳しくありません。このページはほとんど AI によって生成されました。改善のための提案があれば、プルリクエストをオープンしてください。

---

<div class="section_buttons">

| 前へ          |
|:------------------|
| [入門](getting-started.html) |

</div>