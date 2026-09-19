@page module-component ftxui / component
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595349.png)

`ftxui::component` 模組定義了生成互動元件的邏輯，這些元件響應使用者事件（鍵盤、滑鼠等）。

@subpage module-component-examples 章節提供了一系列範例。

A `ftxui::App` defines a main loop that renders a component.

`ftxui::Component` 是 `ftxui::ComponentBase` 的共享指針。後者定義了：
  - `ftxui::ComponentBase::Render()`: 如何渲染介面。
  - `ftxui::ComponentBase::OnEvent()`: 如何響應事件。
  - `ftxui::ComponentBase::Add()`: 在兩個元件之間建立父/子關係。元件樹用於定義如何使用鍵盤導航。

`ftxui::Element` 用於渲染單個畫面。

`ftxui::Component` 用於渲染動態使用者介面，生成多個畫面，並在事件發生時更新其狀態。

[畫廊](https://arthursonzogni.github.io/FTXUI/examples_2component_2gallery_8cpp-example.html) 多個元件的集合。 ([demo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/gallery))

![image](https://user-images.githubusercontent.com/4759106/147247330-b60beb9f-e665-48b4-81c0-4b01ee95bc66.png)

所有預定義的元件都可以在
["ftxui/dom/component.hpp"](./component_8hpp.html) 中找到。

\include ftxui/component/component.hpp

# Input {#component-input}

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2input_8cpp-example.html)：

![image](https://user-images.githubusercontent.com/4759106/147247671-f1d6f606-1845-4e94-a4a0-d4273e9ae6bd.png)

由 `ftxui::Input()` 從 "ftxui/component/component.hpp" 生成。

@htmlonly
<script id="asciicast-223719" src="https://asciinema.org/a/223719.js" async></script>
@endhtmlonly

## Filtered input

可以使用 `ftxui::CatchEvent` 過濾輸入元件接收到的字元。

```cpp
std::string phone_number;
Component input = Input(&phone_number, "phone number");

// Filter out non-digit characters.
input |= CatchEvent([&](Event event) {
  return event.is_character() && !std::isdigit(event.character()[0]);
});

// Filter out characters past the 10th one.
input |= CatchEvent([&](Event event) {
  return event.is_character() && phone_number.size() >= 10;
});
```

# Menu {#component-menu}

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2menu_8cpp-example.html)：

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2dropdown_8cpp-example.html)：

![youtube-video-gif (3)](https://user-images.githubusercontent.com/4759106/147246982-1e821751-531c-4e1f-bc37-2fa290e143cd.gif)


由 `ftxui::Dropdown()` 從 "ftxui/component/component.hpp" 生成。

# Slider {#component-slider}

表示一個滑塊物件，它由一個帶有分箱中間間隔的範圍組成。它可以使用 `ftxui::Slider()` 創建。

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2slider_8cpp-example.html)：

![image](https://user-images.githubusercontent.com/4759106/147249265-7e2cad75-082c-436e-affe-44a550c480ab.png)

由 `ftxui::Slider()` 從 "ftxui/component/component.hpp" 生成。

# Renderer {#component-renderer}

由 `ftxui::Renderer()` 從 \ref ftxui/component/component.hpp 生成。此元件通過使用不同的函數來渲染介面，從而裝飾另一個元件。

# CheckBox {#component-checkbox}

This component defines a checkbox. It is a single entry that can be turned
on/off.

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2checkbox_8cpp-example.html)：

![image](https://user-images.githubusercontent.com/4759106/147246646-b86926a9-1ef9-4efb-af98-48a9b62acd81.png)

Produced by: `ftxui::Checkbox()` from "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223724" src="https://asciinema.org/a/223724.js" async></script>
@endhtmlonly

# RadioBox {#component-radiobox}

A radiobutton component. This is a list of entries, where one can be turned on.

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2radiobox_8cpp-example.html)：
  
![image](https://user-images.githubusercontent.com/4759106/147246401-809d14a5-6621-4e36-8dd9-a2d75ef2a94e.png)

Produced by: `ftxui::Radiobox()` from "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223725" src="https://asciinema.org/a/223725.js" async></script>
@endhtmlonly

# Dropdown {#component-dropdown}

A drop-down menu is a component that, when opened, displays a list of elements
for the user to select from.

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2dropdown_8cpp-example.html)：

![youtube-video-gif (3)](https://user-images.githubusercontent.com/4759106/147246982-1e821751-531c-4e1f-bc37-2fa290e143cd.gif)

Produced by: `ftxui::Dropdown()` from "ftxui/component/component.hpp"

# Slider {#component-slider}

Represents a slider object that consists of a range with binned intermediate
intervals. It can be created by `ftxui::Slider()`.

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2slider_8cpp-example.html)：

