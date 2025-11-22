@page cpp20-modules C++20 Modules


> [!WARNING]
> 此功能仍在开发中，未来的版本中API可能会发生变化。
> 我们需要您的贡献来帮助我们改进FTXUI中C++模块的兼容性和可用性。
> 如果您遇到任何问题或有建议，请提出问题。

FTXUI实验性地支持[C++20模块](https://en.cppreference.com/w/cpp/language/modules)，以减少编译时间并改善代码组织。
库的每个部分都有一个对应的模块，并根据每个头文件分成不同的分区。

使用FTXUI_BUILD_MODULES选项构建FTXUI项目本身以提供C++20模块，
例如使用CMake和Ninja：

```sh
cmake \
    -DCMAKE_GENERATOR=Ninja \
    -DFTXUI_BUILD_MODULES=ON \
    ..

ninja
```

> [!NOTE]
> 要使用模块，您需要一个C++≥20兼容的编译器，CMake 3.20或更高版本，
> 并使用像Ninja这样的兼容生成器。请注意，Makefile生成器**不支持模块**。

然后，在您自己的代码中，您可以像往常一样使用模块和代码：

```cpp
import ftxui;

using ftxui::Button;
using ftxui::ScreenInteractive;

int main() {
  auto screen = ScreenInteractive::TerminalOutput();
  auto button = Button("Click me", screen.QuitClosure());
  screen.Loop(button);
  return 0;
}
```

请注意，`ftxui`便利模块将所有模块简单地组合在一起：

```cpp
export import ftxui.component;
export import ftxui.dom;
export import ftxui.screen;
export import ftxui.util;
```
如果需要，您可以只导入您需要的模块。

要使用CMake正确查找和链接模块，请使用`target_link_libraries`以获取正确的编译器、链接器等标志。

```cmake
target_link_libraries(my_executable
    #...whatever...
    PRIVATE ftxui::modules
)
```

### 模块列表

这些模块直接引用相应的头文件，或一组相关的头文件，以提供更方便的接口。
以下模块可用：

- `ftxui`
    - `ftxui.component`
    - `ftxui.dom`
    - `ftxui.screen`
    - `ftxui.util`