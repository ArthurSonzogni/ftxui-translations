
<p align="center">
  <img src="https://github.com/ArthurSonzogni/FTXUI/assets/4759106/6925b6da-0a7e-49d9-883c-c890e1f36007" alt="Demo image"></img>
  <br/>
  <a href="#"><img src="https://img.shields.io/badge/c++-%2300599C.svg?style=flat&logo=c%2B%2B&logoColor=white"></img></a>
  <a href="http://opensource.org/licenses/MIT"><img src="https://img.shields.io/github/license/arthursonzogni/FTXUI?color=black"></img></a>
  <a href="#"><img src="https://img.shields.io/github/stars/ArthurSonzogni/FTXUI"></img></a>
  <a href="#"><img src="https://img.shields.io/github/forks/ArthurSonzogni/FTXUI"></img></a>
  <a href="#"><img src="https://img.shields.io/github/repo-size/ArthurSonzogni/FTXUI"></img></a>
  <a href="https://github.com/ArthurSonzogni/FTXUI/graphs/contributors"><img src="https://img.shields.io/github/contributors/arthursonzogni/FTXUI?color=blue"></img></a>
  <br/>
  <a href="https://github.com/ArthurSonzogni/FTXUI/issues"><img src="https://img.shields.io/github/issues/ArthurSonzogni/FTXUI"></img></a>
  <a href="https://repology.org/project/ftxui/versions"><img src="https://repology.org/badge/latest-versions/ftxui.svg" alt="latest packaged version(s)"></a>
  <a href="https://codecov.io/gh/ArthurSonzogni/FTXUI">
    <img src="https://codecov.io/gh/ArthurSonzogni/FTXUI/branch/master/graph/badge.svg?token=C41FdRpNVA"/>
  </a>

  
  <br/>
  <a href="https://arthursonzogni.github.io/FTXUI/">ドキュメント</a> ·
  <a href="https://github.com/ArthurSonzogni/FTXUI/issues">バグを報告</a> ·
  <a href="https://arthursonzogni.github.io/FTXUI/examples/">例</a> ·
  <a href="https://github.com/ArthurSonzogni/FTXUI/issues">機能のリクエスト</a> ·
  <a href="https://github.com/ArthurSonzogni/FTXUI/pulls">プルリクエストを送信</a>
  
  <br/>
  ドキュメント:
  <a href="https://github.com/ArthurSonzogni/">English</a> |
  <a href="https://arthursonzogni.github.io/FTXUI/fr/index.html">Français</a> |
  <a href="https://arthursonzogni.github.io/FTXUI/es/index.html">Español</a> |
  <a href="https://arthursonzogni.github.io/FTXUI/zh-TW/index.html">繁體中文</a> |
  <a href="https://arthursonzogni.github.io/FTXUI/zh-CH/index.html">简体中文</a> |
  <a href="https://arthursonzogni.github.io/FTXUI/ja/index.html">日本語</a> |

</p>

# FTXUI

<i>Functional Terminal (X) User interface</i><br>
関数型ターミナル (X) ユーザーインターフェース

ターミナルベースのユーザーインターフェースのための、シンプルなクロスプラットフォームC++ライブラリ!

