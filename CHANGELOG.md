变更日志
=========

下一个
====

### 文档
- 修复损坏的 Doxygen 输出。参见 #1029 中的 @markmandel。
- 使用 Doxygen awesome。添加我们自己的主题。
- 将文档分成几个页面。

### 构建
- 功能：支持 C++20 模块。
  这需要：
  - 使用 Ninja 或 MSVC 生成器
  - 最新的 Clang/GCC/MSVC 编译器。
  - Cmake 3.28 或更高版本。
  用法：
  ```cpp
  import ftxui;
  import ftxui.component;
  import ftxui.dom;
  import ftxui.screen;
  import ftxui.util;
  ```
  感谢 @mikomikotaishi 的 PR #1015。
- 移除对 'pthread' 的依赖。

### 组件
- 功能：POSIX 管道输入处理。
  - 允许 FTXUI 应用程序从 stdin（通过管道连接时）读取数据，同时仍从终端接收键盘输入。
  - 默认启用。
  - 可以使用 `ScreenInteractive::HandlePipedInput(false)` 禁用。
  - 仅适用于 Linux 和 macOS。
  感谢 @HarryPehkonen 的 PR #1094。
- 修复 `ScreenInteractive::FixedSize` 屏幕覆盖前面的终端
  输出。感谢 #1064 中的 @zozowell。
- 修复垂直 `ftxui::Slider`。以前“向上”键会减小
  值。感谢 #1093 中的 @its-pablo 报告此问题。

### DOM
- 修复 `ComputeShrinkHard` 中的整数溢出。感谢 #1137 中的 @its-pablo 报告并修复此问题。
- 为 `vbox/hbox/dbox` 添加特化，以允许 Element 容器作为
  输入。感谢 #1117 中的 @nbusser。

6.1.9 (2025-05-07)
------------

### 构建
如果一切顺利（待定），ftxui 应该会出现在 Bazel 中央仓库中。
可以使用以下行将其导入到您的项目中：

**MODULE.bazel**
```bazel
bazel_dep(name = "ftxui", version = "6.1.9")
```

感谢 @robinlinden 和 @kcc 的审阅。

### dom
- Bugfix：恢复 ftxui 5.0.0 中的 `dbox` 行为。为了在两层之间应用背景颜色混合，将添加一个新的 `dboxBlend`。

6.1.8 (2025-05-01)
------------------

### 构建
- 功能：支持 `bazel` 构建系统。参见 #1032。
  由 Kostya Serebryany @kcc 提出

  **BUILD.bazel**
  ```bazel
  deps = [
    // 依赖整个库：
    "@ftxui//:ftxui",

    // 选择一个特定的子模块：
    "@ftxui//:component",
    "@ftxui//:dom",
    "@ftxui//:screen",
  ]
  ```

### 组件
- Bugfix：修复 `ResizeableSplit` 崩溃问题。参见 #1023。
  - 将屏幕尺寸限制为终端尺寸。
  - 禁止 `ResizeableSplit` 具有负尺寸。

### Dom
- Bugfix：禁止指定负尺寸约束。参见 #1023。


6.0.2 (2025-03-30)
-----

### 组件
- Bugfix：修复 Windows 上影响所有组件的重大崩溃。参见 #1020
- Bugfix：修复 focusRelative。

6.0.1 (2025-03-28)
-----

与 v6.0.0 相同。

由于问题，标签 v6.0.0 被替换。这不是一个好的做法，并影响了在短时间内开始使用它的开发人员。提交具有相同内容的新版本是解决此问题的最佳方法。

参见 #1017 和 #1019。

6.0.0 (2025-03-23)
-----

### 组件
- 功能：添加对原始输入的支持。允许检测更多按键。
- 功能：添加 `ScreenInteractive::ForceHandleCtrlC(false)` 以允许组件
  完全覆盖默认的 `Ctrl+C` 处理程序。
- 功能：添加 `ScreenInteractive::ForceHandleCtrlZ(false)` 以允许组件
  完全覆盖默认的 `Ctrl+Z` 处理程序。
