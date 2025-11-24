@page installation_arch Arch Linux

FTXUIはAURにパッケージされています。AURヘルパーを使用してインストールしてください:

```bash
yay -S ftxui
```

PKGBUILDは<https://aur.archlinux.org/packages/ftxui>から手動でダウンロードすることもできます。

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

> [!note]
> これは非公式パッケージです。つまり、FTXUIチームによってではなく、コミュニティによって維持されています。パッケージメンテナは、パッケージを最新バージョンに積極的に更新しているようです。メンテナの皆様、お疲れ様です！

<div class="section_buttons">

| 前へ          |
|:------------------|
| [はじめに](getting-started.html) |

</div>