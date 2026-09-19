@page installation_manual 手冊
@tableofcontents

## 從原始碼建置（官方）

使用 CMake 克隆和構建項目：

```bash
git clone https://github.com/ArthurSonzogni/FTXUI.git
cd FTXUI
cmake -S . -B build -D FTXUI_ENABLE_INSTALL=ON
cmake --build build -j
sudo cmake --install build
```

安裝後，您可以通過將以下內容添加到您的 `CMakeLists.txt` 中，在您的 CMake 項目中使用它：

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

| 上一個          |
|:------------------|
| [入門](getting-started.html) |

</div>
