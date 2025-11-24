@page cpp20-modules C++20 Modules


> [!WARNING]
> 此功能仍在開發中，API 可能會在未來的版本中更改。
> 我們需要您的貢獻來幫助我們改進 FTXUI 中 C++ 模組的兼容性和可用性。
> 如果您遇到任何問題或有任何建議，請開啟一個 issue。

FTXUI 實驗性地支持
[C++20 模組](https://en.cppreference.com/w/cpp/language/modules) 以減少
編譯時間並改進程式碼組織。庫的每個部分都有一個
對應的模組，每個標頭分成不同的分區。

使用 FTXUI_BUILD_MODULES 選項來建構 FTXUI 專案本身以提供 C++20 模組，
例如使用 CMake 和 Ninja：

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

然後，在您自己的程式碼中，您可以像往常一樣使用模組和程式碼：

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

請注意，`ftxui` 便利模組只是將所有模組整合在一起：

```cpp
export import ftxui.component;
export import ftxui.dom;
export import ftxui.screen;
export import ftxui.util;
```
You can instead import only the module(s) you need if desired.

為了正確地使用 CMake 查找和連結模組，請使用 `target_link_libraries` 來獲取正確的
編譯器、連結器等標誌。

```cmake
target_link_libraries(my_executable
    #...whatever...
    PRIVATE ftxui::modules
)
```

### 模組列表

這些模組直接引用對應的標頭，或一組相關的
標頭以提供更方便的介面。以下模組
可用：

- `ftxui`
    - `ftxui.component`
    - `ftxui.dom`
    - `ftxui.screen`
    - `ftxui.util`