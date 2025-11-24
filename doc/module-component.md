@page module-component ftxui / component
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595349.png)

`ftxui::component` モジュールは、ユーザーイベント (キーボード、マウスなど) に応答するインタラクティブなコンポーネントを生成するロジックを定義します。

@subpage module-component-examples セクションには、例のコレクションが記載されています。

`ftxui::ScreenInteractive` は、コンポーネントをレンダリングするメインループを定義します。

`ftxui::Component` は `ftxui::ComponentBase` への共有ポインタです。後者は以下を定義します。
  - `ftxui::ComponentBase::Render()`: インターフェースのレンダリング方法。
  - `ftxui::ComponentBase::OnEvent()`: イベントへの反応方法。
  - `ftxui::ComponentBase::Add()`: 2つのコンポーネント間の親子関係を構築します。コンポーネントのツリーは、キーボードを使用したナビゲーション方法を定義するために使用されます。

`ftxui::Element` は単一のフレームをレンダリングするために使用されます。

`ftxui::Component` は、動的なユーザーインターフェースをレンダリングし、複数のフレームを生成し、イベントに応じてその状態を更新するために使用されます。

複数のコンポーネントの[ギャラリー](https://arthursonzogni.github.io/FTXUI/examples_2component_2gallery_8cpp-example.html)。([デモ](https://arthursonzogni.github.io/FTXUI/examples/?file=component/gallery))

![image](https://user-images.githubusercontent.com/4759106/147247330-b60beb9f-e665-48b4-81c0-4b01ee95bc66.png)

すべての事前定義されたコンポーネントは
["ftxui/dom/component.hpp"](./component_8hpp.html) で利用可能です。

\include ftxui/component/component.hpp

# Input {#component-input}

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2input_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147247671-f1d6f606-1845-4e94-a4a0-d4273e9ae6bd.png)

によって生成されます: "ftxui/component/component.hpp" の `ftxui::Input()`

@htmlonly
<script id="asciicast-223719" src="https://asciinema.org/a/223719.js" async></script>
@endhtmlonly

## Filtered input

`ftxui::CatchEvent` を使用して、入力コンポーネントによって受信される文字をフィルタリングできます。

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

メニューオブジェクトを定義します。エントリのリストが含まれており、そのうちの1つが選択されています。

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2menu_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147247822-0035fd6f-bb13-4b3a-b057-77eb9291582f.png)


によって生成されます: "ftxui/component/component.hpp" の `ftxui::Menu()`

@htmlonly
<script id="asciicast-223720" src="https://asciinema.org/a/223720.js" async></script>
@endhtmlonly

# Toggle {#component-toggle}

特別な種類のメニュー。エントリは水平に表示されます。

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2toggle_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147249383-e2201cf1-b7b8-4a5a-916f-d761e3e7ae40.png)

によって生成されます: "ftxui/component/component.hpp" の `ftxui::Toggle()`

@htmlonly
<script id="asciicast-223722" src="https://asciinema.org/a/223722.js" async></script>
@endhtmlonly

# CheckBox {#component-checkbox}

このコンポーネントはチェックボックスを定義します。これはオン/オフを切り替えることができる単一のエントリです。

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2checkbox_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147246646-b86926a9-1ef9-4efb-af98-48a9b62acd81.png)

によって生成されます: "ftxui/component/component.hpp" の `ftxui::Checkbox()`

@htmlonly
<script id="asciicast-223724" src="https://asciinema.org/a/223724.js" async></script>
@endhtmlonly

# RadioBox {#component-radiobox}

ラジオボタンコンポーネント。これはエントリのリストであり、そのうちの1つをオンにすることができます。

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2radiobox_8cpp-example.html):
  
![image](https://user-images.githubusercontent.com/4759106/147246401-809d14a5-6621-4e36-8dd9-a2d75ef2a94e.png)

によって生成されます: "ftxui/component/component.hpp" の `ftxui::Radiobox()`

@htmlonly
<script id="asciicast-223725" src="https://asciinema.org/a/223725.js" async></script>
@endhtmlonly

# Dropdown {#component-dropdown}

ドロップダウンメニューは、開くとユーザーが選択できる要素のリストを表示するコンポーネントです。

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2dropdown_8cpp-example.html):

![youtube-video-gif (3)](https://user-images.githubusercontent.com/4759106/147246982-1e821751-531c-4e1f-bc37-2fa290e143cd.gif)

によって生成されます: "ftxui/component/component.hpp" の `ftxui::Dropdown()`

# Slider {#component-slider}

区切られた中間間隔を持つ範囲で構成されるスライダーオブジェクトを表します。`ftxui::Slider()` によって作成できます。

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2slider_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147249265-7e2cad75-082c-436e-affe-44a550c480ab.png)

によって生成されます: "ftxui/component/component.hpp" の `ftxui::Slider()`

# Renderer {#component-renderer}

`ftxui::Renderer()` によって生成されます (\ref ftxui/component/component.hpp から)。このコンポーネントは、インターフェースをレンダリングするために別の関数を使用することで、別のコンポーネントを装飾します。