- 功能：在支持的终端上添加 `Mouse::WeelLeft` 和 `Mouse::WeelRight` 事件。
- 功能：添加 `Event::DebugString()`。
- 功能：添加对 `Input` 的插入模式支持。添加 `InputOption::insert`
  选项。由 @mingsheng13 添加。
- 功能：添加 `DropdownOption` 以配置下拉菜单。参见 #826。
- 功能：添加对选择的支持。感谢 @clement-roblot。参见 #926。
  - 参见 `ScreenInteractive::GetSelection()`。
  - 参见 `ScreenInteractive::SelectionChange(...)` 侦听器。
- Bugfix/破坏性更改：`鼠标过渡`：
  - 检测鼠标移动，而不是按下。
    添加了 `Mouse::Moved` 动作。
  - 按下鼠标左键拖动现在可以避免激活多个复选框。
  - 几个组件现在在鼠标按下时激活，而不是在释放时激活。
  这修复了：https://github.com/ArthurSonzogni/FTXUI/issues/773
  这修复了：https://github.com/ArthurSonzogni/FTXUI/issues/792
- Bugfix：鼠标控制现在报告正确。
- 功能：添加 `ScreenInteractive::FullscreenPrimaryScreen()`。这允许
  在主屏幕上显示全屏组件，而不是在备用屏幕上显示。
- Bugfix：在退格键或删除键时未调用 `Input` `onchange`。
  由 @chrysante 在 PR #776 中修复。
- Bugfix：退出时正确恢复光标形状。参见 #792。
- Bugfix：修复光标在最后一列时的位置。参见 #831。
- Bugfix：修复 `ResizeableSplit` 键盘导航。由 #842 修复。
- Bugfix：修复 `Menu` 焦点。参见 #841
- 功能：添加 `ComponentBase::Index()`。这允许获取组件在其父级中的索引。参见 #932
- 功能：添加 `EntryState::index`。这允许获取菜单项的索引。参见 #932
- 功能：添加 `SliderOption::on_change`。这允许在滑块值更改时设置回调。参见 #938。
- Bugfix：处理没有条目的 `Dropdown`。
- Bugfix：修复 `LinearGradient` 中由于浮点精度和差一错误导致的崩溃。参见 #998。

### DOM
- 功能：添加 `italic` 装饰器。例如：
  ```cpp
  auto italic_text = text("Italic text") | italic;
  ```
  ```cpp
  auto italic_text = italic(text("Italic text"));
  ```
  由 @kenReneris 在 #1009 中提出。
