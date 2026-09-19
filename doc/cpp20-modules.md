@page cpp20-modules C++20 Modules


> [!WARNING]
> 此功能仍在開發中，API 可能會在未來的版本中更改。
> 我們需要您的貢獻來幫助我們改進 FTXUI 中 C++ 模組的兼容性和可用性。
> 如果您遇到任何問題或有任何建議，請開啟一個 issue。

FTXUI 實驗性地支持
[C++20 模組](https://en.cppreference.com/w/cpp/language/modules) 以減少
編譯時間並改進程式碼組織。庫的每個部分都有一個
對應的模組，每個標頭分成不同的分區。

Use the `FTXUI_BUILD_MODULES` option to build the FTXUI project itself to provide C++20 modules,
for example with CMake and Ninja:

```sh
cmake \
    -DCMAKE_GENERATOR=Ninja \
    -DFTXUI_BUILD_MODULES=ON \
    ..

ninja
```

> [!NOTE]
> 要使用模組，您需要 C++≥20 兼容的編譯器、CMake 3.20 或
> 更高版本，並使用兼容的生成器，如 Ninja。請注意，Makefile
> 生成器**不支持模組**。

Then, in your own code you can consume the modules and code as normal:

```cpp
import ftxui;

using ftxui::App;
using ftxui::Button;
using ftxui::Component;

int main() {
  App app = App::TerminalOutput();
  Component button = Button("Click me", app.ExitLoopClosure());
  app.Loop(button);
  return 0;
}
```

Writing `import ftxui;` is equivalent to including all `<ftxui/**/*.hpp>` headers, and provides
the entire library through the singular module.

為了正確地使用 CMake 查找和連結模組，請使用 `target_link_libraries` 來獲取正確的
編譯器、連結器等標誌。

```cmake
target_link_libraries(my_executable
    #...whatever...
    PRIVATE ftxui::modules
)
```

### Module list

While `import ftxui;` provides the entire library, FTXUI is designed in layers. If you only need specific functionalities, you can import the independent modules directly:

- `ftxui` (Convenience module that re-exports all of the below)
    - `ftxui.component` (Interactive components, events, and event loops)
    - `ftxui.dom` (Layout and styling via Elements)
    - `ftxui.screen` (Terminal rendering, pixels, and colors)
    - `ftxui.util` (Internal utilities)

For example:
```cpp
import ftxui.screen;
import ftxui.dom;

// Use only screen and dom functionalities...
```
