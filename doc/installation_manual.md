@page installation_manual 手动安装
@tableofcontents

## 从源代码构建 (官方)

使用 CMake 克隆和构建项目：

```bash
git clone https://github.com/ArthurSonzogni/FTXUI.git
cd FTXUI
cmake -S . -B build -DFTXUI_ENABLE_INSTALL=ON -D
cmake --build build -j
sudo cmake --install build
```

安装后，您可以将以下内容添加到您的 `CMakeLists.txt` 中，在您的 CMake 项目中使用它：

```cmake
find_package(ftxui REQUIRED)
add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```


<div class="section_buttons">

| 上一页          |
|:------------------|
| [入门](getting-started.html) |

</div>