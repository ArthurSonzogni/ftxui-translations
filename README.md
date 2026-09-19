
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
  <a href="https://arthursonzogni.github.io/FTXUI/">文件</a> ·
  <a href="https://github.com/ArthurSonzogni/FTXUI/issues">報告錯誤</a> ·
  <a href="https://arthursonzogni.github.io/FTXUI/examples/">範例</a> 。
  <a href="https://github.com/ArthurSonzogni/FTXUI/issues">請求功能</a> ·
  <a href="https://github.com/ArthurSonzogni/FTXUI/pulls">發送拉取請求</a>
  
  <br/>
  文件：
  <a href="https://github.com/ArthurSonzogni/">English</a> |
  <a href="https://arthursonzogni.github.io/FTXUI/fr/index.html">Français</a> |
  <a href="https://arthursonzogni.github.io/FTXUI/es/index.html">Español</a> |
  <a href="https://arthursonzogni.github.io/FTXUI/zh-TW/index.html">繁體中文</a> |
  <a href="https://arthursonzogni.github.io/FTXUI/zh-CH/index.html">简体中文</a> |
  <a href="https://arthursonzogni.github.io/FTXUI/ja/index.html">日本語</a> |

</p>

# FTXUI

<i>功能性終端機 (X) 使用者介面</i>

