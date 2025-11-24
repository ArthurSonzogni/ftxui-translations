更新日誌
=========

下一個
====

### 文件
- 修復損壞的 Doxygen 輸出。請參閱 #1029 中的 @markmandel。
- 使用 Doxygen awesome。添加我們自己的主題。
- 將文件分成幾個頁面。

### 建置
- 功能：支援 C++20 模組。
  這需要：
  - 使用 Ninja 或 MSVC 生成器
  - 最新的 Clang/GCC/MSVC 編譯器。
  - Cmake 3.28 或更高版本。
  用法：
  ```cpp
  import ftxui;
  import ftxui.component;
  import ftxui.dom;
  import ftxui.screen;
  import ftxui.util;
  ```
  感謝 @mikomikotaishi 在 PR #1015 中的貢獻。
- 移除對 'pthread' 的依賴。

### 組件
- 功能：POSIX 管道輸入處理。
  - 允許 FTXUI 應用程式在接收終端鍵盤輸入的同時，從 stdin（通過管道時）讀取數據。
  - 默認啟用。
  - 可以使用 `ScreenInteractive::HandlePipedInput(false)` 禁用。
  - 僅適用於 Linux 和 macOS。
  感謝 @HarryPehkonen 在 PR #1094 中的貢獻。
- 修復 ScreenInteractive::FixedSize 螢幕覆蓋前面終端輸出問題。感謝 #1064 中的 @zozowell。
- 修復垂直 `ftxui::Slider`。以前 "up" 鍵會減小數值。感謝 #1093 中的 @its-pablo 報告此問題。

### Dom
- 修復 `ComputeShrinkHard` 中的整數溢出。感謝 #1137 中的 @its-pablo 報告並修復此問題。
- 為 `vbox/hbox/dbox` 添加特化，以允許將 Element 容器作為輸入。感謝 #1117 中的 @nbusser。

6.1.9 (2025-05-07)
------------

### 建置
如果一切順利（待定），ftxui 將會出現在 Bazel 中央儲存庫中。
可以使用以下行將其導入到您的專案中：

**MODULE.bazel**
```bazel
bazel_dep(name = "ftxui", version = "6.1.9")
```

感謝 @robinlinden 和 @kcc 的審閱。

### dom
- 錯誤修正：恢復 ftxui 5.0.0 中 `dbox` 的行為。為了在兩個圖層之間應用 bgcolor 混合，將添加一個新的 `dboxBlend`。

6.1.8 (2025-05-01)
------------------

### 建置
- 功能：支援 `bazel` 建置系統。請參閱 #1032。
  由 Kostya Serebryany @kcc 提出

  **BUILD.bazel**
  ```bazel
  deps = [
    // Depend on the whole library:
    "@ftxui//:ftxui",

    // Choose a specific submodule:
    "@ftxui//:component",
    "@ftxui//:dom",
    "@ftxui//:screen",
  ]
  ```

### 組件
- 錯誤修正：修復 ResizeableSplit 的崩潰問題。請參閱 #1023。
  - 將螢幕尺寸限制為終端尺寸。
  - 禁止 `ResizeableSplit` 使用負數尺寸。

### Dom
- 錯誤修正：禁止指定負數尺寸約束。請參閱 #1023。


6.0.2 (2025-03-30)
-----

### 組件
- 錯誤修正：修復 Windows 上影響所有組件的重大崩潰。請參閱 #1020
- 錯誤修正：修復 focusRelative。

6.0.1 (2025-03-28)
-----

與 v6.0.0 相同。

由於問題標籤 v6.0.0 被替換了。這不是一個好的做法，會影響在短期內開始使用它的開發人員。提交一個內容相同的新版本是解決此問題的最佳方法。

請參閱 #1017 和 #1019。

6.0.0 (2025-03-23)
-----

