変更履歴
=========

Next
====

### Doc
- 壊れたDoxygen出力を修正しました。#1029で@markmandelを参照してください。
- Doxygen awesomeを使用し、独自のテーマを追加しました。
- ドキュメントを複数のページに分割しました。

### Build
- 新機能: C++20モジュールをサポート。
  これには以下が必要です:
  - NinjaまたはMSVCジェネレーターの使用。
  - 最新のClang/GCC/MSVCコンパイラ。
  - Cmake 3.28以降。
  使用方法:
  ```cpp
  import ftxui;
  import ftxui.component;
  import ftxui.dom;
  import ftxui.screen;
  import ftxui.util;
  ```
  PR #1015を提供してくれた@mikomikotaishiに感謝します。
- 'pthread'への依存関係を削除。

### Component
- 新機能: POSIXパイプ入力処理。
  - FTXUIアプリケーションが（パイプ経由で）stdinからデータを読み取りながら、ターミナルからキーボード入力を引き続き受信できるようにします。
  - デフォルトで有効になっています。
  - `ScreenInteractive::HandlePipedInput(false)`を使用して無効にできます。
  - LinuxおよびmacOSでのみ利用可能です。
  PR #1094を提供してくれた@HarryPehkonenに感謝します。
- ScreenInteractive::FixedSizeスクリーンが先行するターミナル出力を上書きする問題を修正。#1064で@zozowellに感謝します。
- 垂直方向の`ftxui::Slider`を修正。「上」キーが以前は値を減少させていました。#1093で問題を報告してくれた@its-pabloに感謝します。

### Dom
- `ComputeShrinkHard`での整数オーバーフローを修正。#1137で問題を報告および修正してくれた@its-pabloに感謝します。
- `vbox/hbox/dbox`の特殊化を追加し、Elementのコンテナを入力として許可するようにしました。#1117で@nbusserに感謝します。

6.1.9 (2025-05-07)
------------

### Build
すべてが順調に進めば（保留中）、ftxuiはBazelセントラルリポジトリに表示されるはずです。
以下の行を使用して、プロジェクトにインポートできます:

**MODULE.bazel**
```bazel
bazel_dep(name = "ftxui", version = "6.1.9")
```

レビューしてくれた@robinlindenと@kccに感謝します。

### dom
- バグ修正: ftxui 5.0.0の`dbox`の動作を復元。2つのレイヤー間でbgcolorブレンドを適用するために、新しい`dboxBlend`が追加されます。

6.1.8 (2025-05-01)
------------------

### Build
- 新機能: `bazel`ビルドシステムをサポート。#1032を参照してください。
  Kostya Serebryany @kccによって提案されました。

  **BUILD.bazel**
  ```bazel
  deps = [
    // ライブラリ全体に依存します:
    "@ftxui//:ftxui",

    // 特定のサブモジュールを選択します:
    "@ftxui//:component",
    "@ftxui//:dom",
    "@ftxui//:screen",
  ]
  ```

### Component
- バグ修正: ResizeableSplitでのクラッシュを修正。#1023を参照してください。
  - 画面サイズをターミナルサイズにクランプ。
  - 負のサイズの`ResizeableSplit`を不許可に。

### Dom
- バグ修正: 負のサイズ制約の指定を不許可に。#1023を参照してください。


6.0.2 (2025-03-30)
-----

### Component
- バグ修正: Windowsのすべてのコンポーネントに影響する重大なクラッシュを修正。#1020を参照してください。
- バグ修正: focusRelativeを修正。

6.0.1 (2025-03-28)
-----

v6.0.0と同じです。

タグv6.0.0の問題により置き換えられました。これは良い慣行ではなく、短期間で使い始めた開発者に影響を与えます。同じ内容で新しいリリースを提出するのがこれを修正する最善の方法です。

#1017および#1019を参照してください。

6.0.0 (2025-03-23)
-----

### Component
- 新機能: 生の入力をサポート。より多くのキー検出を可能にします。
- 新機能: `ScreenInteractive::ForceHandleCtrlC(false)`を追加し、コンポーネントがデフォルトの`Ctrl+C`ハンドラを完全にオーバーライドできるようにします。
- 新機能: `ScreenInteractive::ForceHandleCtrlZ(false)`を追加し、コンポーネントがデフォルトの`Ctrl+Z`ハンドラを完全にオーバーライドできるようにします。
- 新機能: サポートされているターミナルで`Mouse::WeelLeft`および`Mouse::WeelRight`イベントを追加。
- 新機能: `Event::DebugString()`を追加。
- 新機能: `Input`の挿入モードをサポート。`InputOption::insert`オプションを追加。@mingsheng13によって追加されました。
- 新機能: ドロップダウンを構成するための`DropdownOption`を追加。#826を参照してください。
- 新機能: 選択をサポート。@clement-roblotに感謝します。#926を参照してください。
  - `ScreenInteractive::GetSelection()`を参照してください。
  - `ScreenInteractive::SelectionChange(...)`リスナーを参照してください。
