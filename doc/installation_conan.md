@page installation_conan Conan 包管理器
@tableofcontents

FTXUI 可以使用 Conan 包管理器轻松获取并集成到您的项目中。

## 前提条件

首先，请确保您的系统上已安装 Conan。如果未安装，您可以通过 pip 进行安装：

```powershell
pip install conan
```
Conan 通常与 CMake 协同工作，因此您还需要安装 CMake。确认 Conan 和 CMake 都已安装后，创建一个项目目录，例如 `ftxui-demo`：

```powershell
mkdir C:\ftxui-demo
cd C:\ftxui-demo
```

## 配置

确保您的环境已正确设置后，创建一个 Conan 配置文件 `conanfile.txt`。此文件用于声明您的项目依赖项。FTXUI 的社区维护包可在 [Conan Center](https://conan.io/center/recipes/ftxui) 上找到。

> [!note]
> 这是一个非官方的构建脚本。这意味着它不是由 FTXUI
> 团队维护，而是由社区维护。包维护者似乎积极地将其更新
> 到最新版本。非常感谢维护者的工作！

@todo 如果您熟悉此过程，请考虑向 Conan Center 添加一个“官方”构建脚本。
这可以是一个 GitHub Action，在新版本发布时自动更新 Conan Center。

```ini
[requires]
ftxui/6.0.2

[generators]
CMakeDeps
CMakeToolchain

[layout]
cmake_layout
```

## 安装依赖项和构建

配置完成后，运行以下命令安装 FTXUI 及其依赖项：

```powershell
conan install . --output-folder=build --build=missing
```

这将从 Conan 的远程仓库下载并安装 `ftxui/6.0.2` 及其所有依赖项。

安装完成后，您可以通过在项目目录中创建 `demo.cpp` 文件进行测试：

```cpp
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>

int main() {
    using namespace ftxui;
    auto document = hbox({
        text(" Hello "),
        text("FTXUI ") | bold | color(Color::Red),
        text(" world! ")
    });
    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);
    std::cout << screen.ToString() << std::endl;
    return 0;
}
```

如果测试成功，您可以在项目目录中创建 `CMakeLists.txt` 文件：

```cmake
cmake_minimum_required(VERSION 3.20)
project(ftxui-demo)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# Find the FTXUI package installed via Conan
find_package(ftxui CONFIG REQUIRED)

# Create the executable
add_executable(demo demo.cpp)

# Link the executable to the FTXUI library
target_link_libraries(demo PRIVATE ftxui::component)
```

@todo 考虑到中国多数地区使用Conan很有可能遇到各种网络问题，我想做一个定制的版本说明，但是我对conan的了解有限再加上没有找到合适的资料，因此这个计划短暂的被搁置了，如果您知道方法，欢迎在[中文版本](xiaoditx.girhub.io/public/docs/ftxui%E4%B8%AD%E6%96%87%E7%BF%BB%E8%AF%91/installation/conan/)的下方留下评论以提醒我

---

<div class="section_buttons">

| 上一页          |
|:------------------|
| [入门](getting-started.html) |

</div>