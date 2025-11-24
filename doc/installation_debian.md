@page installation_debian Debian/Ubuntu

## Debian 和 Ubuntu 套件 (非官方)

預建套件由發行版提供。安裝方式如下：

```bash
sudo apt install libftxui-dev
```

以下套件可用：
- `ftxui-doc`
- `ftxui-examples`
- `libftxui-component<version>`
- `libftxui-dev`
- `libftxui-dom<version>`
- `libftxui-screen<version>`

安裝後，您可以將以下內容添加到您的 `CMakeLists.txt` 中，在您的 CMake 專案中使用它：

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
> 這是一個**非官方**套件。這表示它並非由 FTXUI 團隊維護，而是由社群維護。

<div class="section_buttons">

| 上一頁          |
|:------------------|
| [入門](getting-started.html) |

</div>