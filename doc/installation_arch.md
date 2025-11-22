@page installation_arch Arch Linux

FTXUI 已打包在 AUR 上。请使用 AUR 助手安装：

```bash
yay -S ftxui
```

您也可以从 <https://aur.archlinux.org/packages/ftxui> 手动下载 PKGBUILD。

安装后，您可以通过将以下内容添加到 `CMakeLists.txt` 中，在您的 CMake 项目中使用它：

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
> 这是一个非官方软件包。这意味着它不是由 FTXUI 团队维护，而是由社区维护。
> 软件包维护者似乎积极地将软件包更新到最新版本。感谢维护者的工作！

<div class="section_buttons">

| 上一步          |
|:------------------|
| [开始使用](getting-started.html) |

</div>