![image](https://user-images.githubusercontent.com/4759106/147249265-7e2cad75-082c-436e-affe-44a550c480ab.png)

Produced by: `ftxui::Slider()` from "ftxui/component/component.hpp"

# Renderer {#component-renderer}

Produced by: `ftxui::Renderer()` from \ref ftxui/component/component.hpp. This
component decorate another one by using a different function to render an
interface.

Example:
```cpp
auto inner = [...]

auto renderer = Renderer(inner, [&] {
  return inner->Render() | border
});
```

`ftxui::Renderer` 也支援元件裝飾器模式：
```cpp
auto component = [...]
component = component
  | Renderer([](Element e) { return e | border))
  | Renderer(bold)
```

As a short hand, you can also compose a component with an element decorator:
```cpp
auto component = [...]
component = component | border | bold;
```

# CatchEvent {#component-catchevent}

由 `ftxui::CatchEvent()` 從 \ref ftxui/component/component.hpp 生成。此元件裝飾其他元件，在底層元件之前捕獲事件。

Examples:
```cpp
auto screen = App::TerminalOutput();
auto renderer = Renderer([] {
  return text("My interface");
});
auto component = CatchEvent(renderer, [&](Event event) {
  if (event == Event::Character('q')) {
    screen.ExitLoopClosure()();
    return true;
  }
  return false;
});
screen.Loop(component);
```

`ftxui::CatchEvent` 也可以用作裝飾器：
```cpp
component = component
  | CatchEvent(handler_1)
  | CatchEvent(handler_2)
  | CatchEvent(handler_3)
  ;
```

# Collapsible {#component-collapsible}

對於使用者可以開啟或關閉其可見性的視覺元素很有用。本質上，這是 `ftxui::Checkbox()` 和 `ftxui::Maybe()` 元件的組合。

```cpp
auto collapsible = Collapsible("Show more", inner_element);
```

# Maybe {#component-maybe}

由 `ftxui::Maybe()` 從 \ref ftxui/component/component.hpp 生成。
此元件可用於通過布林值或謂詞顯示/隱藏任何其他元件。

Example with a boolean:
```cpp
bool show = true;
auto component = Renderer([]{ return "Hello World!"; });
auto maybe_component = Maybe(component, &show)
```

Example with a predicate:
```cpp
auto component = Renderer([]{ return "Hello World!"; });
auto maybe_component = Maybe(component, [&] { return time > 10; })
```

像往常一樣，`ftxui::Maybe` 也可以用作裝飾器：
```cpp
component = component
  | Maybe(&a_boolean)
  | Maybe([&] { return time > 10; })
  ;
```

# Container {#component-container}

## Horizontal {#component-horizontal}

由 `ftxui::Container::Horizontal()` 從
"ftxui/component/component.hpp" 生成。它水平顯示元件列表並處理鍵盤/滑鼠導航。

## Vertical {#component-vertical}

由 `ftxui::Container::Vertical()` 從
"ftxui/component/component.hpp" 生成。它垂直顯示元件列表並處理鍵盤/滑鼠導航。

## Tab {#component-tab}

由 `ftxui::Container::Tab()` 從
"ftxui/component/component.hpp" 生成。它接受元件列表並僅顯示其中一個。這對於實現分頁欄很有用。

[垂直](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_vertical_8cpp-example.html)：
  
![ezgif com-gif-maker (1)](https://user-images.githubusercontent.com/4759106/147250144-22ff044a-4773-4ff7-a49c-12ba4034acb4.gif)

[水平](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_horizontal_8cpp-example.html)：
  
  ![ezgif com-gif-maker (2)](https://user-images.githubusercontent.com/4759106/147250217-fe447e0f-7a99-4e08-948a-995087d9b40e.gif)


# ResizableSplit  {#component-resizable-split}

它定義了兩個子元件之間的水平或垂直分隔。分隔的位置是可變的，並且可以使用滑鼠控制。
有四種可能的分隔：
- `ftxui::ResizableSplitLeft()`
- `ftxui::ResizableSplitRight()`
- `ftxui::ResizableSplitTop()`
- `ftxui::ResizableSplitBottom()`
從 "ftxui/component/component.hpp"

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2resizable_split_8cpp-example.html)：

![ezgif com-gif-maker](https://user-images.githubusercontent.com/4759106/147248372-c55512fe-9b96-4b08-a1df-d05cf2cae431.gif)  

@htmlonly
<script id="asciicast-tprMH2EdkUoMb7D2YxgMGgpzx" src="https://asciinema.org/a/tprMH2EdkUoMb7D2YxgMGgpzx.js" async></script>
@endhtmlonly

# Force a frame redraw. {#component-force-redraw}

Typically, `ftxui::App::Loop()` is responsible for drawing a new
frame whenever a new group of events (e.g keyboard, mouse, window resize, etc.)
has been processed. However, you might want to react to arbitrary events that
are unknown to FTXUI. To accomplish this, you must post events using
`ftxui::App::PostEvent` (**this is thread safe**) via a thread.
You will have to post the event `ftxui::Event::Custom`.

Example:
```cpp
screen->PostEvent(Event::Custom);
```

If you don't need to process a new Event, you can use:
```cpp
screen->RequestAnimationFrame();
```
instead.
