@page installation_vcpkg Vcpkg
@tableofcontents

# Vcpkg 包

FTXUI 可在 [Vcpkg 注册表](https://vcpkg.link/ports/ftxui) 中获取

要使用它，您可以将以下内容添加到您的 `vcpkg.json`：

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

# 使用 Vcpkg 安装 FTXUI
```bash
vcpkg install --triplet x64-linux  # or x64-windows / arm64-osx etc.
```

# 配置您的构建系统
如果您正在使用 CMake，您可以将以下内容添加到您的 `CMakeLists.txt` 中：

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

**配置并构建项目**
```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
./build/main
```


<div class="section_buttons">

| Previous          |
|:------------------|
| [入门](getting-started.html) |

</div>
