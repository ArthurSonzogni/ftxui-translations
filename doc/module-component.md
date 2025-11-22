@page module-component ftxui / component
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595349.png)

`ftxui::component` 模块定义了生成响应用户事件（键盘、鼠标等）的交互式组件的逻辑。

@subpage module-component-examples 部分提供了一系列示例。

`ftxui::ScreenInteractive` 定义了一个渲染组件的主循环。

`ftxui::Component` 是 `ftxui::ComponentBase` 的共享指针。后者定义了：
  - `ftxui::ComponentBase::Render()`：如何渲染界面。
  - `ftxui::ComponentBase::OnEvent()`：如何响应事件。
  - `ftxui::ComponentBase::Add()`：在两个组件之间构建父子关系。组件树用于定义如何使用键盘进行导航。

`ftxui::Element` 用于渲染单个帧。

`ftxui::Component` 用于渲染动态用户界面，生成多个帧，并在事件发生时更新其状态。

[组件库](https://arthursonzogni.github.io/FTXUI/examples_2component_2gallery_8cpp-example.html) (多种组件的集合)。([演示](https://arthursonzogni.github.io/FTXUI/examples/?file=component/gallery))

![image](https://user-images.githubusercontent.com/4759106/147247330-b60beb9f-e665-48b4-81c0-4b01ee95bc66.png)

所有预定义组件都可以在
["ftxui/dom/component.hpp"](./component_8hpp.html) 中找到

\include ftxui/component/component.hpp

# Input {#component-input}

[示例](https://arthursonzogni.github.io/FTXUI/examples_2component_2input_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147247671-f1d6f606-1845-4e94-a4a0-d4273e9ae6bd.png)

由以下生成: `ftxui::Input()` 来自 "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223719" src="https://asciinema.org/a/223719.js" async></script>
@endhtmlonly

## Filtered input

可以使用 `ftxui::CatchEvent` 过滤输入组件接收到的字符。

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

定义一个菜单对象。它包含一个条目列表，其中一个被选中。

[示例](https://arthursonzogni.github.io/FTXUI/examples_2component_2menu_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147247822-0035fd6f-bb13-4b3a-b057-77eb9291582f.png)


由以下生成: `ftxui::Menu()` 来自 "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223720" src="https://asciinema.org/a/223720.js" async></script>
@endhtmlonly

# Toggle {#component-toggle}

一种特殊的菜单。条目水平显示。

[示例](https://arthursonzogni.github.io/FTXUI/examples_2component_2toggle_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147249383-e2201cf1-b7b8-4a5a-916f-d761e3e7ae40.png)

由以下生成: `ftxui::Toggle()` 来自 "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223722" src="https://asciinema.org/a/223722.js" async></script>
@endhtmlonly

# CheckBox {#component-checkbox}

该组件定义了一个复选框。它是一个可以打开/关闭的单个条目。

[示例](https://arthursonzogni.github.io/FTXUI/examples_2component_2checkbox_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147246646-b86926a9-1ef9-4efb-af98-48a9b62acd81.png)

由以下生成: `ftxui::Checkbox()` 来自 "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223724" src="https://asciinema.org/a/223724.js" async></script>
@endhtmlonly

# RadioBox {#component-radiobox}

一个单选按钮组件。这是一个条目列表，其中一个可以被选中。

# Dropdown {#component-dropdown}

下拉菜单是一种组件，当打开时，它会显示一个元素列表供用户选择。

[示例](https://arthursonzogni.github.io/FTXUI/examples_2component_2dropdown_8cpp-example.html):

![youtube-video-gif (3)](https://user-images.githubusercontent.com/4759106/147246982-1e821751-531c-4e1f-bc37-2fa290e143cd.gif)

由以下生成: `ftxui::Dropdown()` 来自 "ftxui/component/component.hpp"

# Slider {#component-slider}

表示一个滑块对象，它由一个带有分箱中间间隔的范围组成。它可以通过 `ftxui::Slider()` 创建。

[Example](https://arthursonzogni.github.io/FTXUI/examples_2component_2slider_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147249265-7e2cad75-082c-436e-affe-44a550c480ab.png)

Produced by: `ftxui::Slider()` from "ftxui/component/component.hpp"

# Renderer {#component-renderer}



由以下生成: `ftxui::Renderer()` 来自 \ref ftxui/component/component.hpp。该组件通过使用不同的函数渲染界面来装饰另一个组件。



示例:

```cpp

auto inner = [...]



auto renderer = Renderer(inner, [&] {

  return inner->Render() | border

});

```



`ftxui::Renderer` 也支持组件装饰器模式:

```cpp

auto component = [...]

component = component

  | Renderer([](Element e) { return e | border))

  | Renderer(bold)

```



作为简写，你也可以将一个组件与一个元素装饰器组合:

```cpp

auto component = [...]

component = component | border | bold;

```



# CatchEvent {#component-catchevent}







由以下生成: `ftxui::CatchEvent()` 来自 \ref ftxui/component/component.hpp。



该组件装饰其他组件，在底层组件之前捕获事件。







示例:



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







`ftxui::CatchEvent` 也可以用作装饰器:



```cpp



component = component



  | CatchEvent(handler_1)



  | CatchEvent(handler_2)



  | CatchEvent(handler_3)



  ;



```







# Collapsible {#component-collapsible}







对于其可见性可以通过用户切换打开或关闭的视觉元素很有用。本质上，这是 `ftxui::Checkbox()` 和 `ftxui::Maybe()` 组件的组合。







```cpp



auto collapsible = Collapsible("Show more", inner_element);



```







# Maybe {#component-maybe}







由以下生成: `ftxui::Maybe()` 来自 \ref ftxui/component/component.hpp。



该组件可以通过布尔值或谓词显示/隐藏任何其他组件。







布尔值示例:



```cpp



bool show = true;



auto component = Renderer([]{ return "Hello World!"; });



auto maybe_component = Maybe(component, &show)



```







谓词示例:



```cpp



auto component = Renderer([]{ return "Hello World!"; });



auto maybe_component = Maybe(component, [&] { return time > 10; })



```







像往常一样，`ftxui::Maybe` 也可以用作装饰器:



```cpp



component = component



  | Maybe(&a_boolean)



  | Maybe([&] { return time > 10; })



  ;



```







# Container {#component-container}

## Horizontal {#component-horizontal}

由以下生成: `ftxui::Container::Horizontal()` 来自
"ftxui/component/component.hpp"。它水平显示组件列表并处理键盘/鼠标导航。

## Vertical {#component-vertical}

由以下生成: `ftxui::Container::Vertical()` 来自
"ftxui/component/component.hpp"。它垂直显示组件列表并处理键盘/鼠标导航。

## Tab {#component-tab}

由以下生成: `ftxui::Container::Tab()` 来自
"ftxui/component/component.hpp"。它接受一个组件列表并只显示其中一个。这对于实现选项卡栏很有用。

[Vertical](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_vertical_8cpp-example.html):
  
![ezgif com-gif-maker (1)](https://user-images.githubusercontent.com/4759106/147250144-22ff044a-4773-4ff7-a49c-12ba4034acb4.gif)

[Horizontal](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_horizontal_8cpp-example.html):
  
  ![ezgif com-gif-maker (2)](https://user-images.githubusercontent.com/4759106/147250217-fe447e0f-7a99-4e08-948a-995087d9b40e.gif)


# ResizableSplit {#component-resizable-split}





它定义了两个子组件之间的水平或垂直分隔。分隔的位置是可变的，可以通过鼠标控制。


有四种可能的分隔:


- `ftxui::ResizableSplitLeft()`


- `ftxui::ResizableSplitRight()`


- `ftxui::ResizableSplitTop()`


- `ftxui::ResizableSplitBottom()`


来自 "ftxui/component/component.hpp"





[示例](https://arthursonzogni.github.io/FTXUI/examples_2component_2resizable_split_8cpp-example.html):





![ezgif com-gif-maker](https://user-images.githubusercontent.com/4759106/147248372-c55512fe-9b96-4b08-a1df-d05cf2cae431.gif)  





@htmlonly


<script id="asciicast-tprMH2EdkUoMb7D2YxgMGgpzx" src="https://asciinema.org/a/tprMH2EdkUoMb7D2YxgMGgpzx.js" async></script>


@endhtmlonly





# Force a frame redraw. {#component-force-redraw}





通常，`ftxui::ScreenInteractive::Loop()` 负责在处理完一组新事件（例如键盘、鼠标、窗口大小调整等）时绘制新帧。但是，你可能希望对 FTXUI 未知的任意事件作出反应。为了实现这一点，你必须通过线程使用 `ftxui::ScreenInteractive::PostEvent`（**这是线程安全的**）发布事件。你必须发布事件 `ftxui::Event::Custom`。





示例:


```cpp


screen->PostEvent(Event::Custom);


```





如果你不需要处理新事件，可以使用:


```cpp


screen->RequestAnimationFrame();


```


代替。
