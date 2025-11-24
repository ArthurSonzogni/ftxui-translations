@page cpp20-modules C++20 Modules


> [!WARNING]
> この機能はまだ開発中であり、APIは将来のリリースで変更される可能性があります。
> FTXUIにおけるC++モジュールの互換性と使いやすさの向上にご協力をお願いします。
> 何か問題に遭遇した場合や提案がある場合は、issueをオープンしてください。

FTXUIは、コンパイル時間の短縮とコード構成の改善のために、[C++20モジュール](https://en.cppreference.com/w/cpp/language/modules)を実験的にサポートしています。ライブラリの各部分は対応するモジュールを持ち、各ヘッダーごとにパーティションに分割されています。

C++20モジュールを提供するためにFTXUIプロジェクト自体をビルドするには、FTXUI_BUILD_MODULESオプションを使用します。例えば、CMakeとNinjaを使用する場合：

```sh
cmake \
    -DCMAKE_GENERATOR=Ninja \
    -DFTXUI_BUILD_MODULES=ON \
    ..

ninja
```

> [!NOTE]
> モジュールを使用するには、C++≥20互換コンパイラ、CMakeバージョン3.20以降、およびNinjaのような互換性のあるジェネレータが必要です。Makefileジェネレータは**モジュールをサポートしていません**のでご注意ください。

次に、ご自身のコードでモジュールを通常通り使用できます。

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

ftxui`コンビニエンスモジュールは、すべてのモジュールをまとめるだけのものです。

```cpp
export import ftxui.component;
export import ftxui.dom;
export import ftxui.screen;
export import ftxui.util;
```
必要に応じて、必要なモジュールのみをインポートすることもできます。

CMakeでモジュールを適切に見つけてリンクするには、`target_link_libraries`を使用して適切なコンパイラ、リンカなどのフラグを取得します。

```cmake
target_link_libraries(my_executable
    #...whatever...
    PRIVATE ftxui::modules
)
```

### モジュールリスト

モジュールは、対応するヘッダー、またはより便利なインターフェースを提供するための関連ヘッダーのグループを直接参照します。以下のモジュールが利用可能です。

- `ftxui`
    - `ftxui.component`
    - `ftxui.dom`
    - `ftxui.screen`
    - `ftxui.util`
