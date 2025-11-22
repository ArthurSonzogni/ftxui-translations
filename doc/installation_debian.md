@page installation_debian Debian/Ubuntu

## Debian 和 Ubuntu 软件包 (非官方)

预构建的软件包由发行版提供。安装命令如下：

```bash
sudo apt install libftxui-dev
```

以下软件包可用：
- `ftxui-doc`
- `ftxui-examples`
- `libftxui-component<version>`
- `libftxui-dev`
- `libftxui-dom<version>`
- `libftxui-screen<version>`

安装后，您可以在 CMake 项目中使用它，方法是将以下内容添加到您的 `CMakeLists.txt` 中：

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
> 这是一个**非官方**软件包。这意味着它不是由 FTXUI 团队维护，而是由社区维护。

<div class="section_buttons">

| 上一页          |
|:------------------|
| [入门](getting-started.html) |

</div>
