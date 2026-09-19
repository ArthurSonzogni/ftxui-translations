@page getting-started はじめに
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595256.jpg)

FTXUIは、ターミナルベースのユーザーインターフェースのための関数型C++ライブラリです。3つの主要なモジュールで構成されており、それぞれが前のモジュールを基盤としています。

# 3つのモジュール

1.  **Screen**: 最下層。ターミナルの状態、色、生の文字グリッドを扱います。
2.  **DOM**: レイアウトエンジン。複雑でレスポンシブなレイアウトを作成するために組み合わせられる `Element` のセットを提供します。
3.  **Component**: インタラクティブな層。ユーザー入力（キーボード、マウス）を処理し、アプリケーションのメインループを管理します。

# インストールと依存関係

プロジェクトにFTXUIをセットアップするには、[インストールガイド](installation.html)に従ってください。

CMakeユーザーに最も推奨される方法は `FetchContent` を使うことです。これを `CMakeLists.txt` に追加してください:

```cmake
include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main # or a specific version like v7.0.3
)
FetchContent_MakeAvailable(ftxui)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE ftxui::ftxui)
```

# 最初の静的UI (DOM)

`DOM` モジュールを使うと、UIを宣言的に記述できます。要素の合成は、関数呼び出しをネストするか、デコレーターにパイプ演算子を使うだけの簡単な作業です。

これを `main.cpp` として保存してください:

```cpp
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

int main() {
  using namespace ftxui;

  // Define the document structure
  Element document = vbox({
    text("FTXUI Getting Started") | bold | center,
    separator(),
    hbox({
      text("Left Panel") | border,
      vbox({
        text("Main Content Area") | flex,
        separator(),
        text("Footer Information") | dim,
      }) | border | flex,
    }) | flex,
  });

  // Create the screen and render
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}
```

# インタラクティブ性を追加する (Component)

ユーザー入力を処理して動的なアプリケーションを作成するには、`Component` モジュールと `App` クラスを使用します。コンポーネントは自身の状態を管理し、コンテナを使って組み合わせることができます。

```cpp
#include <ftxui/component/component.hpp>
#include <ftxui/component/app.hpp>
#include <ftxui/dom/elements.hpp>

int main() {
  using namespace ftxui;

  std::vector<std::string> entries = {
      "Entry 1",
      "Entry 2",
      "Entry 3",
  };
  int selected = 0;

  // Create a menu component
  auto menu = Menu(&entries, &selected);

  // You can decorate components using the pipe operator.
  auto component = menu | border;

  // Start the main loop
  auto app = App::TerminalOutput();
  app.Loop(component);

  return 0;
}
```

# 次のステップ

*   [サンプル](https://arthursonzogni.github.io/FTXUI/examples/) を閲覧して、何ができるか見てみましょう。
*   事前設定済みのプロジェクト構成については、[スターターテンプレート](https://github.com/ArthurSonzogni/ftxui-starter) をご覧ください。

<div class="section_buttons">
 
| 前へ                          | 次へ                    |
|:----------------------------------|------------------------:|
| [はじめに](index.html) | [モジュール](modules.html) |

 
</div>
