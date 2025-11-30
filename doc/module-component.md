@page module-component ftxui / component
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595349.png)

El módulo `ftxui::component` define la lógica que produce componentes interactivos
que responden a los eventos del usuario (teclado, ratón, etc.).

La sección @subpage module-component-examples proporciona una colección de ejemplos.

Un `ftxui::ScreenInteractive` define un bucle principal que renderiza un componente.

Un `ftxui::Component` es un puntero compartido a un `ftxui::ComponentBase`. Este último define:
  - `ftxui::ComponentBase::Render()`: Cómo renderizar la interfaz.
  - `ftxui::ComponentBase::OnEvent()`: Cómo reaccionar a los eventos.
  - `ftxui::ComponentBase::Add()`: Construir una relación padre/hijo
    entre dos componentes. El árbol de componentes se utiliza para definir cómo
    navegar usando el teclado.

`ftxui::Element` se utilizan para renderizar un solo fotograma.

`ftxui::Component` se utilizan para renderizar interfaces de usuario dinámicas, produciendo múltiples
fotogramas y actualizando su estado en los eventos.

[Galería](https://arthursonzogni.github.io/FTXUI/examples_2component_2gallery_8cpp-example.html) de múltiples componentes. ([demo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/gallery))

![image](https://user-images.githubusercontent.com/4759106/147247330-b60beb9f-e665-48b4-81c0-4b01ee95bc66.png)

Todos los componentes predefinidos están disponibles en
["ftxui/dom/component.hpp"](./component_8hpp.html)

\include ftxui/component/component.hpp

# Input {#component-input}

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2input_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147247671-f1d6f606-1845-4e94-a4a0-d4273e9ae6bd.png)

Producido por: `ftxui::Input()` de "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223719" src="https://asciinema.org/a/223719.js" async></script>
@endhtmlonly

## Entrada filtrada

Se pueden filtrar los caracteres recibidos por el componente de entrada, usando
`ftxui::CatchEvent`.

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

Define un objeto de menú. Contiene una lista de entradas, una de ellas seleccionada.

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2menu_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147247822-0035fd6f-bb13-4b3a-b057-77eb9291582f.png)


Producido por: `ftxui::Menu()` de "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223720" src="https://asciinema.org/a/223720.js" async></script>
@endhtmlonly

# Toggle {#component-toggle}

Un tipo especial de menú. Las entradas se muestran horizontalmente.

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2toggle_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147249383-e2201cf1-b7b8-4a5a-916f-d761e3e7ae40.png)

Producido por: `ftxui::Toggle()` de "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223722" src="https://asciinema.org/a/223722.js" async></script>
@endhtmlonly

# CheckBox {#component-checkbox}

Este componente define una casilla de verificación. Es una sola entrada que puede
activarse/desactivarse.

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2checkbox_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147246646-b86926a9-1ef9-4efb-af98-48a9b62acd81.png)

Producido por: `ftxui::Checkbox()` de "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223724" src="https://asciinema.org/a/223724.js" async></script>
@endhtmlonly

# RadioBox {#component-radiobox}

Un componente de radiobotón. Es una lista de entradas, donde una puede activarse.

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2radiobox_8cpp-example.html):
  
![image](https://user-images.githubusercontent.com/4759106/147246401-809d14a5-6621-4e36-8dd9-a2d75ef2a94e.png)

Producido por: `ftxui::Radiobox()` de "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223725" src="https://asciinema.org/a/223725.js" async></script>
@endhtmlonly

# Dropdown {#component-dropdown}

Un menú desplegable es un componente que, al abrirse, muestra una lista de
elementos para que el usuario seleccione.

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2dropdown_8cpp-example.html):

![youtube-video-gif (3)](https://user-images.githubusercontent.com/4759106/147246982-1e821751-531c-4e1f-bc37-2fa290e143cd.gif)

Producido por: `ftxui::Dropdown()` de "ftxui/component/component.hpp"

# Slider {#component-slider}

Representa un objeto deslizador que consiste en un rango con intervalos
intermedios categorizados. Puede ser creado por `ftxui::Slider()`.

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2slider_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147249265-7e2cad75-082c-436e-affe-44a550c480ab.png)

Producido por: `ftxui::Slider()` de "ftxui/component/component.hpp"

# Renderer {#component-renderer}

Producido por: `ftxui::Renderer()` de \ref ftxui/component/component.hpp. Este
componente decora otro utilizando una función diferente para renderizar una
interfaz.

Ejemplo:
```cpp
auto inner = [...] 

auto renderer = Renderer(inner, [&] {
  return inner->Render() | border
});
```

`ftxui::Renderer` también soporta el patrón de decorador de componentes:
```cpp
auto component = [...]
component = component
  | Renderer([](Element e) { return e | border))
  | Renderer(bold)
```