- 功能：添加 `hscroll_indicator`。它显示一个反映当前滚动位置的水平指示器。由 @ibrahimnasson 在
  [issue 752](https://github.com/ArthurSonzogni/FTXUI/issues/752) 中提出。
- 功能：向 `Dimension::Fit(..)` 添加 `extend_beyond_screen` 选项，允许
  元素大于屏幕。由 @LordWhiro 提出。参见 #572 和 #949。
- 功能：添加对选择的支持。感谢 @clement-roblot。参见 #926。
  - 参见 `selectionColor` 装饰器。
  - 参见 `selectionBackgroundColor` 装饰器。
  - 参见 `selectionForegroundColor` 装饰器。
  - 参见 `selectionStyle(style)` 装饰器。
  - 参见 `selectionStyleReset` 装饰器。
- 破坏性更改：更改“焦点”/“选择”的处理方式。这修复了行为。
- 破坏性更改：`Component::OnRender()` 成为覆盖以渲染组件的方法。这取代了仍然用于在其子项上调用渲染方法的 `Component::Render()`。此更改允许修复一些焦点处理问题。

### 屏幕
- 功能：添加 `Box::IsEmpty()`。
- 功能：颜色透明度
    - 添加 `Color::RGBA(r,g,b,a)`。
    - 添加 `Color::HSVA(r,g,b,a)`。
    - 添加 `Color::Blend(Color)`。
    - 添加 `Color::IsOpaque()`
- 功能：支持 `ConstStringListRef` 的任意 `Adapter`。参见 #843。

### 构建
- 支持 cmake 的“unity/jumbo”构建。由 @ClausKlein 修复。

5.0.0
-----

### 组件
- 破坏性：MenuDirection 枚举重命名为 Direction
- 破坏性：GaugeDirection 枚举重命名为 Direction
- 破坏性：Direction 枚举重命名为 WidthOrHeight
- 破坏性：移除 `ComponentBase` 复制构造函数/赋值。
- 破坏性：MenuOption::entries 重命名为 MenuOption::entries_option。
- 破坏性：`Ref<{Component}Option>` 在组件构造函数中变为 `{Component}Option`。
- 功能：`ResizeableSplit` 现在支持任意元素作为分隔符。
- 功能：`input` 现在支持多行。
- 功能：`input` 样式现在可以自定义。
- Bugfix：支持来自 OS 终端的 F1-F5。
- 功能：添加基于结构体的构造函数：
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
- 功能：添加 `ScreenInteractive::TrackMouse(false)` 禁用鼠标支持。

### DOM
- 功能：添加 `hyperlink` 装饰器。例如：
  ```cpp
  auto link = text("Click here") | hyperlink("https://github.com/FTXUI")
  ```
  参见 [OSC 8 页面](https://gist.github.com/egmontkob/eb114294efbcd5adb1944c9f3cb5feda)。
  FTXUI 支持由 @aaleino 在 [#662](https://github.com/ArthurSonzogni/FTXUI/issues/662) 中提出。

### 屏幕
- 破坏性：`WordBreakProperty` 变为 uint8_t 枚举。这带来了 0.8% 的性能提升。
- 破坏性：移除用户定义的 Pixel 构造函数和相等运算符。
- 性能：基准测试中快 19%。


### 构建
- 使用 cmake find_package() 时检查版本兼容性
- 添加 `FTXUI_DEV_WARNING` 选项以在构建 FTXUI 时打开警告
- 默认关闭 `FTXUI_BUILD_DOCS`
- 默认关闭 `FTXUI_BUILD_EXAMPLE`

4.1.1
-----

### 组件
- 修复：支持应用程序模式下的箭头键
- 修复：使用备用屏幕时移除无用的新行。

### DOM
- 功能：为边框和分隔符添加虚线样式：
  - 参见 `DASHED` 枚举，以及 `separatorDashed()`、`borderDashed()` 函数。
- 功能：添加彩色边框。
  - 参见函数：`borderStyled(BorderStyle, Color)` 和 `borderStyled(Color)`。
- 功能：添加 `LinearGradient`。它可用于 `color` 和 `bgColor`。
- 改进：`Color::Interpolate()` 使用伽马校正。
- 修复：检查 `graph` 区域是否为正。

### 构建/安装
- 如果设置了，则使用全局设置的 CMAKE_CXX_STANDARD。
- 暴露 pkg-config 文件
- 使用 cmake find_package() 时检查版本兼容性

4.1.0 （已放弃）
-----
此版本已放弃，不得使用。它引入了 API 中的破坏性更改。

4.0.0
-----

### DOM
- 功能：更多样式：
  - `strikethrough`
  - `underlinedDouble`
- 功能：自定义光标。添加以下装饰器：
  - `focusCursorBlock`
  - `focusCursorBlockBlinking`
  - `focusCursorBar`
  - `focusCursorBarBlinking`
  - `focusCursorUnderline`
  - `focusCursorUnderlineBlinking`
- Bugfix：修复 `vbox`/`hbox`/`dbox` 包含 `flexbox` 时的 `focus`/`select`
- Bugfix：修复选择/聚焦区域。它以前比请求的大/长 1 个单元格
- Bugfix：在 gridbox 中转发子级的选择/聚焦区域。
- Bugfix：修复不正确的 Canvas 计算尺寸。
- Bugfix：支持内部尺寸为零的 `vscroll_indicator`。
- Bugfix：修复 `vscroll_indicator` 隐藏最后一列。

### 组件：
- 功能：添加 `Modal` 组件。
- 功能：`Slider` 支持为其所有参数提供引用。
- 功能：`Slider` 支持 `SliderOption`。它支持：
    - 多个方向。
    - 多个颜色。
    - 各种值（value、min、max、increment）。
- 功能：定义 `ScreenInteractive::Exit()`。
- 功能：添加 `Loop` 以使开发人员更好地控制主循环。这
  可用于将 FTXUI 集成到另一个主循环中，而无需完全
  控制。
- 功能：`Input` 支持 CTRL+Left 和 CTRL+Right
- 功能：在 `Input` 组件中使用闪烁条。
- 改进：当使用鼠标选择条目时，`Menu` 保持焦点。
- Bugfix：添加 `ButtonOption::Border()` 的实现。它缺失了。
- Bugfix：为 F1-F4 和 F11 提供正确的按键。
- 功能：添加 `Hoverable` 组件装饰器。

### 屏幕
- 功能：添加 `Box::Union(a,b) -> Box`
- Bugfix：修复 `dim` 重置与 `bold` 重置冲突。
- 功能：添加 emscripten 屏幕调整大小支持。
- Bugfix：为全角字符添加 unicode 13 支持。
- Bugfix：修复 MSVC 将 codecvt C++17 弃用函数视为错误。

### 构建
- 支持使用包管理器提供的 google test 版本。

3.0.0
-----

### 构建
- **破坏性更改**：库前缀现在恢复为“lib”（默认）。这
    意味着非 cmake 用户不应链接“libftxui-dom”等。

### 组件
- **动画**模块！组件可以实现 `OnAnimation` 方法和
  `animation::Animator` 来定义一些动画属性。
  - `Menu` 现在支持动画。
  - `Button` 现在支持动画。
- 支持 SIGTSTP。（Ctrl+Z）。
- 支持任务发布。`ScreenInteractive::Post(Task)`。
- `Menu` 现在可以使用 4 个方向，使用 `MenuOption.direction`。
- `Menu` 可以显示动画下划线，使用
  `MenuOption.underline.enabled`。
- `Button` 现在在帧中获取焦点。
- **破坏性更改** 所有选项现在都使用转换函数。
- **破坏性更改** `Toggle` 组件现在使用 `Menu` 实现。
- **bugfix** `Container::Tab` 实现了 `Focusable()`。
- **bugfix** 改进了 ComponentBase `Focusable()` 和
  `ActiveChild()` 方法的默认实现。
- **bugfix** 自动将 '' 键转换为 '
'，适用于不发送
  正确回车键代码的 Linux 程序，例如 'bind'。
  https://github.com/ArthurSonzogni/FTXUI/issues/337
- 为组件添加装饰器：
  - `operator|(Component, ComponentDecorator)`
  - `operator|(Component, ElementDecorator)`
  - `operator|=(Component, ComponentDecorator)`
  - `operator|=(Component, ElementDecorator)`
  - 添加 `Maybe` 装饰器。
  - 添加 `CatchEvent` 装饰器。
  - 添加 `Renderer` 装饰器。
- **破坏性更改** 移除 "deprectated.hpp" 头文件和对宽
    字符串的 Input 支持。

### DOM：
- **破坏性更改**：`inverted` 装饰器现在在 inverted 属性中切换。
- 添加 4 个方向的 `gauge`。暴露以下 API：
```cpp
Element gauge(float ratio);
Element gaugeLeft(float ratio);
Element gaugeRight(float ratio);
Element gaugeUp(float ratio);
Element gaugeDown(float ratio);
Element gaugeDirection(float ratio, GaugeDirection);
```
- 添加 `separatorHSelector` 和 `separatorVSelector` 元素。这可用于
  突出显示区域。
- 添加 `automerge` 装饰器。这使得分隔符字符与附近的其他字符合并。
- 修复 `Table` 渲染函数，以允许自动合并字符。
- **Bugfix**：`vscroll_indicator` 现在正确计算其偏移量和大小。
- 添加 `operator|=(Element, Decorator)`

### 屏幕：
- 添加：`Color::Interpolate(lambda, color_a, color_b)`。

2.0.0
-----

### 功能：

#### 屏幕
- 将 `automerge` 添加到 Pixel 位字段。这现在控制哪些像素
  自动合并。

#### DOM：
- 添加 `Canvas` 类和 `ElementFrom('canvas')` 函数。库的用户可以
  使用盲文和块字符进行绘图。
- 支持 `flexbox` dom 元素。这与 HTML 的构建是对称的。
  支持所有以下属性：direction、wrap、justify-content、
  align-items、align-content、gap
- 添加基于 `flexbox` 的 dom 元素助手：
  - `paragraph`
  - `paragraphAlignLeft`
  - `paragraphAlignCenter`
  - `paragraphAlignRight`
  - `paragraphAlignJustify`
- 添加基于 `flexbox` 的助手元素：`hflow()`、`vflow()`。
- 添加：`focusPositionRelative` 和 `focusPosition`
- 从 Element 的 2D 向量而不是字符串添加 `Table` 构造函数。

#### 组件
- 添加 `collapsible` 组件。
- 添加 `ScreenInteractive::WithRestoredIO`。这装饰了一个回调。这
  在终端钩子暂时卸载的情况下运行它。这在您想直接
  使用 stdin/stdout/sterr 执行命令时很有用。

### 错误

#### 表格
- `table` 的水平和垂直分隔符现在已正确展开。

#### 组件
- `Input` 在鼠标悬停时不应获取焦点。
- 在 on_enter/on_change 事件期间修改 `Input` 现在可以正常工作。

### 破坏性更改：
- `paragraph` 的行为已修改。它现在返回一个 Element，
  而不是一个 Element 列表。

0.11.1
------

# 组件
- 功能：支持 PageUp/PageDown/Home/End 按钮。
- Bugfix：检查所选元素是否在 Dropdown 的边界内。

# 构建
- Bugfix：使用“发布配置”打包库。而不是调试。

0.11
----

## github 工作流程
- 添加 Windows 和 MacOS 工件。
- 合并所有工作流程。

## 错误
- 在 Unix 系统上，失败时回退到 {80,25} 屏幕尺寸。

## CMake
- 通过 `BUILD_SHARED_LIBS` 选项支持共享库。
- 添加库版本和符号链接。

0.10 (2021-09-30)
--------------------

## 错误
- 修复边框的自动合并。

### DOM
- `Table()` 类，用于构建带样式的表格。
   参见 https://github.com/ArthurSonzogni/FTXUI/discussions/228
- `vscroll_indicator`。在右侧显示滚动条指示器。
- `separatorEmpty`。一个不绘制任何内容的分隔符。
- `separatorFixed`。一个绘制指定字符的分隔符。

### 组件
- `Maybe`：根据布尔值有条件地显示组件。
- `Dropdown`：一个下拉选择列表。

0.9 (2021-09-26)
----------------

最初发布，其中包含变更日志。

此版本包括：

### 屏幕
- 样式：
  - 粗体。
  - 闪烁。
  - 暗淡。
  - 反转。
  - 下划线。
  - 前景颜色。
  - 背景颜色。
- 支持 UTF8 Unicode。
  - 全角字符：测试。
  - 组合字符：a⃒
- 模板缓冲区。
- 自动合并框绘字符。
- 检测终端尺寸。

### DOM

- 元素：
  - `text` 和 `vtext`
  - `separator` 和 5 种变体。
  - `gauge`
  - `border` 和 6 种变体。
  - `window`
  - `spinner`
  - `paragraph` 和 `hflow`。

- 布局：
  - `hbox`
  - `vbox`
  - `dbox`
  - `gridbox`
  - `frame`：在虚拟区域内绘图，可能比真实区域大。
  - `focus`，`select`：滚动帧的内部视图，使其可见。
  - `flex` 和 8 种变体。`filler`

- 装饰器：
  - `bold`
  - `dim`
  - `inverted`
  - `blink`
  - `color`
  - `bgcolor`
  - `clearunder`

### 组件

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
- `Renderer` 和变体
- `CatchEvent`

### 杂项

- 模糊测试器
- 使用 gtest 进行测试。
- Doxygen 文档
- IWYU
- 52 个示例。
- 支持 WebAssembly。
- 支持 Window 和损坏终端的备用方案。