## 特徴
 * 関数型スタイル。以下からインスピレーションを得ています
   [1](https://hackernoon.com/building-reactive-terminal-interfaces-in-c-d392ce34e649?gi=d9fb9ce35901)
   および [React](https://reactjs.org/)
 * シンプルでエレガントな構文（個人的な意見です）
 * キーボード＆マウスによるナビゲーション
 * [UTF8](https://en.wikipedia.org/wiki/UTF-8) と [全角文字](https://en.wikipedia.org/wiki/Halfwidth_and_fullwidth_forms) のサポート (→ 测试)
 * アニメーションのサポート。[デモ 1](https://arthursonzogni.github.io/FTXUI/examples/?file=component/menu_underline_animated_gallery)、[デモ 2](https://arthursonzogni.github.io/FTXUI/examples/?file=component/button_style)
 * 描画のサポート。[デモ](https://arthursonzogni.github.io/FTXUI/examples/?file=component/canvas_animated)
 * 依存関係なし
 * [C++20 モジュールのサポート](https://arthursonzogni.github.io/FTXUI/cpp20-modules.html)
 * **クロスプラットフォーム**: Linux/MacOS（メインターゲット）、WebAssembly、Windows（コントリビューターのおかげです！）
 * [サンプル](#documentation)や[チュートリアル](#documentation)で学べます
 * 複数のパッケージ:
     - CMake [FetchContent]([https://bewagner.net/programming/2020/05/02/cmake-fetchcontent/](https://cmake.org/cmake/help/latest/module/FetchContent.html)) （推奨）
     - [Bazel](https://registry.bazel.build/modules/ftxui)
     - [vcpkg](https://vcpkg.link/ports/ftxui)
     - [Conan](https://conan.io/center/recipes/ftxui) [Debian パッケージ](https://tracker.debian.org/pkg/ftxui)
     - [Ubuntu パッケージ](https://launchpad.net/ubuntu/+source/ftxui)
     - [Arch Linux](https://aur.archlinux.org/packages/ftxui/)
    - [OpenSUSE](https://build.opensuse.org/package/show/devel:libraries:c_c++/ftxui)
    - [XMake](https://xmake.io) リポジトリ [パッケージ](https://github.com/xmake-io/xmake-repo/blob/dev/packages/f/ftxui/xmake.lua)
    - [Nix](https://github.com/ArthurSonzogni/FTXUI/blob/main/flake.nix)
    - [アマルガメーション版（単一ヘッダー/ソース）](https://arthursonzogni.github.io/FTXUI/installation_amalgamated.html) (7.0.0から)
    - [conda-forge](https://anaconda.org/channels/conda-forge/packages/ftxui/overview)
 * 優れた開発習慣: ドキュメント、テスト、ファジング、パフォーマンステスト、自動CI、自動パッケージングなど

## ドキュメント

- [スターター CMake](https://github.com/ArthurSonzogni/ftxui-starter)
- [スターター Bazel](https://github.com/ArthurSonzogni/ftxui-bazel)
- [ドキュメント](https://arthursonzogni.github.io/FTXUI/)
- [サンプル (WebAssembly)](https://arthursonzogni.github.io/FTXUI/examples/)
- [CMake でビルドする](https://arthursonzogni.github.io/FTXUI/#build-cmake)
- [Bazel でビルドする](https://arthursonzogni.github.io/FTXUI/#build-bazel)
- [Meson でビルドする](doc/installation_meson.md)

## サンプル
~~~cpp
#include <ftxui/ftxui.hpp>
using namespace ftxui;

int main() {
  auto document =
    vbox({
      hbox({
        text("one") | border,
        text("two") | border | flex,
        text("three") | border | flex,
      }),

      gauge(0.25) | color(Color::Red),
      gauge(0.50) | color(Color::White),
      gauge(0.75) | color(Color::Blue),
    });

  auto screen = Screen::Create(Dimension::Full());
  Render(screen, document);
  screen.Print();

  return 0;
}
~~~

![image](https://github.com/ArthurSonzogni/FTXUI/assets/4759106/569bf043-4e85-4245-aad5-2324572135c4)

## 簡単なギャラリー

FTXUIは3つのモジュールで構成されています:
1.  **screen**: 低レベルのレンダリング（色、ピクセル、ターミナル）
2.  **dom**: レイアウトと構成（階層的な要素）
3.  **component**: ユーザーとのインタラクション（ウィジェット、イベント、メインループ）

ほとんどのユーザーにとって、すべてを一度にインクルードするのが最も簡単な方法です:
- ヘッダー: `#include <ftxui/ftxui.hpp>`
- CMake ターゲット: `ftxui::ftxui`
- Bazel ターゲット: `@ftxui//:ftxui`

#### DOM

このモジュールは `Element` の階層的な集合を定義します。`Element` はレイアウトを管理し、ターミナルの寸法に応じてレスポンシブになれます。

これらは [`<ftxui/dom/elements.hpp>`](https://arthursonzogni.github.io/FTXUI/elements_8hpp_source.html
) に宣言されています
  
<details><summary>レイアウト</summary>

`Element` は次のように組み合わせることができます:
  - `hbox` で水平方向に
  - `vbox` で垂直方向に
  - `gridbox` でグリッド内に
  - `flexbox` で一方向に折り返して
  
`Element` は `flex` デコレーターを使うことでフレキシブルになります。
  
`hbox`、`vbox`、``filler` を使用した [例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2vbox_hbox_8cpp-example.html)。

![image](https://user-images.githubusercontent.com/4759106/147242524-7103b5d9-1a92-4e2d-ac70-b3d6740061e3.png)
  
  
gridbox を使用した [例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2gridbox_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147242972-0db1f2e9-0790-496f-86e6-ed2c604f7a73.png)

flexbox を使用した [例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/hflow.cpp):

![image](https://user-images.githubusercontent.com/4759106/147243064-780ac7cc-605b-475f-94b8-cf7c4aed03a5.png)

[こちら](https://arthursonzogni.github.io/FTXUI/examples_2dom_2hflow_8cpp-example.html)や、この[デモ](https://arthursonzogni.github.io/FTXUI/examples/?file=component/flexbox)も参照してください。

</details>

<details><summary>スタイル</summary>

Element は以下の関数を使用して装飾できます。
  - `bold`
  - `italic`
  - `dim`
  - `inverted`
  - `underlined`
  - `underlinedDouble`
  - `blink`
  - `strikethrough`
  - `color`
  - `bgcolor`
  - `hyperlink`

[例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2style_gallery_8cpp-example.html)

![image](https://user-images.githubusercontent.com/4759106/147244118-380bf834-9e33-40df-9ff0-07c10f2598ef.png)
  
FTXUI はパイプ演算子をサポートしています。つまり、`decorator1(decorator2(element))` と `element | decorator1 | decorator2` の両方を使用できます。
  
</details>

<details><summary>色</summary>

FTXUIはすべてのカラーパレットをサポートしています:

カラー[ギャラリー](https://arthursonzogni.github.io/FTXUI/examples_2dom_2color_gallery_8cpp-example.html):
![image](https://user-images.githubusercontent.com/4759106/147248595-04c7245a-5b85-4544-809d-a5984fc6f9e7.png)

</details>
  
<details><summary>ボーダーとセパレーター</summary>

デコレーター `border` と要素 `separator()` を使ってUIを分割します:
  
```cpp
Element document = vbox({
    text("top"),
    separator(),
    text("bottom"),
}) | border;

```

[デモ](https://arthursonzogni.github.io/FTXUI/examples_2dom_2separator_8cpp-example.html):
  
![image](https://user-images.githubusercontent.com/4759106/147244514-4135f24b-fb8e-4067-8896-bc53545583f7.png)
  
</details>

<details><summary>テキストとパラグラフ</summary>

単純なテキストは `text("content")` を使用して表現されます。

スペースに従ったテキストの折り返しをサポートするために、以下の関数が提供されています:
```cpp
Element paragraph(string text);
Element paragraphAlignLeft(string text);
Element paragraphAlignRight(string text);
Element paragraphAlignCenter(string text);
Element paragraphAlignJustify(string text);
```
  
[パラグラフの例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2paragraph_8cpp-example.html)
  
![ezgif com-gif-maker (4)](https://user-images.githubusercontent.com/4759106/147251370-983a06e7-6f41-4113-92b8-942f43d34d06.gif)

</details>

<details><summary>テーブル</summary>

データのテーブルを簡単にスタイル設定するためのクラスです。

[例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2table_8cpp-example.html):
  
![image](https://user-images.githubusercontent.com/4759106/147250766-77d8ec9e-cf2b-486d-9866-1fd9f1bd2e6b.png)

</details>

<details><summary>キャンバス</summary>

`Canvas` 上に、点字、ブロック、または単純な文字を使って描画できます:
  
単純な[例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/canvas.cpp):
  
![image](https://user-images.githubusercontent.com/4759106/147245843-76cc62fb-ccb4-421b-aacf-939f9afb42fe.png)

複雑な [例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/component/canvas_animated.cpp):
  
![ezgif com-gif-maker (3)](https://user-images.githubusercontent.com/4759106/147250538-783a8246-98e0-4a25-b032-3bd3710549d1.gif)
</details>

#### Component

`ftxui/component` はユーザーの入力に反応する動的なUIを生成します。これは `ftxui::Component` の集合を定義します。コンポーネントは `Event`（キーボード、マウス、リサイズなど）に反応し、`Element`（前のセクションを参照）として `Render` されます。

プリビルドされたコンポーネントは [`<ftxui/component/component.hpp>`](https://arthursonzogni.github.io/FTXUI/component_8hpp_source.html) に宣言されています

<details><summary>ギャラリー</summary>

複数のコンポーネントの[ギャラリー](https://arthursonzogni.github.io/FTXUI/examples_2component_2gallery_8cpp-example.html)。([デモ](https://arthursonzogni.github.io/FTXUI/examples/?file=component/gallery))

![image](https://user-images.githubusercontent.com/4759106/147247330-b60beb9f-e665-48b4-81c0-4b01ee95bc66.png)

</details>

<details><summary>ラジオボックス</summary>

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2radiobox_8cpp-example.html):
  
![image](https://user-images.githubusercontent.com/4759106/147246401-809d14a5-6621-4e36-8dd9-a2d75ef2a94e.png)

</details>

<details><summary>チェックボックス</summary>

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2checkbox_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147246646-b86926a9-1ef9-4efb-af98-48a9b62acd81.png)

</details>

<details><summary>入力</summary>

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2input_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147247671-f1d6f606-1845-4e94-a4a0-d4273e9ae6bd.png)

</details>

<details><summary>トグル</summary>

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2toggle_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147249383-e2201cf1-b7b8-4a5a-916f-d761e3e7ae40.png)

</details>


<details><summary>スライダー</summary>

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2slider_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147249265-7e2cad75-082c-436e-affe-44a550c480ab.png)

</details>


<details><summary>メニュー</summary>

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2menu_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147247822-0035fd6f-bb13-4b3a-b057-77eb9291582f.png)

</details>


<details><summary>リサイズ可能な分割</summary>

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2resizable_split_8cpp-example.html):

![ezgif com-gif-maker](https://user-images.githubusercontent.com/4759106/147248372-c55512fe-9b96-4b08-a1df-d05cf2cae431.gif)
</details>


<details><summary>ドロップダウン</summary>

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2dropdown_8cpp-example.html):

![youtube-video-gif (3)](https://user-images.githubusercontent.com/4759106/147246982-1e821751-531c-4e1f-bc37-2fa290e143cd.gif)

</details>

<details><summary>タブ</summary>

[垂直](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_vertical_8cpp-example.html):
  
![ezgif com-gif-maker (1)](https://user-images.githubusercontent.com/4759106/147250144-22ff044a-4773-4ff7-a49c-12ba4034acb4.gif)

[水平](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_horizontal_8cpp-example.html):
  
![ezgif com-gif-maker (2)](https://user-images.githubusercontent.com/4759106/147250217-fe447e0f-7a99-4e08-948a-995087d9b40e.gif)

  

</details>

## FTXUI用ライブラリ
- *FTXUI向けの便利なコンポーネントを共有したいですか？お気軽にこちらに追加してください*
- [ftxui-grid-container](https://github.com/mingsheng13/grid-container-ftxui)
- [ftxui-ip-input](https://github.com/mingsheng13/ip-input-ftxui)
- [ftxui-image-view](https://github.com/ljrrjl/ftxui-image-view.git): 画像表示用。
- [ftxui-image-view-async](https://github.com/orrnithogalum/ftxui-image-view-async.git): オリジナルのフォークで、フレーム再描画ごとの画像再読み込みと変換を行いません。
- [ftxui-navigation-tree](https://github.com/Appisolato/navigation-tree-ftxui)
- [MarkdownFTXUI](https://github.com/zvasilev/MarkdownFTXUI): ターミナル用のMarkdownエディタ/ビューア。


## FTXUI を使用しているプロジェクト

こちらにあなたのプロジェクトを自由に追加してください:
- [json-tui](https://github.com/ArthurSonzogni/json-tui)
- [git-tui](https://github.com/ArthurSonzogni/git-tui)
- [rgb-tui](https://github.com/ArthurSonzogni/rgb-tui)
- [chrome-log-beautifier](https://github.com/ArthurSonzogni/chrome-log-beautifier)
---
- [2048-cpp](https://github.com/Chessom/2048-cpp)
- [BestEdrOfTheMarket](https://github.com/Xacone/BestEdrOfTheMarket)
- [Captain's log](https://github.com/nikoladucak/caps-log)
- [Caravan](https://github.com/r3w0p/caravan)
- [CryptoCalculator](https://github.com/brevis/CryptoCalculator)
- [FTB - tertminal file browser](https://github.com/Cyxuan0311/FTB)
- [FTowerX](https://github.com/MhmRhm/FTowerX)
- [Fallout terminal hacking](https://github.com/gshigin/yet-another-fallout-terminal-hacking-game)
- [Lazylist](https://github.com/zhuyongqi9/lazylist)
- [Memory game](https://github.com/mikolajlubiak/memory)
- [nfolens](https://github.com/a4x7/nfolens)
- [Path Finder](https://github.com/Ruebled/Path_Finder)
- [Pigeon ROS TUI](https://github.com/PigeonSensei/Pigeon_ros_tui)
- [SHOOT!](https://github.com/ShingZhanho/ENGG1340-Project-25Spring)
- [StartUp](https://github.com/StubbornVegeta/StartUp)
- [Step-Writer](https://github.com/BrianAnakPintar/step-writer)
- [TUISIC](https://github.com/Dark-Kernel/tuisic)
- [Terminal Animation](https://github.com/mikolajlubiak/terminal_animation)
- [TimeAccumulator](https://github.com/asari555/TimeAccumulator)
- [UDP chat](https://github.com/Sergeydigl3/udp-chat-tui)
- [VerifySN (Fast Hash Tool)](https://github.com/d06i/verifySN)
- [XJ music](https://github.com/xjmusic/xjmusic)
- [beagle-config](https://github.com/SAtacker/beagle-config)
- [cachyos-cli-installer](https://github.com/cachyos/new-cli-installer)
- [eCAL monitor](https://github.com/eclipse-ecal/ecal)
- [ftxuiFileReader](https://github.com/J0sephDavis/ftxuiFileReader)
- [ftxui_CPUMeter](https://github.com/tzzzzzzzx/ftxui_CPUMeter)
- [hastur](https://github.com/robinlinden/hastur)
- [i3-termdialogs](https://github.com/mibli/i3-termdialogs)
- [inLimbo](https://github.com/nots1dd/inLimbo)
- [keywords](https://github.com/Oakamoore/keywords) ([Web版をプレイ :heart:](https://oakamoore.itch.io/keywords))
- [ltuiny](https://github.com/adrianoviana87/ltuiny)
- [openJuice](https://github.com/mikomikotaishi/openJuice)
- [ostree-tui](https://github.com/AP-Sensing/ostree-tui)
- [OvenbirdBT](https://github.com/ziptt/OvenbirdBT)
- [pciex](https://github.com/s0nx/pciex)
- [resource-monitor](https://github.com/catalincd/resource-monitor)
- [rw-tui](https://github.com/LeeKyuHyuk/rw-tui)
- [simpPRU](https://github.com/VedantParanjape/simpPRU)
- [sweeper](https://www.thomthom.net/thoughts/2026/01/sweeper-a-hat-tip-to-the-simple-fun-games/) ([Web版をプレイ :heart:](https://games.evilsoftwareempire.com/sweeper/play/))
- [tabdeeli](https://github.com/typon/tabdeeli)
- [terminal-rain](https://github.com/Oakamoore/terminal-rain)
- [tic-tac-toe](https://github.com/birland/tic-tac-toe)
- [tiles](https://github.com/tusharpm/tiles)
- [todoman](https://github.com/aaleino/todoman)
- [turing_cmd](https://github.com/DanArmor/turing_cmd)
- [typing-speed-test](https://github.com/ymcx/typing-speed-test)
- [vantage](https://github.com/gokulmaxi/vantage)
- [x86-64 CPU Architecture Simulation](https://github.com/AnisBdz/CPU)
- [C++ Process Manager](https://github.com/ondrejhonus/cpp_proc)
- [Baobab TUI Remake](https://github.com/orrnithogalum/bonsai)
- [Youtube Music Client](https://github.com/orrnithogalum/moroder)
- [TUI_DB](https://github.com/klementii229/TUI_DB)

### [cpp-best-practices/game_jam](https://github.com/cpp-best-practices/game_jam)

ゲームジャム中に FTXUI を使用したいくつかのゲームが作成されました:
- [TermBreaker](https://github.com/ArthurSonzogni/termBreaker) [**[ウェブ版をプレイ]**](https://arthursonzogni.com/TermBreaker/)
- [Minesweeper Marathon](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/minesweeper_marathon.md) [**[ウェブ版をプレイ]**](https://barlasgarden.com/minesweeper/index.html)
- [Grand Rounds](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/grandrounds.md)
- [LightsRound](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/LightsRound.v.0.1.0.md)
- [DanteO](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/danteo.md)
- [Sumo](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/sumo.md)
- [Drag Me aROUND](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/drag_me_around.md)
- [DisarmSelfDestruct](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/DisarmSelfDestruct.md)
- [TheWorld](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/TheWorld.md)
- [smoothlife](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/smoothlife.md)
- [Consu](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/consu.md)


## CMake を使用したビルド

FTXUI に依存するには CMake FetchContent を使用することを**強く**お勧めします。これにより、依存するコミットを指定できます。
```cmake
include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v7.0.3
)
FetchContent_MakeAvailable(ftxui)

target_link_libraries(your_target PRIVATE
    # Use the umbrella target (recommended)
    ftxui::ftxui

    # Or chose a submodule
    # ftxui::component
    # ftxui::dom
    # ftxui::screen
)
```

# Bazel を使用したビルド

**MODULE.bazel**
```starlark
bazel_dep(
    name = "ftxui",
    version = "7.0.3",
)
```

**BUILD.bazel**
```starlark
cc_binary(
    name = "your_target",
    srcs = ["your_source.cc"],
    deps = [
        # Choose submodules
        "@ftxui//:component",
        "@ftxui//:dom",
        "@ftxui//:screen",
        
        # Or use the single ftxui target (includes all modules)
        # "@ftxui//:ftxui",
    ],
)
```

## Mesonでビルドする

FTXUIは[Meson](https://mesonbuild.com/)を使ってビルドすることもできます。詳しい手順は[doc/installation_meson.md](doc/installation_meson.md)を参照してください。

```bash
meson setup builddir
ninja -C builddir
```

MesonプロジェクトでFTXUIをサブプロジェクトとして使用するには、`subprojects/ftxui.wrap`ファイルを作成するか、依存関係として使用してください:
```meson
ftxui_dep = dependency('ftxui-component')
```


# 他の方法でビルドする:
そうでない場合、FTXUIは以下のパッケージから利用できます:
- CMake [FetchContent]([https://bewagner.net/programming/2020/05/02/cmake-fetchcontent/](https://cmake.org/cmake/help/latest/module/FetchContent.html)) （推奨）、
- [Bazel](https://registry.bazel.build/modules/ftxui)、
- [vcpkg](https://vcpkg.link/ports/ftxui)、
- [Conan](https://conan.io/center/recipes/ftxui)
- [Debianパッケージ](https://tracker.debian.org/pkg/ftxui)、
- [Ubuntuパッケージ](https://launchpad.net/ubuntu/+source/ftxui)、
- [Arch Linux](https://aur.archlinux.org/packages/ftxui/)、
- [OpenSUSE](https://build.opensuse.org/package/show/devel:libraries:c_c++/ftxui)、
- [Nix](https://github.com/ArthurSonzogni/FTXUI/blob/main/flake.nix)、

[![Packaging status](https://repology.org/badge/vertical-allrepos/libftxui.svg)](https://repology.org/project/libftxui/versions)


FTXUI を自分でビルドしてリンクする場合、`ftxui-component` は他の FTXUI ライブラリに対してリンク順序で最初にする必要があります。
```bash
g++ . . . -lftxui-component -lftxui-dom -lftxui-screen . . .
```

モジュールで FTXUI をビルドするには、[ドキュメント](https://arthursonzogni.github.io/FTXUI/cpp20-modules.html) を確認してください。

## コントリビューター

<a href="https://github.com/ArthurSonzogni/FTXUI/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=ArthurSonzogni/FTXUI" />
</a>
