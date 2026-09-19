@page cpp20-modules C++20 Modules


> [!WARNING]
> この機能はまだ開発中であり、APIは将来のリリースで変更される可能性があります。
> FTXUIにおけるC++モジュールの互換性と使いやすさの向上にご協力をお願いします。
> 何か問題に遭遇した場合や提案がある場合は、issueをオープンしてください。

FTXUIは、コンパイル時間の短縮とコード構成の改善のために、[C++20モジュール](https://en.cppreference.com/w/cpp/language/modules)を実験的にサポートしています。ライブラリの各部分は対応するモジュールを持ち、各ヘッダーごとにパーティションに分割されています。

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
> モジュールを使用するには、C++≥20互換コンパイラ、CMakeバージョン3.20以降、およびNinjaのような互換性のあるジェネレータが必要です。Makefileジェネレータは**モジュールをサポートしていません**のでご注意ください。

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

CMakeでモジュールを適切に見つけてリンクするには、`target_link_libraries`を使用して適切なコンパイラ、リンカなどのフラグを取得します。

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