## 功能
 * 功能性風格。靈感來自
   [1](https://hackernoon.com/building-reactive-terminal-interfaces-in-c-d392ce34e649?gi=d9fb9ce35901)
   和 [React](https://reactjs.org/)
 * 簡潔優雅的語法 (我認為)
 * 鍵盤和滑鼠導航。
 * 支援 [UTF8](https://en.wikipedia.org/wiki/UTF-8) 和 [全形字元](https://en.wikipedia.org/wiki/Halfwidth_and_fullwidth_forms) (→ 測試)
 * 支援動畫。 [演示 1](https://arthursonzogni.github.io/FTXUI/examples/?file=component/menu_underline_animated_gallery), [演示 2](https://arthursonzogni.github.io/FTXUI/examples/?file=component/button_style)
 * 支援繪圖。 [演示](https://arthursonzogni.github.io/FTXUI/examples/?file=component/canvas_animated)
 * 無依賴項。
 * [C++20 模組支援](https://arthursonzogni.github.io/FTXUI/cpp20-modules.html)
 * **跨平台**: Linux/MacOS (主要目標), WebAssembly, Windows (感謝貢獻者！).
 * 透過 [範例](#documentation) 和 [教學](#documentation) 學習
 * 多個套件:
     - CMake [FetchContent]([https://bewagner.net/programming/2020/05/02/cmake-fetchcontent/](https://cmake.org/cmake/help/latest/module/FetchContent.html)) (推薦)
     - [Bazel](https://registry.bazel.build/modules/ftxui)
     - [vcpkg](https://vcpkg.link/ports/ftxui)
     - [Conan](https://conan.io/center/recipes/ftxui) [Debian 套件](https://tracker.debian.org/pkg/ftxui)
     - [Ubuntu 套件](https://launchpad.net/ubuntu/+source/ftxui)
     - [Arch Linux](https://aur.archlinux.org/packages/ftxui/)
    - [OpenSUSE](https://build.opensuse.org/package/show/devel:libraries:c_c++/ftxui)
    - [XMake](https://xmake.io) 倉庫 [套件](https://github.com/xmake-io/xmake-repo/blob/dev/packages/f/ftxui/xmake.lua)
    - [Nix](https://github.com/ArthurSonzogni/FTXUI/blob/main/flake.nix)
 * 良好實踐: 文件、測試、模糊測試器、效能測試、自動化 CI、自動化打包等...

## 特色
 * 函數式風格。靈感來自
   [1](https://hackernoon.com/building-reactive-terminal-interfaces-in-c-d392ce34e649?gi=d9fb9ce35901)
   與 [React](https://reactjs.org/)
 * 簡潔優雅的語法（依我之見）
 * 鍵盤與滑鼠導覽。
 * 支援 [UTF8](https://en.wikipedia.org/wiki/UTF-8) 與 [全形字元](https://en.wikipedia.org/wiki/Halfwidth_and_fullwidth_forms)（→ 测试）
 * 支援動畫。[Demo 1](https://arthursonzogni.github.io/FTXUI/examples/?file=component/menu_underline_animated_gallery)，[Demo 2](https://arthursonzogni.github.io/FTXUI/examples/?file=component/button_style)
 * 支援繪圖。[Demo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/canvas_animated)
 * 無相依性。
 * [C++20 模組支援](https://arthursonzogni.github.io/FTXUI/cpp20-modules.html)
 * **跨平台**：Linux/MacOS（主要目標）、WebAssembly、Windows（感謝貢獻者！）。
 * 透過[範例](#documentation)與[教學](#documentation)學習
 * 多種套件：
     - CMake [FetchContent]([https://bewagner.net/programming/2020/05/02/cmake-fetchcontent/](https://cmake.org/cmake/help/latest/module/FetchContent.html))（推薦）
     - [Bazel](https://registry.bazel.build/modules/ftxui)
     - [vcpkg](https://vcpkg.link/ports/ftxui)
     - [Conan](https://conan.io/center/recipes/ftxui) [Debian 套件](https://tracker.debian.org/pkg/ftxui)
     - [Ubuntu 套件](https://launchpad.net/ubuntu/+source/ftxui)
     - [Arch Linux](https://aur.archlinux.org/packages/ftxui/)
    - [OpenSUSE](https://build.opensuse.org/package/show/devel:libraries:c_c++/ftxui)
    - [XMake](https://xmake.io) 儲存庫[套件](https://github.com/xmake-io/xmake-repo/blob/dev/packages/f/ftxui/xmake.lua)
    - [Nix](https://github.com/ArthurSonzogni/FTXUI/blob/main/flake.nix)
    - [合併版（單一標頭／原始碼）](https://arthursonzogni.github.io/FTXUI/installation_amalgamated.html)（自 7.0.0 起）
    - [conda-forge](https://anaconda.org/channels/conda-forge/packages/ftxui/overview)
 * 良好實踐：文件、測試、模糊測試、效能測試、自動化 CI、自動化打包等等...

## 範例
~~~cpp
    vbox({
      hbox({
        text("one") | border,
        text("two") | border | flex,
        text("three") | border | flex,
      }),

- [Starter CMake](https://github.com/ArthurSonzogni/ftxui-starter)
- [Starter Bazel](https://github.com/ArthurSonzogni/ftxui-bazel)
- [文件](https://arthursonzogni.github.io/FTXUI/)
- [範例（WebAssembly）](https://arthursonzogni.github.io/FTXUI/examples/)
- [使用 CMake 建置](https://arthursonzogni.github.io/FTXUI/#build-cmake)
- [使用 Bazel 建置](https://arthursonzogni.github.io/FTXUI/#build-bazel)
- [使用 Meson 建置](doc/installation_meson.md)

## 範例
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

## 簡短展示

FTXUI 分為三個模組：
1.  **screen**：低階渲染（顏色、像素、終端機）
2.  **dom**：版面與組合（階層式元素）
3.  **component**：使用者互動（元件、事件、主迴圈）

對大多數使用者而言，一次包含所有內容是最簡單的入門方式：
- 標頭：`#include <ftxui/ftxui.hpp>`
- CMake 目標：`ftxui::ftxui`
- Bazel 目標：`@ftxui//:ftxui`

#### DOM

此模組定義了一組階層式的 `Element`。`Element` 負責管理版面配置，並可對終端機尺寸做出響應。

它們宣告於 [`<ftxui/dom/elements.hpp>`](https://arthursonzogni.github.io/FTXUI/elements_8hpp_source.html
)
  
<details><summary>佈局</summary>

`Element` 可以彼此組合排列：
  - 使用 `hbox` 水平排列
  - 使用 `vbox` 垂直排列
  - 使用 `gridbox` 排列於網格中
  - 使用 `flexbox` 沿單一方向換行。
  
`Element` 可透過 `flex` 修飾器變為可伸縮的。
  
[範例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2vbox_hbox_8cpp-example.html) 使用 `hbox`、`vbox` 和 `filler`。

![image](https://user-images.githubusercontent.com/4759106/147242524-7103b5d9-1a92-4e2d-ac70-b3d6740061e3.png)
  
  
[範例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2gridbox_8cpp-example.html) 使用 gridbox：

![image](https://user-images.githubusercontent.com/4759106/147242972-0db1f2e9-0790-496f-86e6-ed2c604f7a73.png)

[範例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/hflow.cpp) 使用 flexbox：

![image](https://user-images.githubusercontent.com/4759106/147243064-780ac7cc-605b-475f-94b8-cf7c4aed03a5.png)

請參閱 [此處](https://arthursonzogni.github.io/FTXUI/examples_2dom_2hflow_8cpp-example.html) 還有這個 [演示](https://arthursonzogni.github.io/FTXUI/examples/?file=component/flexbox)。

</details>

<details><summary>樣式</summary>

Element 可以使用以下函數進行裝飾：
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

[範例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2style_gallery_8cpp-example.html)

![image](https://user-images.githubusercontent.com/4759106/147244118-380bf834-9e33-40df-9ff0-07c10f2598ef.png)
  
FTXUI 支援管道運算符。這表示：`decorator1(decorator2(element))` 和 `element | decorator1 | decorator2` 都可以使用。
  
</details>

<details><summary>顏色</summary>

FTXUI 支援各種色彩調色盤：

顏色 [圖庫](https://arthursonzogni.github.io/FTXUI/examples_2dom_2color_gallery_8cpp-example.html)：
![image](https://user-images.githubusercontent.com/4759106/147248595-04c7245a-5b85-4544-809d-a5984fc6f9e7.png)

</details>
  
<details><summary>邊框和分隔符</summary>

使用修飾器 `border` 與元素 `separator()` 來分割您的 UI：
  
```cpp
Element document = vbox({
    text("top"),
    separator(),
    text("bottom"),
}) | border;

```

[演示](https://arthursonzogni.github.io/FTXUI/examples_2dom_2separator_8cpp-example.html)：
  
![image](https://user-images.githubusercontent.com/4759106/147244514-4135f24b-fb8e-4067-8896-bc53545583f7.png)
  
</details>

<details><summary>文本和段落</summary>

一段簡單的文字可使用 `text("content")` 表示。

為支援依空白處換行的文字包裝，提供了以下函式：
```cpp
Element paragraph(string text);
Element paragraphAlignLeft(string text);
Element paragraphAlignRight(string text);
Element paragraphAlignCenter(string text);
Element paragraphAlignJustify(string text);
```
  
[段落範例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2paragraph_8cpp-example.html)
  
![ezgif com-gif-maker (4)](https://user-images.githubusercontent.com/4759106/147251370-983a06e7-6f41-4113-92b8-942f43d34d06.gif)

</details>

<details><summary>表格</summary>

一個方便為資料表格設計樣式的類別。

[範例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2table_8cpp-example.html)：
  
![image](https://user-images.githubusercontent.com/4759106/147250766-77d8ec9e-cf2b-486d-9866-1fd9f1bd2e6b.png)

</details>

<details><summary>畫布</summary>

可在 `Canvas` 上進行繪圖，使用點字、方塊或簡單字元：
  
簡單的 [範例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/canvas.cpp)：
  
![image](https://user-images.githubusercontent.com/4759106/147245843-76cc62fb-ccb4-421b-aacf-939f9afb42fe.png)

複雜的 [範例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/component/canvas_animated.cpp)：
  
![ezgif com-gif-maker (3)](https://user-images.githubusercontent.com/4759106/147250538-783a8246-98e0-4a25-b032-3bd3710549d1.gif)  
</details>

#### Component

`ftxui/component` 產生動態的 UI，能對使用者輸入做出反應。它定義了一組 `ftxui::Component`。元件會回應 `Event`（鍵盤、滑鼠、大小調整……）並以 `Element` 的形式呈現（見前一節）。

預先建置的元件宣告於 [`<ftxui/component/component.hpp>`](https://arthursonzogni.github.io/FTXUI/component_8hpp_source.html)

<details><summary>圖庫</summary>

[畫廊](https://arthursonzogni.github.io/FTXUI/examples_2component_2gallery_8cpp-example.html) 多個元件的集合。 ([demo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/gallery))

![image](https://user-images.githubusercontent.com/4759106/147247330-b60beb9f-e665-48b4-81c0-4b01ee95bc66.png)

</details>

<details><summary>單選框</summary>

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2radiobox_8cpp-example.html)：
  
![image](https://user-images.githubusercontent.com/4759106/147246401-809d14a5-6621-4e36-8dd9-a2d75ef2a94e.png)

</details>

<details><summary>複選框</summary>

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2checkbox_8cpp-example.html)：

![image](https://user-images.githubusercontent.com/4759106/147246646-b86926a9-1ef9-4efb-af98-48a9b62acd81.png)

</details>

<details><summary>輸入</summary>

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2input_8cpp-example.html)：

![image](https://user-images.githubusercontent.com/4759106/147247671-f1d6f606-1845-4e94-a4a0-d4273e9ae6bd.png)

</details>

<details><summary>切換開關</summary>

![image](https://user-images.githubusercontent.com/4759106/147249265-7e2cad75-082c-436e-affe-44a550c480ab.png)

由 `ftxui::Slider()` 從 "ftxui/component/component.hpp" 生成。

</details>


<details><summary>滑塊</summary>

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2slider_8cpp-example.html)：

![image](https://user-images.githubusercontent.com/4759106/147249265-7e2cad75-082c-436e-affe-44a550c480ab.png)

</details>


<details><summary>菜單</summary>

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2dropdown_8cpp-example.html)：

![youtube-video-gif (3)](https://user-images.githubusercontent.com/4759106/147246982-1e821751-531c-4e1f-bc37-2fa290e143cd.gif)

</details>


<details><summary>可調整大小分割</summary>

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2resizable_split_8cpp-example.html)：

![ezgif com-gif-maker](https://user-images.githubusercontent.com/4759106/147248372-c55512fe-9b96-4b08-a1df-d05cf2cae431.gif)  
</details>


<details><summary>下拉選單</summary>

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2dropdown_8cpp-example.html)：

![youtube-video-gif (3)](https://user-images.githubusercontent.com/4759106/147246982-1e821751-531c-4e1f-bc37-2fa290e143cd.gif)

</details>

<details><summary>分頁</summary>

[垂直](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_vertical_8cpp-example.html)：
  
![ezgif com-gif-maker (1)](https://user-images.githubusercontent.com/4759106/147250144-22ff044a-4773-4ff7-a49c-12ba4034acb4.gif)

[水平](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_horizontal_8cpp-example.html)：
  
  ![ezgif com-gif-maker (2)](https://user-images.githubusercontent.com/4759106/147250217-fe447e0f-7a99-4e08-948a-995087d9b40e.gif)

  

</details>

## 適用於 FTXUI 的函式庫
- *想分享實用的 FTXUI 元件嗎？歡迎在此新增您的作品*
- [ftxui-grid-container](https://github.com/mingsheng13/grid-container-ftxui)
- [ftxui-ip-input](https://github.com/mingsheng13/ip-input-ftxui)
- [ftxui-image-view](https://github.com/ljrrjl/ftxui-image-view.git)：用於圖片顯示。
- [ftxui-image-view-async](https://github.com/orrnithogalum/ftxui-image-view-async.git)：原版的分支，每次畫面重繪時不再重新載入與轉換圖片。
- [ftxui-navigation-tree](https://github.com/Appisolato/navigation-tree-ftxui)
- [MarkdownFTXUI](https://github.com/zvasilev/MarkdownFTXUI)：終端機用的 Markdown 編輯／檢視器。


## 使用 FTXUI 的專案

歡迎在此新增您的專案：
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
- [keywords](https://github.com/Oakamoore/keywords)（[線上試玩 :heart:](https://oakamoore.itch.io/keywords)）
- [ltuiny](https://github.com/adrianoviana87/ltuiny)
- [openJuice](https://github.com/mikomikotaishi/openJuice)
- [ostree-tui](https://github.com/AP-Sensing/ostree-tui)
- [OvenbirdBT](https://github.com/ziptt/OvenbirdBT)
- [pciex](https://github.com/s0nx/pciex)
- [resource-monitor](https://github.com/catalincd/resource-monitor)
- [rw-tui](https://github.com/LeeKyuHyuk/rw-tui)
- [simpPRU](https://github.com/VedantParanjape/simpPRU)
- [sweeper](https://www.thomthom.net/thoughts/2026/01/sweeper-a-hat-tip-to-the-simple-fun-games/)（[線上試玩 :heart:](https://games.evilsoftwareempire.com/sweeper/play/)）
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

在 Game Jam 期間，已經製作了幾款使用 FTXUI 的遊戲：
- [TermBreaker](https://github.com/ArthurSonzogni/termBreaker) [**[玩網頁版]**](https://arthursonzogni.com/TermBreaker/)
- [Minesweeper Marathon](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/minesweeper_marathon.md) [**[玩網頁版]**](https://barlasgarden.com/minesweeper/index.html)
- [Grand Rounds](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/grandrounds.md)
- [LightsRound](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/LightsRound.v.0.1.0.md)
- [DanteO](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/danteo.md)
- [Sumo](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/sumo.md)
- [Drag Me aROUND](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/drag_me_around.md)
- [DisarmSelfDestruct](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/DisarmSelfDestruct.md)
- [TheWorld](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/TheWorld.md)
- [smoothlife](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/smoothlife.md)
- [Consu](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/consu.md)


## 使用 CMake 建置

**強烈建議**使用 CMake FetchContent 依賴 FTXUI，這樣您就可以指定要依賴的提交。
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

# 使用 Bazel 建置

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

## 使用 Meson 建置

FTXUI 也可以使用 [Meson](https://mesonbuild.com/) 建置。詳細說明請參閱 [doc/installation_meson.md](doc/installation_meson.md)。

```bash
meson setup builddir
ninja -C builddir
```

若要在您的 Meson 專案中將 FTXUI 作為子專案使用，請建立 `subprojects/ftxui.wrap` 檔案，或將其作為相依項使用：
```meson
ftxui_dep = dependency('ftxui-component')
```


# 使用其他方式建置：
若您不這麼做，FTXUI 也可以從以下套件取得：
- CMake [FetchContent]([https://bewagner.net/programming/2020/05/02/cmake-fetchcontent/](https://cmake.org/cmake/help/latest/module/FetchContent.html))（推薦），
- [Bazel](https://registry.bazel.build/modules/ftxui)，
- [vcpkg](https://vcpkg.link/ports/ftxui)，
- [Conan](https://conan.io/center/recipes/ftxui)
- [Debian 套件](https://tracker.debian.org/pkg/ftxui)，
- [Ubuntu 套件](https://launchpad.net/ubuntu/+source/ftxui)，
- [Arch Linux](https://aur.archlinux.org/packages/ftxui/)，
- [OpenSUSE](https://build.opensuse.org/package/show/devel:libraries:c_c++/ftxui)，
- [Nix](https://github.com/ArthurSonzogni/FTXUI/blob/main/flake.nix)，

[![Packaging status](https://repology.org/badge/vertical-allrepos/libftxui.svg)](https://repology.org/project/libftxui/versions)


如果您選擇自己建置和連結 FTXUI，`ftxui-component` 必須相對於其他 FTXUI 函式庫在連結順序中排在最前面，即
```bash
g++ . . . -lftxui-component -lftxui-dom -lftxui-screen . . .
```

要使用模組建置 FTXUI，請查看 [文件](https://arthursonzogni.github.io/FTXUI/cpp20-modules.html)

## 貢獻者

<a href="https://github.com/ArthurSonzogni/FTXUI/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=ArthurSonzogni/FTXUI" />
</a>
