@page installation_vcpkg Vcpkg
@tableofcontents

# Vcpkg 套件

FTXUI 可在 [Vcpkg 註冊表](https://vcpkg.link/ports/ftxui) 中取得

若要使用它，您可以將以下內容新增到您的 `vcpkg.json`：

```json
{
  "name": "your-project",
  "version-string": "0.1.0",
  "dependencies": [
    {
        "name": "ftxui",
        "version>=": "7.0.3"
    }
  ]
}
```

# 使用 Vcpkg 安裝 FTXUI
```bash
vcpkg install --triplet x64-linux  # or x64-windows / arm64-osx etc.
```

# 設定您的建置系統。
如果您使用 CMake，可以在 `CMakeLists.txt` 中使用以下內容：

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
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>

int main() {
  using namespace ftxui;

  auto screen = App::TerminalOutput();
  auto button = Button("Click me", [] { std::cout << "Clicked!\n"; });

  screen.Loop(button);
}
```

**設定並建置專案**
```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
./build/main
```




<div class="section_buttons">
 
| 上一個          |
|:------------------|
| [入門](getting-started.html) |
 
</div>