例:
```cpp
auto inner = [...]

auto renderer = Renderer(inner, [&] {
  return inner->Render() | border
});
```

`ftxui::Renderer` はコンポーネントデコレーターパターンもサポートしています。
```cpp
auto component = [...]
component = component
  | Renderer([](Element e) { return e | border))
  | Renderer(bold)
```

短縮形として、コンポーネントを要素デコレーターと組み合わせることもできます。
```cpp
auto component = [...]
component = component | border | bold;
```

# CatchEvent {#component-catchevent}

`ftxui::CatchEvent()` によって生成されます (\ref ftxui/component/component.hpp から)。このコンポーネントは、基になるコンポーネントの前にイベントをキャッチして、他のコンポーネントを装飾します。

例:
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

`ftxui::CatchEvent` はデコレーターとしても使用できます。
```cpp
component = component
  | CatchEvent(handler_1)
  | CatchEvent(handler_2)
  | CatchEvent(handler_3)
  ;
```

# Collapsible {#component-collapsible}

ユーザーが可視性をオン/オフに切り替えることができる視覚要素に役立ちます。本質的に、これは `ftxui::Checkbox()` と `ftxui::Maybe()` コンポーネントの組み合わせです。

```cpp
auto collapsible = Collapsible("Show more", inner_element);
```

# Maybe {#component-maybe}

`ftxui::Maybe()` によって生成されます (\ref ftxui/component/component.hpp から)。このコンポーネントは、ブール値または述語を介して他のコンポーネントを表示/非表示するために利用できます。

ブール値の例:
```cpp
bool show = true;
auto component = Renderer([]{ return "Hello World!"; });
auto maybe_component = Maybe(component, &show)
```

述語の例:
```cpp
auto component = Renderer([]{ return "Hello World!"; });
auto maybe_component = Maybe(component, [&] { return time > 10; })
```

通常どおり、`ftxui::Maybe` はデコレーターとしても使用できます。
```cpp
component = component
  | Maybe(&a_boolean)
  | Maybe([&] { return time > 10; })
  ;
```

# Container {#component-container}

## Horizontal {#component-horizontal}

"ftxui/component/component.hpp" の `ftxui::Container::Horizontal()` によって生成されます。コンポーネントのリストを水平に表示し、キーボード/マウスナビゲーションを処理します。

## Vertical {#component-vertical}

"ftxui/component/component.hpp" の `ftxui::Container::Vertical()` によって生成されます。コンポーネントのリストを垂直に表示し、キーボード/マウスナビゲーションを処理します。

## Tab {#component-tab}

"ftxui/component/component.hpp" の `ftxui::Container::Tab()` によって生成されます。コンポーネントのリストを受け取り、そのうちの1つだけを表示します。これはタブバーを実装するのに役立ちます。

[垂直](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_vertical_8cpp-example.html):
  
![ezgif com-gif-maker (1)](https://user-images.githubusercontent.com/4759106/147250144-22ff044a-4773-4ff7-a49c-12ba4034acb4.gif)

[水平](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_horizontal_8cpp-example.html):
  
  ![ezgif com-gif-maker (2)](https://user-images.githubusercontent.com/4759106/147250217-fe447e0f-7a99-4e08-948a-995087d9b40e.gif)


# ResizableSplit {#component-resizable-split}

2つの子コンポーネント間の水平または垂直の分離を定義します。分割の位置は可変で、マウスで制御できます。
4つの可能な分割があります。
- `ftxui::ResizableSplitLeft()`
- `ftxui::ResizableSplitRight()`
- `ftxui::ResizableSplitTop()`
- `ftxui::ResizableSplitBottom()`
は "ftxui/component/component.hpp" から。

[例](https://arthursonzogni.github.io/FTXUI/examples_2component_2resizable_split_8cpp-example.html):

![ezgif com-gif-maker](https://user-images.githubusercontent.com/4759106/147248372-c55512fe-9b96-4b08-a1df-d05cf2cae431.gif)  

@htmlonly
<script id="asciicast-tprMH2EdkUoMb7D2YxgMGgpzx" src="https://asciinema.org/a/tprMH2EdkUoMb7D2YxgMGgpzx.js" async></script>
@endhtmlonly

# Force a frame redraw. {#component-force-redraw}

通常、`ftxui::ScreenInteractive::Loop()` は、新しいイベントグループ (キーボード、マウス、ウィンドウのリサイズなど) が処理されるたびに新しいフレームを描画する役割を担います。ただし、FTXUI に認識されない任意のイベントに反応したい場合があります。これを達成するには、スレッドを介して `ftxui::ScreenInteractive::PostEvent` (**これはスレッドセーフです**) を使用してイベントをポストする必要があります。イベント `ftxui::Event::Custom` をポストする必要があります。

例:
```cpp
screen->PostEvent(Event::Custom);
```

新しいイベントを処理する必要がない場合は、代わりに以下を使用できます。
```cpp
screen->RequestAnimationFrame();
```
instead.