### 組件
- 功能：添加對原始輸入的支援。允許檢測更多按鍵。
- 功能：添加 `ScreenInteractive::ForceHandleCtrlC(false)` 以允許組件完全覆蓋默認的 `Ctrl+C` 處理程序。
- 功能：添加 `ScreenInteractive::ForceHandleCtrlZ(false)` 以允許組件完全覆蓋默認的 `Ctrl+Z` 處理程序。
- 功能：在支援的終端上添加 `Mouse::WeelLeft` 和 `Mouse::WeelRight` 事件。
- 功能：添加 `Event::DebugString()`。
- 功能：添加對 `Input` 插入模式的支援。添加 `InputOption::insert` 選項。由 @mingsheng13 添加。
- 功能：添加 `DropdownOption` 以配置下拉選單。請參閱 #826。
- 功能：添加對選擇的支援。感謝 @clement-roblot。請參閱 #926。
  - 請參閱 `ScreenInteractive::GetSelection()`。
  - 請參閱 `ScreenInteractive::SelectionChange(...)` 監聽器。
- 錯誤修正/破壞性變更：`Mouse transition`：
  - 檢測滑鼠移動，而不是按下。
    新增了 Mouse::Moved 動作。
  - 按住左鍵拖動滑鼠現在可以避免啟用多個複選框。
  - 現在，當滑鼠按下時，一些組件會被激活，而不是在釋放時激活。
  這修復了：https://github.com/ArthurSonzogni/FTXUI/issues/773
  這修復了：https://github.com/ArthurSonzogni/FTXUI/issues/792
- 錯誤修正：mouse.control 現在報告正確。
- 功能：添加 `ScreenInteractive::FullscreenPrimaryScreen()`。這允許在主螢幕上顯示全螢幕組件，而不是在輔助螢幕上。
- 錯誤修正：`Input` `onchange` 在退格鍵或刪除鍵時未被呼叫。
  由 @chrysante 在 PR #776 中修復。
- 錯誤修正：退出時正確恢復游標形狀。請參閱 #792。
- 錯誤修正：修復游標在最後一列時的位置。請參閱 #831。
- 錯誤修正：修復 `ResizeableSplit` 鍵盤導航。由 #842 修復。
- 錯誤修正：修復 `Menu` 焦點。請參閱 #841
- 功能：添加 `ComponentBase::Index()`。這允許獲取組件在其父級中的索引。請參閱 #932
- 功能：添加 `EntryState::index`。這允許獲取菜單項的索引。請參閱 #932
- 功能：添加 `SliderOption::on_change`。這允許在滑塊值更改時設置回調。請參閱 #938。
- 錯誤修正：處理沒有條目的 `Dropdown`。
- 錯誤修正：修復 `LinearGradient` 中由於浮點精度和差一錯誤導致的崩潰。請參閱 #998。

### Dom
- 功能：添加 `italic` 裝飾器。例如：
  ```cpp
  auto italic_text = text("Italic text") | italic;
  ```
  ```cpp
  auto italic_text = italic(text("Italic text"));
  ```
  由 @kenReneris 在 #1009 中提出。