Como una forma abreviada, también puedes componer un componente con un decorador de elemento:
```cpp
auto component = [...] 
component = component | border | bold;
```

# CatchEvent {#component-catchevent}

Producido por: `ftxui::CatchEvent()` de \ref ftxui/component/component.hpp.
Este componente decora a otros, capturando eventos antes del componente subyacente.

Ejemplos:
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

`ftxui::CatchEvent` también se puede usar como un decorador:
```cpp
component = component
  | CatchEvent(handler_1)
  | CatchEvent(handler_2)
  | CatchEvent(handler_3)
  ;
```

# Collapsible {#component-collapsible}

Útil para elementos visuales cuya visibilidad puede ser activada o desactivada
por el usuario. Esencialmente, es la combinación de los componentes
`ftxui::Checkbox()` y `ftxui::Maybe()`.

```cpp
auto collapsible = Collapsible("Show more", inner_element);
```

# Maybe {#component-maybe}

Producido por: `ftxui::Maybe()` de \ref ftxui/component/component.hpp.
Este componente puede ser utilizado para mostrar/ocultar cualquier otro componente
mediante un booleano o un predicado.

Ejemplo con un booleano:
```cpp
bool show = true;
auto component = Renderer([]{ return "Hello World!"; });
auto maybe_component = Maybe(component, &show)
```

Ejemplo con un predicado:
```cpp
auto component = Renderer([]{ return "Hello World!"; });
auto maybe_component = Maybe(component, [&] { return time > 10; })
```

Como de costumbre, `ftxui::Maybe` también se puede usar como un decorador:
```cpp
component = component
  | Maybe(&a_boolean)
  | Maybe([&] { return time > 10; })
  ;
```

# Container {#component-container}

## Horizontal {#component-horizontal}

Producido por: `ftxui::Container::Horizontal()` de
"ftxui/component/component.hpp". Muestra una lista de componentes
horizontalmente y gestiona la navegación con teclado/ratón.

## Vertical {#component-vertical}

Producido por: `ftxui::Container::Vertical()` de
"ftxui/component/component.hpp". Muestra una lista de componentes
verticalmente y gestiona la navegación con teclado/ratón.

## Tab {#component-tab}

Producido por: `ftxui::Container::Tab()` de
"ftxui/component/component.hpp". Toma una lista de componentes y muestra
solo uno de ellos. Esto es útil para implementar una barra de pestañas.

[Vertical](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_vertical_8cpp-example.html):
  
![ezgif com-gif-maker (1)](https://user-images.githubusercontent.com/4759106/147250144-22ff044a-4773-4ff7-a49c-12ba4034acb4.gif)

[Horizontal](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_horizontal_8cpp-example.html):
  
  ![ezgif com-gif-maker (2)](https://user-images.githubusercontent.com/4759106/147250217-fe447e0f-7a99-4e08-948a-995087d9b40e.gif)


# ResizableSplit  {#component-resizable-split}

Define una separación horizontal o vertical entre dos componentes hijos.
La posición de la división es variable y controlable con el ratón.
Hay cuatro posibles divisiones:
- `ftxui::ResizableSplitLeft()`
- `ftxui::ResizableSplitRight()`
- `ftxui::ResizableSplitTop()`
- `ftxui::ResizableSplitBottom()`
de "ftxui/component/component.hpp"

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2resizable_split_8cpp-example.html):

![ezgif com-gif-maker](https://user-images.githubusercontent.com/4759106/147248372-c55512fe-9b96-4b08-a1df-d05cf2cae431.gif)  

@htmlonly
<script id="asciicast-tprMH2EdkUoMb7D2YxgMGgpzx" src="https://asciinema.org/a/tprMH2EdkUoMb7D2YxgMGgpzx.js" async></script>
@endhtmlonly

# Forzar un nuevo renderizado de fotogramas. {#component-force-redraw}

Normalmente, `ftxui::ScreenInteractive::Loop()` es responsable de dibujar un nuevo
fotograma cada vez que se ha procesado un nuevo grupo de eventos (p. ej., teclado,
ratón, cambio de tamaño de ventana, etc.). Sin embargo, es posible que desees
reaccionar a eventos arbitrarios que FTXUI desconoce. Para lograr esto, debes
publicar eventos usando `ftxui::ScreenInteractive::PostEvent` (**esto es seguro
para hilos**) a través de un hilo. Tendrás que publicar el evento
`ftxui::Event::Custom`.

Ejemplo:
```cpp
screen->PostEvent(Event::Custom);
```

Si no necesitas procesar un nuevo evento, puedes usar:
```cpp
screen->RequestAnimationFrame();
```
en su lugar.