- バグ修正/破壊的変更: `Mouse transition`:
  - マウスが押された状態ではなく、移動したときに検出します。
    Mouse::Movedモーションが追加されました。
  - 左ボタンを押したままマウスをドラッグすると、複数のチェックボックスがアクティブになるのを防ぎます。
  - いくつかのコンポーネントは、マウスが離されたときではなく、押されたときにアクティブになります。
  これにより、https://github.com/ArthurSonzogni/FTXUI/issues/773が修正されます。
  これにより、https://github.com/ArthurSonzogni/FTXUI/issues/792が修正されます。
- バグ修正: mouse.controlが正しく報告されるようになりました。
- 新機能: `ScreenInteractive::FullscreenPrimaryScreen()`を追加。これにより、代替スクリーンではなく、プライマリスクリーンにフルスクリーンコンポーネントを表示できます。
- バグ修正: `Input` `onchange`がバックスペースまたはDeleteキーで呼び出されない問題を修正。
  PR #776で@chrysanteによって修正されました。
- バグ修正: 終了時にカーソル形状を適切に復元。#792を参照してください。
- バグ修正: 最終列でのカーソル位置を修正。#831を参照してください。
- バグ修正: `ResizeableSplit`のキーボードナビゲーションを修正。#842によって修正されました。
- バグ修正: `Menu`のフォーカスを修正。#841を参照してください。
- 新機能: `ComponentBase::Index()`を追加。これにより、親コンポーネント内のコンポーネントのインデックスを取得できます。#932を参照してください。
- 新機能: `EntryState::index`を追加。これにより、メニューエントリのインデックスを取得できます。
  #932を参照してください。
- 新機能: `SliderOption::on_change`を追加。これにより、スライダーの値が変更されたときにコールバックを設定できます。#938を参照してください。
- バグ修正: エントリのない`Dropdown`を処理。
- バグ修正: 浮動小数点精度とオフバイワンエラーによる`LinearGradient`でのクラッシュを修正。#998を参照してください。

### Dom
- 新機能: `italic`デコレータを追加。例えば:
  ```cpp
  auto italic_text = text("Italic text") | italic;
  ```
  ```cpp
  auto italic_text = italic(text("Italic text"));
  ```
  #1009で@kenRenerisによって提案されました。