- 功能：添加 `hscroll_indicator`。它顯示一個反映當前滾動位置的水平指示器。由 @ibrahimnasson 在 [issue 752](https://github.com/ArthurSonzogni/FTXUI/issues/752) 中提出
- 功能：為 `Dimension::Fit(..)` 添加 `extend_beyond_screen` 選項，允許元素大於螢幕。由 @LordWhiro 提出。請參閱 #572 和 #949。
- 功能：添加對選擇的支援。感謝 @clement-roblot。請參閱 #926。
  - 請參閱 `selectionColor` 裝飾器。
  - 請參閱 `selectionBackgroundColor` 裝飾器。
  - 請參閱 `selectionForegroundColor` 裝飾器。
  - 請參閱 `selectionStyle(style)` 裝飾器。
  - 請參閱 `selectionStyleReset` 裝飾器。
- 破壞性變更：更改 "focus"/"select" 的處理方式。這修復了行為。
- 破壞性變更：`Component::OnRender()` 成為覆寫以渲染組件的方法。這取代了仍然用於在子項上呼叫渲染方法的 `Component::Render()`。此更改可以修復一些圍繞焦點處理的問題。

### 螢幕
- 功能：添加 `Box::IsEmpty()`。
- 功能：顏色透明度
    - 添加 `Color::RGBA(r,g,b,a)`。
    - 添加 `Color::HSVA(r,g,b,a)`。
    - 添加 `Color::Blend(Color)`。
    - 添加 `Color::IsOpaque()`

### 工具
- 功能：支援 `ConstStringListRef` 的任意 `Adapter`。請參閱 #843。

### 建置
- 支援 cmake 的 "unity/jumbo" 建置。由 @ClausKlein 修復。

5.0.0
-----

### 組件
- 破壞性變更：MenuDirection enum 已重新命名為 Direction
- 破壞性變更：GaugeDirection enum 已重新命名為 Direction
- 破壞性變更：Direction enum 已重新命名為 WidthOrHeight
- 破壞性變更：移除 `ComponentBase` 複製建構子/賦值。
- 破壞性變更：MenuOption::entries 已重新命名為 MenuOption::entries_option。
- 破壞性變更：`Ref<{Component}Option>` 在組件建構子中變為 `{Component}Option`。
- 功能：`ResizeableSplit` 現在支援任意元素作為分隔符。
- 功能：`input` 現在支援多行。
- 功能：`input` 樣式現在可自定義。
- 錯誤修正：支援來自作業系統終端的 F1-F5。
- 功能：添加基於結構的建構子：
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
- 功能：添加 `ScreenInteractive::TrackMouse(false)` 禁用滑鼠支援。

### Dom
- 功能：添加 `hyperlink` 裝飾器。例如：
  ```cpp
  auto link = text("Click here") | hyperlink("https://github.com/FTXUI")
  ```
  請參閱 [OSC 8 頁面](https://gist.github.com/egmontkob/eb114294efbcd5adb1944c9f3cb5feda)。
  FTXUI 支援由 @aaleino 在 [#662](https://github.com/ArthurSonzogni/FTXUI/issues/662) 中提出。

### 螢幕
- 破壞性變更：`WordBreakProperty` 變為 uint8_t 列舉。這帶來了 0.8% 的性能提升。
- 破壞性變更：移除使用者定義的 Pixel 建構子和相等運算子。
- 性能：基準測試快了 19%。


### 建置
- 使用 cmake find_package() 時檢查版本兼容性
- 添加 `FTXUI_DEV_WARNING` 選項以在建置 FTXUI 時開啟警告
- 預設關閉 `FTXUI_BUILD_DOCS`
- 預設關閉 `FTXUI_BUILD_EXAMPLE`

4.1.1
-----

### 組件
- 修正：在應用程式模式下支援箭頭鍵
- 修正：使用替代螢幕時移除無用的新行。

### Dom
- 功能：為邊框和分隔符添加虛線樣式：
  - 請參閱 `DASHED` 列舉，以及 `separatorDashed()`、`borderDashed()` 函數。
- 功能：添加彩色邊框。
  - 請參閱函數：`borderStyled(BorderStyle, Color)` 和 `borderStyled(Color)`。
- 功能：添加 `LinearGradient`。它可以用於 `color` 和 `bgColor`。
- 改進：Color::Interpolate() 使用 gamma 校正。
- 修正：檢查 `graph` 區域是否為正。

### 建置/安裝
- 如果已設定，則使用全域設定的 CMAKE_CXX_STANDARD。
- 暴露 pkg-config 文件
- 使用 cmake find_package() 時檢查版本兼容性

4.1.0 (已放棄)
-----
此版本已放棄，不應使用。它引入了 API 的破壞性變更。

4.0.0
-----

### DOM
- 功能：更多樣式：
  - `strikethrough`
  - `underlinedDouble`
- 功能：自定義游標。添加以下裝飾器：
  - `focusCursorBlock`
  - `focusCursorBlockBlinking`
  - `focusCursorBar`
  - `focusCursorBarBlinking`
  - `focusCursorUnderline`
  - `focusCursorUnderlineBlinking`
- 錯誤修正：修復當 `vbox`/`hbox`/`dbox` 包含 `flexbox` 時的 `focus`/`select`
- 錯誤修正：修復選定/焦點區域。它曾經比請求的尺寸大/長 1 個單元格
- 錯誤修正：在 gridbox 中轉發子項的選定/焦點區域。
- 錯誤修正：修復不正確的 Canvas 計算尺寸。
- 錯誤修正：支援內部尺寸為零的 `vscroll_indicator`。
- 錯誤修正：修復 `vscroll_indicator` 隱藏最後一列的問題。

### 組件：
- 功能：添加 `Modal` 組件。
- 功能：`Slider` 支援獲取其所有參數的引用。
- 功能：`Slider` 支援 `SliderOption`。它支援：
    - 多個方向。
    - 多種顏色。
    - 各種值（value, min, max, increment）。
- 功能：定義 `ScreenInteractive::Exit()`。
- 功能：添加 `Loop` 以讓開發人員更好地控制主循環。這可以用於將 FTXUI 整合到另一個主循環中，而無需完全控制。
- 功能：`Input` 支援 CTRL+Left 和 CTRL+Right
- 功能：在 `Input` 組件中使用閃爍的條。
- 改進：當使用滑鼠選擇條目時，`Menu` 會保持焦點。
- 錯誤修正：添加 `ButtonOption::Border()` 的實現。它之前缺失。
- 錯誤修正：為 F1-F4 和 F11 提供正確的鍵。
- 功能：添加 `Hoverable` 組件裝飾器。

### 螢幕
- 功能：添加 `Box::Union(a,b) -> Box`
- 錯誤修正：修復重置 `dim` 與重置 `bold` 衝突的問題。
- 功能：添加 emscripten 螢幕調整大小支援。
- 錯誤修正：添加對全寬字元的 unicode 13 支援。
- 錯誤修正：修復 MSVC 將 codecvt C++17 棄用函數視為錯誤。

### 建置
- 支援使用套件管理器提供的 google test 版本。

3.0.0
-----

### 建置
- **破壞性變更**：程式庫前綴現在恢復為 "lib"（默認）。這表示非 cmake 用戶不應該連結 "libftxui-dom" 等。

### 組件
- **動畫**模組！組件可以實現 `OnAnimation` 方法和 animation::Animator 來定義一些動畫屬性。
  - `Menu` 現在支援動畫。
  - `Button` 現在支援動畫。
- 支援 SIGTSTP。(ctrl+z)。
- 支援任務發布。`ScreenInteractive::Post(Task)`。
- `Menu` 現在可以使用 `MenuOption.direction` 在 4 個方向上使用。
- `Menu` 可以使用 `MenuOption.underline.enabled` 顯示動畫下劃線。
- `Button` 現在在框架中獲取焦點。
- **破壞性變更**所有選項現在都使用轉換函數。
- **破壞性變更** `Toggle` 組件現在使用 `Menu` 實現。
- **錯誤修正** Container::Tab 實現 `Focusable()`。
- **錯誤修正** 改進了 ComponentBase `Focusable()` 和 `ActiveChild()` 方法的默認實現。
- **錯誤修正** 自動將 '\r' 鍵轉換為 '\n'，用於不發送正確回車鍵代碼的 Linux 程式，如 'bind'。
  https://github.com/ArthurSonzogni/FTXUI/issues/337
- 為組件添加裝飾器：
  - `operator|(Component, ComponentDecorator)`
  - `operator|(Component, ElementDecorator)`
  - `operator|=(Component, ComponentDecorator)`
  - `operator|=(Component, ElementDecorator)`
  - 添加 `Maybe` 裝飾器。
  - 添加 `CatchEvent` 裝飾器。
  - 添加 `Renderer` 裝飾器。
- **破壞性變更** 移除 "deprectated.hpp" 標頭和 Input 對寬字串的支援。

### DOM：
- **破壞性變更**：`inverted` 裝飾器現在切換 `inverted` 屬性。
- 為 4 個方向添加 `gauge`。公開以下 API：
```cpp
Element gauge(float ratio);
Element gaugeLeft(float ratio);
Element gaugeRight(float ratio);
Element gaugeUp(float ratio);
Element gaugeDown(float ratio);
Element gaugeDirection(float ratio, GaugeDirection);
```
- 添加 `separatorHSelector` 和 `separatorVSelector` 元素。這可以用於突出顯示一個區域。
- 添加 `automerge` 裝飾器。這使得分隔符字元與附近的其他字元合併。
- 修復 `Table` 渲染函數，以允許自動合併字元。
- **錯誤修正**：`vscroll_indicator` 現在正確計算其偏移量和尺寸。
- 添加 `operator|=(Element, Decorator)`

### 螢幕：
- 添加：`Color::Interpolate(lambda, color_a, color_b)`。

2.0.0
-----

### 功能：

#### 螢幕
- 將 `automerge` 添加到 Pixel 位元欄位。這現在控制哪些像素會自動合併。

#### DOM：
- 添加 `Canvas` 類別和 `ElementFrom('canvas')` 函數。庫的使用者可以一起使用盲文和塊字元進行繪圖。
- 支援 `flexbox` dom 元素。這與 HTML 的建置是對稱的。支援所有以下屬性：direction, wrap, justify-content, align-items, align-content, gap
- 添加基於 `flexbox` 的 dom 元素助手：
  - `paragraph`
  - `paragraphAlignLeft`
  - `paragraphAlignCenter`
  - `paragraphAlignRight`
  - `paragraphAlignJustify`
- 添加基於 `flexbox` 的助手元素：`hflow()`, `vflow()`。
- 添加：`focusPositionRelative` 和 `focusPosition`
- 添加 `Table` 建構子，來自 Element 的 2D 向量，而不是字串。

#### 組件
- 添加 `collapsible` 組件。
- 添加 `ScreenInteractive::WithRestoredIO`。這會裝飾一個回調。它會在臨時卸載終端鉤子後運行它。如果您想直接使用 stdin/stdout/sterr 執行命令，這會很有用。

### 錯誤

#### 表格
- `table` 的水平和垂直分隔符現在已正確展開。

#### 組件
- `Input` 在滑鼠懸停時不應獲取焦點。
- 在 on_enter/on_change 事件期間修改 `Input` 現在可以正常工作。

### 破壞性變更：
- `paragraph` 的行為已修改。它現在返回一個 Element，而不是 Element 列表。

0.11.1
------

0.11.1
------

# 組件
- 功能：支援 PageUp/PageDown/Home/End 按鈕。
- 錯誤修正：檢查所選元素是否在下拉選單的範圍內。

# 建置
- 錯誤修正：使用 "Release config" 打包庫。不是調試。

0.11
----

## github 工作流程
- 添加 Windows 和 MacOS 工件。
- 合併所有工作流程。

## 錯誤
- 在 Unix 系統上，失敗時回退到 {80,25} 螢幕尺寸。

## CMake
- 透過 `BUILD_SHARED_LIBS` 選項支援共用庫。
- 添加庫版本和符號連結。

0.10 (2021-09-30)
--------------------

## 錯誤
- 修復邊框的自動合併。

### Dom
- `Table()` 類別用於建置樣式化表格。
   請參閱 https://github.com/ArthurSonzogni/FTXUI/discussions/228
- `vscroll_indicator`。在右側顯示一個滾動條指示器。
- `separatorEmpty`。一個不繪製任何內容的分隔符。
- `separatorFixed`。一個繪製指定字元的分隔符。

### 組件
- `Maybe`：根據布林值有條件地顯示組件。
- `Dropdown`：一個下拉選擇列表。

0.9 (2021-09-26)
----------------

首次發布更新日誌。

此版本包括：

### 螢幕
- 樣式：
  - 粗體。
  - 閃爍。
  - 暗淡。
  - 反轉。
  - 底線。
  - 前景色。
  - 背景色。
- 支援 UTF8 unicode。
  - 全寬字元：测试。
  - 組合字元：a⃒
- 模板緩衝區。
- 自動合併框繪製字元。
- 檢測終端尺寸。

### DOM

- 元素：
  - `text` & `vtext`
  - `separator` 和 5 種變體。
  - `gauge`
  - `border` 和 6 種變體。
  - `window`
  - `spinner`
  - `paragraph` 和 `hflow`。

- 佈局：
  - `hbox`
  - `vbox`
  - `dbox`
  - `gridbox`
  - `frame`：在虛擬區域內繪製，可能大於實際區域。
  - `focus`，`select`：滾動框架的內部視圖，使其可見。
  - `flex` 和 8 種變體。`filler`
  
- 裝飾器：
  - `bold`
  - `dim`
  - `inverted`
  - `blink`
  - `color`
  - `bgcolor`
  - `clearunder`

### 組件

- 容器：
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
- `Renderer` 及變體
- `CatchEvent`

### 其他

- Fuzzer
- 使用 gtest 進行測試。
- Doxygen 文件
- IWYU
- 52 個範例。
- 支援 WebAssembly。
- 支援 Window 和故障終端的 fallback。
