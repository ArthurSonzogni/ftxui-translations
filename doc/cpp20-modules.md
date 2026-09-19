@page cpp20-modules C++20 模組


> [!WARNING]
> 此功能仍在開發中，API 可能會在未來的版本中更改。
> 我們需要您的貢獻來幫助我們改進 FTXUI 中 C++ 模組的兼容性和可用性。
> 如果您遇到任何問題或有任何建議，請開啟一個 issue。

FTXUI 實驗性地支持
[C++20 模組](https://en.cppreference.com/w/cpp/language/modules) 以減少
編譯時間並改進程式碼組織。庫的每個部分都有一個
對應的模組，每個標頭分成不同的分區。

使用 `FTXUI_BUILD_MODULES` 選項來建置 FTXUI 專案本身以提供 C++20 模組，
例如搭配 CMake 與 Ninja：

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

接著，在您自己的程式碼中就能像平常一樣使用這些模組及程式碼：

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

撰寫 `import ftxui;` 等同於包含所有 `<ftxui/**/*.hpp>` 標頭檔，並透過單一模組
提供整個函式庫。

為了正確地使用 CMake 查找和連結模組，請使用 `target_link_libraries` 來獲取正確的
編譯器、連結器等標誌。

```cmake
target_link_libraries(my_executable
    #...whatever...
    PRIVATE ftxui::modules
)
```

### 模組列表

雖然 `import ftxui;` 提供了整個函式庫，但 FTXUI 是以分層方式設計的。若您只需要特定功能，可以直接匯入獨立的模組：

- `ftxui`（便利模組，重新匯出以下所有模組）
    - `ftxui.component`（互動式元件、事件與事件迴圈）
    - `ftxui.dom`（透過 Element 進行版面與樣式設計）
    - `ftxui.screen`（終端機渲染、像素與顏色）
    - `ftxui.util`（內部工具）

舉例來說：
```cpp
import ftxui.screen;
import ftxui.dom;

// Use only screen and dom functionalities...
```