- 新機能: `hscroll_indicator`を追加。現在のスクロール位置を反映する水平インジケーターを表示します。#752で@ibrahimnassonによって提案されました。
  [issue 752](https://github.com/ArthurSonzogni/FTXUI/issues/752)
- 新機能: `Dimension::Fit(..)`に`extend_beyond_screen`オプションを追加し、要素が画面より大きくなることを許可します。@LordWhiroによって提案されました。#572および#949を参照してください。
- 新機能: 選択をサポート。@clement-roblotに感謝します。#926を参照してください。
  - `selectionColor`デコレータを参照してください。
  - `selectionBackgroundColor`デコレータを参照してください。
  - `selectionForegroundColor`デコレータを参照してください。
  - `selectionStyle(style)`デコレータを参照してください。
  - `selectionStyleReset`デコレータを参照してください。
- 破壊的変更: 「focus」/「select」の処理方法を変更。これにより動作が修正されます。
- 破壊的変更: `Component::OnRender()`がコンポーネントをレンダリングするためにオーバーライドするメソッドになります。これは、子でレンダリングメソッドを呼び出すためにまだ使用されている`Component::Render()`を置き換えます。この変更により、フォーカス処理に関するいくつかの問題を修正できます。

### Screen
- 新機能: `Box::IsEmpty()`を追加。
- 新機能: 色の透明度
    - `Color::RGBA(r,g,b,a)`を追加。
    - `Color::HSVA(r,g,b,a)`を追加。
    - `Color::Blend(Color)`を追加。
    - `Color::IsOpaque()`を追加。

### Util
- 新機能: `ConstStringListRef`の任意の`Adapter`をサポート。#843を参照してください。

### Build
- cmakeの「unity/jumbo」ビルドをサポート。@ClausKleinによって修正されました。

5.0.0
-----

### Component
- 破壊的変更: MenuDirection enumがDirectionに改名されました。
- 破壊的変更: GaugeDirection enumがDirectionに改名されました。
- 破壊的変更: Direction enumがWidthOrHeightに改名されました。
- 破壊的変更: `ComponentBase`のコピーコンストラクタ/代入を削除。
- 破壊的変更: MenuOption::entriesがMenuOption::entries_optionに改名されました。
- 破壊的変更: コンポーネントコンストラクタで`Ref<{Component}Option>`が`{Component}Option`になります。
- 新機能: `ResizeableSplit`がセパレータとして任意の要素をサポートするようになりました。
- 新機能: `input`が複数行をサポートするようになりました。
- 新機能: `input`スタイルがカスタマイズ可能になりました。
- バグ修正: OSターミナルからのF1-F5をサポート。
- 新機能: 構造体ベースのコンストラクタを追加:
  ```cpp
  Component Button(ButtonOption options);
  Component Checkbox(CheckboxOption options);
  Component Input(InputOption options);
  Component Menu(MenuOption options);
  Component MenuEntry(MenuEntryOption options);
  Component Radiobox(RadioboxOption options);
  Component Slider(SliderOption<T> options);
  Component ResizableSplit(ResizableSplitOption options);
  ```
- 新機能: `ScreenInteractive::TrackMouse(false)`でマウスサポートを無効にします。

### Dom
- 新機能: `hyperlink`デコレータを追加。例えば:
  ```cpp
  auto link = text("Click here") | hyperlink("https://github.com/FTXUI")
  ```
  [OSC 8ページ](https://gist.github.com/egmontkob/eb114294efbcd5adb1944c9f3cb5feda)を参照してください。
  FTXUIのサポートは、[#662](https://github.com/ArthurSonzogni/FTXUI/issues/662)で@aaleinoによって提案されました。

### Screen
- 破壊的変更: `WordBreakProperty`がuint8_t enumになりました。これにより0.8%のパフォーマンス向上があります。
- 破壊的変更: ユーザー定義のPixelコンストラクタと等値演算子を削除。
- パフォーマンス: ベンチマークで19%高速化。

### Build
- cmake find_package()使用時のバージョン互換性をチェック。
- FTXUIビルド時に警告を有効にする`FTXUI_DEV_WARNING`オプションを追加。
- `FTXUI_BUILD_DOCS`をデフォルトでOFFに。
- `FTXUI_BUILD_EXAMPLE`をデフォルトでOFFに。

4.1.1
-----

### Component
- 修正: アプリケーションモードでの矢印キーをサポート。
- 修正: 代替スクリーン使用時の不要な改行を削除。

### Dom
- 新機能: 枠線とセパレータに破線スタイルを追加:
  - `DASHED` enum、`separatorDashed()`、`borderDashed()`関数を参照してください。
- 新機能: 色付きの枠線を追加。
  - 関数:`borderStyled(BorderStyle, Color)`および`borderStyled(Color)`を参照してください。
- 新機能: `LinearGradient`を追加。`color`および`bgColor`で使用できます。
- 改善: Color::Interpolate()がガンマ補正を使用。
- 修正: `graph`領域が正であることを確認。

### Build/Install
- グローバルに設定されたCMAKE_CXX_STANDARDが設定されている場合に使用。
- pkg-configファイルを公開。
- cmake find_package()使用時のバージョン互換性をチェック。

4.1.0 (放棄)
-----
このバージョンは放棄されており、使用してはなりません。APIに破壊的変更が導入されました。

4.0.0
-----

### DOM
- 新機能: その他のスタイル:
  - `strikethrough`
  - `underlinedDouble`
- 新機能: カーソルをカスタマイズ。以下のデコレータを追加:
  - `focusCursorBlock`
  - `focusCursorBlockBlinking`
  - `focusCursorBar`
  - `focusCursorBarBlinking`
  - `focusCursorUnderline`
  - `focusCursorUnderlineBlinking`
- バグ修正: `vbox`/`hbox`/`dbox`が`flexbox`を含む場合の`focus`/`select`を修正。
- バグ修正: 選択/フォーカス領域を修正。以前は要求より1セル大きかった。
- バグ修正: gridbox内の子から選択/フォーカス領域を転送。
- バグ修正: Canvasの計算されたディメンションが不正な問題を修正。
- バグ修正: 内部サイズがゼロの`vscroll_indicator`をサポート。
- バグ修正: `vscroll_indicator`が最後の列を隠す問題を修正。

### Component:
- 新機能: `Modal`コンポーネントを追加。
- 新機能: `Slider`がすべての引数で参照をサポートするようになりました。
- 新機能: `Slider`が`SliderOption`をサポート。以下をサポート:
    - 複数の方向。
    - 複数の色。
    - さまざまな値（value, min, max, increment）。
- 新機能: `ScreenInteractive::Exit()`を定義。
- 新機能: `Loop`を追加し、開発者がメインループをより細かく制御できるようにします。これは、FTXUIを別のメインループに統合し、完全な制御を奪わない場合に使用できます。
- 新機能: `Input`がCTRL+LeftおよびCTRL+Rightをサポート。
- 新機能: `Input`コンポーネントで点滅するバーを使用。
- 改善: `Menu`は、マウスでエントリが選択されたときにフォーカスを保持します。
- バグ修正: `ButtonOption::Border()`の実装を追加。欠落していました。
- バグ修正: F1-F4およびF11の正しいキーを提供。
- 新機能: `Hoverable`コンポーネントデコレータを追加。

### Screen
- 新機能: `Box::Union(a,b) -> Box`を追加。
- バグ修正: `dim`のリセットが`bold`のリセットと衝突する問題を修正。
- 新機能: emscriptenスクリーンサイズ変更サポートを追加。
- バグ修正: 全角文字のUnicode 13サポートを追加。
- バグ修正: MSVCがcodecvt C++17の非推奨関数をエラーとして扱う問題を修正。

### Build
- パッケージマネージャによって提供されるGoogle Testバージョンを使用するサポート。

3.0.0
-----

### Build
- **破壊的変更**: ライブラリプレフィックスが「lib」（デフォルト）に戻りました。これは、cmakeを使用しないユーザーは、例えば「libftxui-dom」にリンクすべきではないことを意味します。

### Component
- **アニメーション**モジュール！コンポーネントは`OnAnimation`メソッドとanimation::Animatorを実装して、アニメーションプロパティを定義できます。
  - `Menu`がアニメーションをサポートするようになりました。
  - `Button`がアニメーションをサポートするようになりました。
- SIGTSTP（ctrl+z）をサポート。
- タスク投稿をサポート。`ScreenInteractive::Post(Task)`。
- `Menu`が`MenuOption.direction`を使用して4方向で使用できるようになりました。
- `Menu`が`MenuOption.underline.enabled`を使用してアニメーション化された下線を表示できるようになりました。
- `Button`がフレーム内でフォーカスを取得するようになりました。
- **破壊的変更** すべてのオプションが変換関数を使用するようになりました。
- **破壊的変更** `Toggle`コンポーネントが`Menu`を使用して実装されるようになりました。
- **バグ修正** Container::Tabが`Focusable()`を実装。
- **バグ修正** ComponentBaseの`Focusable()`および`ActiveChild()`メソッドのデフォルト実装を改善。
- **バグ修正** 「bind」のように、正しいコードを送信しないLinuxプログラムのために、「」キーを自動的に「
」に変換。
  https://github.com/ArthurSonzogni/FTXUI/issues/337
- コンポーネントのデコレータを追加:
  - `operator|(Component, ComponentDecorator)`
  - `operator|(Component, ElementDecorator)`
  - `operator|=(Component, ComponentDecorator)`
  - `operator|=(Component, ElementDecorator)`
  - `Maybe`デコレータを追加。
  - `CatchEvent`デコレータを追加。
  - `Renderer`デコレータを追加。
- **破壊的変更** 「deprectated.hpp」ヘッダとワイド文字列のInputサポートを削除。

### DOM:
- **破壊的変更**: `inverted`デコレータが反転属性で切り替わるようになりました。
- 4方向の`gauge`を追加。以下のAPIを公開:
```cpp
Element gauge(float ratio);
Element gaugeLeft(float ratio);
Element gaugeRight(float ratio);
Element gaugeUp(float ratio);
Element gaugeDown(float ratio);
Element gaugeDirection(float ratio, GaugeDirection);
```
- `separatorHSelector`および`separatorVSelector`要素を追加。これは領域を強調表示するために使用できます。
- `automerge`デコレータを追加。これにより、セパレータ文字が近くの他の文字とマージされるようになります。
- `Table`レンダリング関数を修正し、文字の自動マージを許可するようにしました。
- **バグ修正**: `vscroll_indicator`がオフセットとサイズを正しく計算するようになりました。
- `operator|=(Element, Decorator)`を追加。

### Screen:
- 追加: `Color::Interpolate(lambda, color_a, color_b)`。

2.0.0
-----

### Features:

#### Screen
- `automerge`をPixelビットフィールドに追加。これにより、どのピクセルが自動的にマージされるかが制御されます。

#### DOM:
- `Canvas`クラスと`ElementFrom('canvas')`関数を追加。これにより、ライブラリのユーザーは点字とブロック文字を使用して描画できます。
- `flexbox` DOM要素をサポート。これはHTMLのものと対称的に構築されています。以下のすべての属性がサポートされています: direction, wrap, justify-content, align-items, align-content, gap
- `flexbox`に基づくDOM要素ヘルパーを追加:
  - `paragraph`
  - `paragraphAlignLeft`
  - `paragraphAlignCenter`
  - `paragraphAlignRight`
  - `paragraphAlignJustify`
- `flexbox`に基づくヘルパー要素を追加: `hflow()`, `vflow()`。
- 追加: `focusPositionRelative`および`focusPosition`
- `Table`コンストラクタを、文字列ではなくElementの2Dベクトルから構築するように変更。

#### Component
- `collapsible`コンポーネントを追加。
- `ScreenInteractive::WithRestoredIO`を追加。これはコールバックを装飾します。これにより、一時的にターミナルフックがアンインストールされた状態で実行されます。これは、stdin/stdout/stderrを直接使用してコマンドを実行する場合に役立ちます。

### Bug

#### Table
- `table`の水平および垂直セパレータが正しく展開されるようになりました。

#### Component
- `Input`はマウスでホバーされたときにフォーカスを取得すべきではありません。
- `Input`の`on_enter`/`on_change`イベント中に`Input`を変更する操作が正しく機能するようになりました。

### Breaking changes:
- `paragraph`の動作が変更されました。要素のリストではなく、要素を返すようになりました。

0.11.1
------

# Component
- 新機能: PageUp/PageDown/Home/Endボタンのサポート。
- バグ修正: Dropdownで選択された要素が境界内にあるかを確認。

# Build
- バグ修正: 「リリース設定」を使用してライブラリをパッケージ化。デバッグではない。

0.11
----

## github workflow
- WindowsおよびMacOSアーティファクトを追加。
- すべてのワークフローをマージ。

## Bug
- Unixシステムで、失敗した場合に画面サイズを{80,25}にフォールバック。

## CMake
- `BUILD_SHARED_LIBS`オプションによる共有ライブラリのサポート。
- ライブラリバージョンとシンボリックリンクを追加。

0.10 (2021-09-30)
--------------------

## Bug
- 枠線の自動マージを修正。

### Dom
- `Table()`クラスでスタイル付きテーブルを構築。
   https://github.com/ArthurSonzogni/FTXUI/discussions/228を参照。
- `vscroll_indicator`。右側にスクロールバーインジケーターを表示。
- `separatorEmpty`。何も描画しないセパレータ。
- `separatorFixed`。提供された文字を描画するセパレータ。

### Component
- `Maybe`: 真偽値に基づいてコンポーネントを条件付きで表示。
- `Dropdown`: ドロップダウン選択リスト。

0.9 (2021-09-26)
----------------
変更履歴が書かれ始めた最初のリリースです。

このバージョンには以下が含まれます:

### screen
- スタイル:
  - 太字。
  - 点滅。
  - Dim。
  - 反転。
  - 下線。
  - 前景色。
  - 背景色。
- UTF8ユニコードのサポート。
  - 全角文字: テスト。
  - 結合文字: a⃒
- ステンシルバッファ。
- ボックス描画文字を自動的にマージ。
- ターミナルサイズを検出。

### DOM

- 要素:
  - `text` & `vtext`
  - `separator`と5つのバリエーション。
  - `gauge`
  - `border`と6つのバリエーション。
  - `window`
  - `spinner`
  - `paragraph`と`hflow`。

- レイアウト:
  - `hbox`
  - `vbox`
  - `dbox`
  - `gridbox`
  - `frame`: 実際の領域よりも大きい仮想領域内に描画。
  - `focus`, `select`: フレームの内部ビューをスクロールして表示されるようにする。
  - `flex`と8つのバリエーション。`filler`

- デコレータ:
  - `bold`
  - `dim`
  - `inverted`
  - `blink`
  - `color`
  - `bgcolor`
  - `clearunder`

### Component

- コンテナ:
  - `Container::Vertical`
  - `Container::Horizontal`
  - `Container::Tab`
- `Button`
- `Checkbox`
- `Input`
- `Menu`
- `MenuEntry`
- `Radiobox`
- `Toggle`
- `Slider`
- `Renderer`とバリエーション
- `CatchEvent`

### MISC

- ファザー
- gtestを使用したテスト。
- Doxygenドキュメント
- IWYU
- 52の例。
- WebAssemblyのサポート。
- Windowのサポートと、壊れたターミナルへのフォールバック。