@page installation_vcpkg Vcpkg
@tableofcontents

# Vcpkg パッケージ

FTXUIは[Vcpkgレジストリ](https://vcpkg.link/ports/ftxui)で利用可能です。

これを使用するには、`vcpkg.json`に以下を追加します。

```json
{
  "name": "your-project",
  "version-string": "0.1.0",
  "dependencies": [
    {
        "name": "ftxui",
        "version>=": "6.1.9"
    }
  ]
}
```

# Vcpkgを使用してFTXUIをインストールする
```bash
vcpkg install --triplet x64-linux  # or x64-windows / arm64-osx etc.
```

# ビルドシステムを構成する
CMakeを使用している場合、`CMakeLists.txt`で以下を使用できます。

**CMakeLists.txt**
```cmake
cmake_minimum_required(VERSION 3.15)
project(my_project)

# Make sure vcpkg toolchain file is passed at configure time
find_package(ftxui CONFIG REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)
```

**main.cpp**
```cpp
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>

int main() {
  using namespace ftxui;

  auto screen = ScreenInteractive::TerminalOutput();
  auto button = Button("Click me", [] { std::cout << "Clicked!\n"; });

  screen.Loop(button);
}
```

**プロジェクトの構成とビルド**
```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
./build/main
```


<div class="section_buttons">
 
| 前へ          |
|:------------------|
| [入門](getting-started.html) |
 
</div>