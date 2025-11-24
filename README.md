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

</p>

# FTXUI

<i>功能性終端機 (X) 使用者介面</i>

一個簡單的跨平台 C++ 函式庫，用於基於終端機的使用者介面！

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

## 文件

- [入門 CMake](https://github.com/ArthurSonzogni/ftxui-starter)
- [入門 Bazel](https://github.com/ArthurSonzogni/ftxui-bazel)
- [文件](https://arthursonzogni.github.io/FTXUI/)
- [範例 (WebAssembly)](https://arthursonzogni.github.io/FTXUI/examples/)
- [使用 CMake 建置](https://arthursonzogni.github.io/FTXUI/#build-cmake)
- [使用 Bazel 建置](https://arthursonzogni.github.io/FTXUI/#build-bazel)

## 範例
~~~cpp
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
~~~

![image](https://github.com/ArthurSonzogni/FTXUI/assets/4759106/569bf043-4e85-4245-aad5-2324572135c4)

## 簡短圖庫

#### DOM

此模組定義了一組分層的 Element。Element 管理佈局並且可以響應終端尺寸。

它們在 [<ftxui/dom/elements.hpp>](https://arthursonzogni.github.io/FTXUI/elements_8hpp_source.html
)
  
<details><summary>佈局</summary>

Element 可以組合在一起：
  - 使用 `hbox` 水平排列
  - 使用 `vbox` 垂直排列
  - 使用 `gridbox` 在網格中排列
  - 使用 `flexbox` 沿一個方向換行
  
Element 可以使用 `flex` 裝飾器變得靈活。
  
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

FTXUI 支援所有調色板：

顏色 [圖庫](https://arthursonzogni.github.io/FTXUI/examples_2dom_2color_gallery_8cpp-example.html)：
![image](https://user-images.githubusercontent.com/4759106/147248595-04c7245a-5b85-4544-809d-a5984fc6f9e7.png)

</details>
  
<details><summary>邊框和分隔符</summary>

使用裝飾器 `border` 和元素 `separator()` 來細分您的 UI：
  
```cpp
auto document = vbox({
    text("top"),
    separator(),
    text("bottom"),
}) | border;

```

[演示](https://arthursonzogni.github.io/FTXUI/examples_2dom_2separator_8cpp-example.html)：
  
![image](https://user-images.githubusercontent.com/4759106/147244514-4135f24b-fb8e-4067-8896-bc53545583f7.png)
  
</details>

<details><summary>文本和段落</summary>

A simple piece of text is represented using `text("content")`.

為了支援依照空格自動換行，提供了以下函數：
```cpp
Element paragraph(std::string text);
Element paragraphAlignLeft(std::string text);
Element paragraphAlignRight(std::string text);
Element paragraphAlignCenter(std::string text);
Element paragraphAlignJustify(std::string text);
```
  
[段落範例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2paragraph_8cpp-example.html)
  
![ezgif com-gif-maker (4)](https://user-images.githubusercontent.com/4759106/147251370-983a06e7-6f41-4113-92b8-942f43d34d06.gif)

</details>

<details><summary>表格</summary>

A class to easily style a table of data.

[範例](https://arthursonzogni.github.io/FTXUI/examples_2dom_2table_8cpp-example.html)：
  
![image](https://user-images.githubusercontent.com/4759106/147250766-77d8ec9e-cf2b-486d-9866-1fd9f1bd2e6b.png)

</details>

<details><summary>畫布</summary>

Drawing can be made on a Canvas, using braille, block, or simple characters:
  
簡單的 [範例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/canvas.cpp)：
  
![image](https://user-images.githubusercontent.com/4759106/147245843-76cc62fb-ccb4-421b-aacf-939f9afb42fe.png)

複雜的 [範例](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/component/canvas_animated.cpp)：
  
![ezgif com-gif-maker (3)](https://user-images.githubusercontent.com/4759106/147250538-783a8246-98e0-4a25-b032-3bd3710549d1.gif)  
</details>

#### 元件

ftxui/component 產生動態 UI，對使用者的輸入做出反應。它定義了一組 ftxui::Component。一個元件對事件（鍵盤、滑鼠、調整大小等）做出反應，並渲染為 Element（參見上一節）。

預建元件在 [<ftxui/component/component.hpp>](https://arthursonzogni.github.io/FTXUI/component_8hpp_source.html) 中聲明

<details><summary>圖庫</summary>

多個元件的 [圖庫](https://arthursonzogni.github.io/FTXUI/examples_2component_2gallery_8cpp-example.html)。 ([演示](https://arthursonzogni.github.io/FTXUI/examples/?file=component/gallery))

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

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2toggle_8cpp-example.html)：

![image](https://user-images.githubusercontent.com/4759106/147249383-e2201cf1-b7b8-4a5a-916f-d761e3e7ae40.png)

</details>


<details><summary>滑塊</summary>

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2slider_8cpp-example.html)：

![image](https://user-images.githubusercontent.com/4759106/147249265-7e2cad75-082c-436e-affe-44a550c480ab.png)

</details>


<details><summary>菜單</summary>

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2menu_8cpp-example.html)：

![image](https://user-images.githubusercontent.com/4759106/147247822-0035fd6f-bb13-4b3a-b057-77eb9291582f.png)

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
- *想要分享 FTXUI 的實用元件嗎？請隨時在此處添加您的元件*
- [ftxui-grid-container](https://github.com/mingsheng13/grid-container-ftxui)
- [ftxui-ip-input](https://github.com/mingsheng13/ip-input-ftxui)
- [ftxui-image-view](https://github.com/ljrrjl/ftxui-image-view.git): 用於圖像顯示。


## 使用 FTXUI 的專案

請隨時在此處添加您的專案：
- [json-tui](https://github.com/ArthurSonzogni/json-tui)
- [git-tui](https://github.com/ArthurSonzogni/git-tui)
- [ostree-tui](https://github.com/AP-Sensing/ostree-tui)
- [rgb-tui](https://github.com/ArthurSonzogni/rgb-tui)
- [chrome-log-beautifier](https://github.com/ArthurSonzogni/chrome-log-beautifier)
- [x86-64 CPU 架構模擬](https://github.com/AnisBdz/CPU)
- [ltuiny](https://github.com/adrianoviana87/ltuiny)
- [i3-termdialogs](https://github.com/mibli/i3-termdialogs)
- [simpPRU](https://github.com/VedantParanjape/simpPRU)
- [Pigeon ROS TUI](https://github.com/PigeonSensei/Pigeon_ros_tui)
- [hastur](https://github.com/robinlinden/hastur)
- [CryptoCalculator](https://github.com/brevis/CryptoCalculator)
- [todoman](https://github.com/aaleino/todoman)
- [TimeAccumulator](https://github.com/asari555/TimeAccumulator)
- [vantage](https://github.com/gokulmaxi/vantage)
- [tabdeeli](https://github.com/typon/tabdeeli)
- [tiles](https://github.com/tusharpm/tiles)
- [cachyos-cli-installer](https://github.com/cachyos/new-cli-installer)
- [beagle-config](https://github.com/SAtacker/beagle-config)
- [turing_cmd](https://github.com/DanArmor/turing_cmd)
- [StartUp](https://github.com/StubbornVegeta/StartUp)
- [eCAL monitor](https://github.com/eclipse-ecal/ecal)
- [Path Finder](https://github.com/Ruebled/Path_Finder)
- [rw-tui](https://github.com/LeeKyuHyuk/rw-tui)
- [resource-monitor](https://github.com/catalincd/resource-monitor)
- [ftxuiFileReader](https://github.com/J0sephDavis/ftxuiFileReader)
- [ftxui_CPUMeter](https://github.com/tzzzzzzzx/ftxui_CPUMeter)
- [Captain's log](https://github.com/nikoladucak/caps-log)
- [FTowerX](https://github.com/MhmRhm/FTowerX)
- [Caravan](https://github.com/r3w0p/caravan)
- [Step-Writer](https://github.com/BrianAnakPintar/step-writer)
- [XJ music](https://github.com/xjmusic/xjmusic)
- [UDP chat](https://github.com/Sergeydigl3/udp-chat-tui)
- [2048-cpp](https://github.com/Chessom/2048-cpp)
- [Memory game](https://github.com/mikolajlubiak/memory)
- [Terminal Animation](https://github.com/mikolajlubiak/terminal_animation)
- [pciex](https://github.com/s0nx/pciex)
- [Fallout terminal hacking](https://github.com/gshigin/yet-another-fallout-terminal-hacking-game)
- [Lazylist](https://github.com/zhuyongqi9/lazylist)
- [TUISIC](https://github.com/Dark-Kernel/tuisic)
- [inLimbo](https://github.com/nots1dd/inLimbo)
- [BestEdrOfTheMarket](https://github.com/Xacone/BestEdrOfTheMarket)
- [terminal-rain](https://github.com/Oakamoore/terminal-rain)
- [keywords](https://github.com/Oakamoore/keywords) ([玩網頁版 :heart:](https://oakamoore.itch.io/keywords))
- [FTB - 終端檔案瀏覽器](https://github.com/Cyxuan0311/FTB)
- [openJuice](https://github.com/mikomikotaishi/openJuice)
- [SHOOT!](https://github.com/ShingZhanho/ENGG1340-Project-25Spring)
- [VerifySN (快速雜湊工具)](https://github.com/d06i/verifySN)
- [tic-tac-toe](https://github.com/birland/tic-tac-toe)
- [打字速度測試](https://github.com/ymcx/typing-speed-test)

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
  GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

target_link_libraries(your_target PRIVATE
    # Chose a submodule
    ftxui::component
    ftxui::dom
    ftxui::screen
)
```

# 使用 Bazel 建置

**MODULE.bazel**
```starlark
bazel_dep(
    name = "ftxui",
    version = "v6.1.9",
)
```

**BUILD.bazel**
```starlark
cc_binary(
    name = "your_target",
    srcs = ["your_source.cc"],
    deps = [
        "@ftxui//:component",
        "@ftxui//:dom",
        "@ftxui//:screen",
    ],
)
```


# 使用其他方式建置：
如果您不這樣做，FTXUI 可以從以下套件中使用：
- CMake [FetchContent]([https://bewagner.net/programming/2020/05/02/cmake-fetchcontent/](https://cmake.org/cmake/help/latest/module/FetchContent.html)) (推薦)，
- [Bazel](https://registry.bazel.build/modules/ftxui)，
- [vcpkg](https://vcpkg.link/ports/ftxui)，
- [Conan](https://conan.io/center/recipes/ftxui)
- [Debian 套件](https://tracker.debian.org/pkg/ftxui)，
- [Ubuntu 套件](https://launchpad.net/ubuntu/+source/ftxui)，
- [Arch Linux](https://aur.archlinux.org/packages/ftxui/)，
- [OpenSUSE](https://build.opensuse.org/package/show/devel:libraries:c_c++/ftxui)，
[Nix](https://github.com/ArthurSonzogni/FTXUI/blob/main/flake.nix)，
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