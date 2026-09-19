@page cpp20-modules C++20 モジュール


> [!WARNING]
> この機能はまだ開発中であり、APIは将来のリリースで変更される可能性があります。
> FTXUIにおけるC++モジュールの互換性と使いやすさの向上にご協力をお願いします。
> 何か問題に遭遇した場合や提案がある場合は、issueをオープンしてください。

FTXUIは、コンパイル時間の短縮とコード構成の改善のために、[C++20モジュール](https://en.cppreference.com/w/cpp/language/modules)を実験的にサポートしています。ライブラリの各部分は対応するモジュールを持ち、各ヘッダーごとにパーティションに分割されています。

FTXUIプロジェクト自体をビルドしてC++20モジュールを提供するには、`FTXUI_BUILD_MODULES`オプションを使用します。
例えば、CMakeとNinjaを使う場合:

```sh
cmake \
    -DCMAKE_GENERATOR=Ninja \
    -DFTXUI_BUILD_MODULES=ON \
    ..

ninja
```

> [!NOTE]
> モジュールを使用するには、C++≥20互換コンパイラ、CMakeバージョン3.20以降、およびNinjaのような互換性のあるジェネレータが必要です。Makefileジェネレータは**モジュールをサポートしていません**のでご注意ください。

次に、あなた自身のコードでは、モジュールと通常通りのコードを利用できます:

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

`import ftxui;` と書くことは、すべての `<ftxui/**/*.hpp>` ヘッダーをインクルードすることと同等であり、
単一のモジュールを通じてライブラリ全体を提供します。

CMakeでモジュールを適切に見つけてリンクするには、`target_link_libraries`を使用して適切なコンパイラ、リンカなどのフラグを取得します。

```cmake
target_link_libraries(my_executable
    #...whatever...
    PRIVATE ftxui::modules
)
```

### モジュール一覧

`import ftxui;` はライブラリ全体を提供しますが、FTXUIはレイヤー構造で設計されています。特定の機能だけが必要な場合は、独立したモジュールを直接インポートできます:

- `ftxui`（以下のすべてを再エクスポートする便利モジュール）
    - `ftxui.component`（インタラクティブなコンポーネント、イベント、イベントループ）
    - `ftxui.dom`（Elementによるレイアウトとスタイリング）
    - `ftxui.screen`（ターミナルのレンダリング、ピクセル、色）
    - `ftxui.util`（内部ユーティリティ）

例:
```cpp
import ftxui.screen;
import ftxui.dom;

// Use only screen and dom functionalities...
```
