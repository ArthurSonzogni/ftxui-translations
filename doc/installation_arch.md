@page installation_arch Arch Linux

FTXUI 已在 AUR 上打包。使用 AUR 助手安裝：

```bash
yay -S ftxui
```

您也可以從 <https://aur.archlinux.org/packages/ftxui> 手動下載 PKGBUILD。

安裝後，您可以將其新增至 `CMakeLists.txt`，以在 CMake 專案中使用：

```cmake
find_package(ftxui REQUIRED)
add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

> [!note]
> 這是一個非官方套件。這表示它並非由 FTXUI 團隊維護，而是由社群維護。
> 套件維護者似乎會積極地將套件更新至最新版本。感謝維護者的辛勤工作！

<div class="section_buttons">

| 上一步          |
|:------------------|
| [入門](getting-started.html) |

</div>