@page installation_manual マニュアル
@tableofcontents

## Building from Source (Official)

CMakeを使用してプロジェクトをクローンし、ビルドします:

```bash
git clone https://github.com/ArthurSonzogni/FTXUI.git
cd FTXUI
cmake -S . -B build -D FTXUI_ENABLE_INSTALL=ON
cmake --build build -j
sudo cmake --install build
```

インストール後、`CMakeLists.txt`に以下を追加することで、CMakeプロジェクトで使用できます:

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

| 前へ          |
|:------------------|
| [入門](getting-started.html) |

</div>
