@page installation_opensuse openSUSE

## openSUSE 套件 (非官方)

FTXUI 似乎可以從 `devel:libraries:c_c++` 儲存庫取得。

```bash
sudo zypper addrepo https://download.opensuse.org/repositories/devel:libraries:c_c++/openSUSE_Leap_$releasever/devel:libraries:c_c++.repo
sudo zypper install ftxui
```

詳情請參閱 <https://build.opensuse.org/package/show/devel:libraries:c_c++/ftxui>。

> [!note]
> 這是一個**非官方**套件。這表示它不是由 FTXUI 團隊維護，而是由社群維護。

--

> [!note]
> FTXUI 作者對 openSUSE 不甚了解。此頁面主要由 AI 生成。如果您有任何改進建議，請提交 PR。


<div class="section_buttons">

| 上一頁          |
|:------------------|
| [入門](getting-started.html) |

</div>