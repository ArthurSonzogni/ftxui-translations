@page module-component ftxui / component
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595349.png)

`ftxui::component` 模組定義了生成互動元件的邏輯，這些元件響應使用者事件（鍵盤、滑鼠等）。

@subpage module-component-examples 章節提供了一系列範例。

`ftxui::ScreenInteractive` 定義了一個主循環，用於渲染元件。

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

定義一個選單物件。它包含一個條目列表，其中一個被選中。

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2menu_8cpp-example.html)：

![image](https://user-images.githubusercontent.com/4759106/147247822-0035fd6f-bb13-4b3a-b057-77eb9291582f.png)


由 `ftxui::Menu()` 從 "ftxui/component/component.hpp" 生成。

@htmlonly
<script id="asciicast-223720" src="https://asciinema.org/a/223720.js" async></script>
@endhtmlonly

# Toggle {#component-toggle}

一種特殊的選單。條目水平顯示。

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2toggle_8cpp-example.html)：

![image](https://user-images.githubusercontent.com/4759106/147249383-e2201cf1-b7b8-4a5a-916f-d761e3e7ae40.png)

由 `ftxui::Toggle()` 從 "ftxui/component/component.hpp" 生成。

@htmlonly
<script id="asciicast-223722" src="https://asciinema.org/a/223722.js" async></script>
@endhtmlonly

# CheckBox {#component-checkbox}

此元件定義了一個核取方塊。它是一個單一條目，可以開啟/關閉。

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2checkbox_8cpp-example.html)：

![image](https://user-images.githubusercontent.com/4759106/147246646-b86926a9-1ef9-4efb-af98-48a9b62acd81.png)

由 `ftxui::Checkbox()` 從 "ftxui/component/component.hpp" 生成。

@htmlonly
<script id="asciicast-223724" src="https://asciinema.org/a/223724.js" async></script>
@endhtmlonly

# RadioBox {#component-radiobox}

一個單選按鈕元件。這是一個條目列表，其中一個可以開啟。

[範例](https://arthursonzogni.github.io/FTXUI/examples_2component_2radiobox_8cpp-example.html)：
  
![image](https://user-images.githubusercontent.com/4759106/147246401-809d14a5-6621-4e36-8dd9-a2d75ef2a94e.png)

由 `ftxui::Radiobox()` 從 "ftxui/component/component.hpp" 生成。

@htmlonly
<script id="asciicast-223725" src="https://asciinema.org/a/223725.js" async></script>
@endhtmlonly

# Dropdown {#component-dropdown}

下拉式選單是一個元件，當開啟時，會顯示一個元素列表供使用者選擇。

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

範例：
```cpp
auto inner = [...] 

auto renderer = Renderer(inner, [&] {
  return inner->Render() | border
});
```

`ftxui::Renderer` 也支援元件裝飾器模式：
```cpp
auto component = [...] 
component = component
  | Renderer([](Element e) { return e | border))
  | Renderer(bold)
```

作為簡寫，您還可以將元件與元素裝飾器組合：
```cpp
auto component = [...] 
component = component | border | bold;
```

# CatchEvent {#component-catchevent}

由 `ftxui::CatchEvent()` 從 \ref ftxui/component/component.hpp 生成。此元件裝飾其他元件，在底層元件之前捕獲事件。

範例：
```cpp
auto screen = ScreenInteractive::TerminalOutput();
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

布林值範例：
```cpp
bool show = true;
auto component = Renderer([]{ return "Hello World!"; });
auto maybe_component = Maybe(component, &show)
```

謂詞範例：
```cpp
auto component = Renderer([]{ return "Hello World!"; });
auto maybe_component = Maybe(component, [&] { return time > 10; })
```

像往常一樣，`ftxui::Maybe` 也可以用作裝飾器：
```cpp
component = component
  | Maybe(&a_boolean)
  | Maybe([&] { return time > 10; })
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

通常，`ftxui::ScreenInteractive::Loop()` 負責在處理完新的事件組（例如鍵盤、滑鼠、視窗大小調整等）時繪製新畫面。但是，您可能希望響應 FTXUI 未知的任意事件。為此，您必須通過執行緒使用 `ftxui::ScreenInteractive::PostEvent`（**這是執行緒安全的**）發布事件。您必須發布事件 `ftxui::Event::Custom`。

範例：
```cpp
screen->PostEvent(Event::Custom);
```

如果您不需要處理新事件，可以使用：
```cpp
screen->RequestAnimationFrame();
```
代替。

```