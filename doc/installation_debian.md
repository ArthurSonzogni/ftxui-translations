@page installation_debian Debian/Ubuntu

## Debian および Ubuntu パッケージ (非公式)

プリビルドされたパッケージはディストリビューションによって提供されます。以下でインストールします。

```bash
sudo apt install libftxui-dev
```

以下のパッケージが利用可能です。
- `ftxui-doc`
- `ftxui-examples`
- `libftxui-component<version>`
- `libftxui-dev`
- `libftxui-dom<version>`
- `libftxui-screen<version>`

インストール後、CMake プロジェクトで利用するには、`CMakeLists.txt` に以下を追加します。

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
> これは**非公式**パッケージです。つまり、FTXUI チームではなく、コミュニティによってメンテナンスされています。

<div class="section_buttons">

| 前へ          |
|:------------------|
| [入門](getting-started.html) |

</div>