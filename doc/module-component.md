@page module-component ftxui / component
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595349.png)

Le module `ftxui::component` définit la logique qui produit des
composants interactifs qui répondent aux événements utilisateur (clavier,
souris, etc.).

La section @subpage module-component-examples fournit une collection d'exemples.

Un `ftxui::ScreenInteractive` définit une boucle principale qui rend un composant.

Un `ftxui::Component` est un pointeur partagé vers un `ftxui::ComponentBase`. Ce dernier définit :
  - `ftxui::ComponentBase::Render()`: Comment rendre l'interface.
  - `ftxui::ComponentBase::OnEvent()`: Comment réagir aux événements.
  - `ftxui::ComponentBase::Add()`: Construit une relation parent/enfant
    entre deux composants. L'arbre des composants est utilisé pour définir
    comment naviguer à l'aide du clavier.

Les `ftxui::Element` sont utilisés pour rendre une seule trame.

Les `ftxui::Component` sont utilisés pour rendre une interface utilisateur
dynamique, produisant plusieurs trames et mettant à jour son état sur les
événements.

[Galerie](https://arthursonzogni.github.io/FTXUI/examples_2component_2gallery_8cpp-example.html) de plusieurs composants. ([démo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/gallery))

![image](https://user-images.githubusercontent.com/4759106/147247330-b60beb9f-e665-48b4-81c0-4b01ee95bc66.png)

Tous les composants prédéfinis sont disponibles dans
["ftxui/dom/component.hpp"](./component_8hpp.html)

\include ftxui/component/component.hpp

# Input {#component-input}

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2input_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147247671-f1d6f606-1845-4e94-a4a0-d4273e9ae6bd.png)

Produit par : `ftxui::Input()` de "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223719" src="https://asciinema.org/a/223719.js" async></script>
@endhtmlonly

## Saisie filtrée

On peut filtrer les caractères reçus par le composant de saisie, en utilisant
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

Définit un objet menu. Il contient une liste d'entrées, dont une est sélectionnée.

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2menu_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147247822-0035fd6f-bb13-4b3a-b057-77eb9291582f.png)


Produit par : `ftxui::Menu()` de "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223720" src="https://asciinema.org/a/223720.js" async></script>
@endhtmlonly

# Bascule {#component-toggle}

Un type de menu spécial. Les entrées sont affichées horizontalement.

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2toggle_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147249383-e2201cf1-b7b8-4a5a-916f-d761e3e7ae40.png)

Produit par : `ftxui::Toggle()` de "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223722" src="https://asciinema.org/a/223722.js" async></script>
@endhtmlonly

# Case à cocher {#component-checkbox}

Ce composant définit une case à cocher. C'est une seule entrée qui peut être
activée/désactivée.

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2checkbox_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147246646-b86926a9-1ef9-4efb-af98-48a9b62acd81.png)

Produit par : `ftxui::Checkbox()` de "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223724" src="https://asciinema.org/a/223724.js" async></script>
@endhtmlonly

# Bouton radio {#component-radiobox}

Un composant bouton radio. C'est une liste d'entrées, où une seule peut être
activée.

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2radiobox_8cpp-example.html):
  
![image](https://user-images.githubusercontent.com/4759106/147246401-809d14a5-6621-4e36-8dd9-a2d75ef2a94e.png)

Produit par : `ftxui::Radiobox()` de "ftxui/component/component.hpp"

@htmlonly
<script id="asciicast-223725" src="https://asciinema.org/a/223725.js" async></script>
@endhtmlonly

# Menu déroulant {#component-dropdown}

Un menu déroulant est un composant qui, lorsqu'il est ouvert, affiche une
liste d'éléments parmi lesquels l'utilisateur peut choisir.

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2dropdown_8cpp-example.html):

![youtube-video-gif (3)](https://user-images.githubusercontent.com/4759106/147246982-1e821751-531c-4e1f-bc37-2fa290e143cd.gif)

Produit par : `ftxui::Dropdown()` de "ftxui/component/component.hpp"

# Curseur {#component-slider}

Représente un objet curseur composé d'une plage avec des intervalles
intermédiaires binés. Il peut être créé par `ftxui::Slider()`.

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2slider_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147249265-7e2cad75-082c-436e-affe-44a550c480ab.png)

Produit par : `ftxui::Slider()` de "ftxui/component/component.hpp"

# Rendu {#component-renderer}

Produit par : `ftxui::Renderer()` de \ref ftxui/component/component.hpp. Ce
composant décore un autre en utilisant une fonction différente pour rendre une
interface.

Exemple:
```cpp
auto inner = [...]

auto renderer = Renderer(inner, [&] {
  return inner->Render() | border
});
```

`ftxui::Renderer` supporte également le modèle de décorateur de composant :
```cpp
auto component = [...]
component = component
  | Renderer([](Element e) { return e | border))
  | Renderer(bold)
```

Comme raccourci, vous pouvez également composer un composant avec un décorateur d'élément :
```cpp
auto component = [...]
component = component | border | bold;
```

# Intercepter un événement {#component-catchevent}

Produit par : `ftxui::CatchEvent()` de \ref ftxui/component/component.hpp.
Ce composant décore d'autres composants, interceptant les événements avant le
composant sous-jacent.

Exemples:
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

Le `ftxui::CatchEvent` peut également être utilisé comme décorateur :
```cpp
component = component
  | CatchEvent(handler_1)
  | CatchEvent(handler_2)
  | CatchEvent(handler_3)
  ;
```

# Rétractable {#component-collapsible}

Utile pour les éléments visuels dont la visibilité peut être activée ou
désactivée par l'utilisateur. Essentiellement, c'est la combinaison des
composants `ftxui::Checkbox()` et `ftxui::Maybe()`.

```cpp
auto collapsible = Collapsible("Show more", inner_element);
```

# Peut-être {#component-maybe}

Produit par : `ftxui::Maybe()` de \ref ftxui/component/component.hpp.
Ce composant peut être utilisé pour afficher/masquer tout autre composant via
un booléen ou un prédicat.

Exemple avec un booléen :
```cpp
bool show = true;
auto component = Renderer([]{ return "Hello World!"; });
auto maybe_component = Maybe(component, &show)
```

Exemple avec un prédicat :
```cpp
auto component = Renderer([]{ return "Hello World!"; });
auto maybe_component = Maybe(component, [&] { return time > 10; })
```

Comme d'habitude, `ftxui::Maybe` peut également être utilisé comme décorateur :
```cpp
component = component
  | Maybe(&a_boolean)
  | Maybe([&] { return time > 10; })
  ;
```

# Conteneur {#component-container}

## Horizontal {#component-horizontal}

Produit par : `ftxui::Container::Horizontal()` de
"ftxui/component/component.hpp". Il affiche une liste de composants
horizontalement et gère la navigation au clavier/souris.

## Vertical {#component-vertical}

Produit par : `ftxui::Container::Vertical()` de
"ftxui/component/component.hpp". Il affiche une liste de composants
verticalement et gère la navigation au clavier/souris.

## Onglet {#component-tab}

Produit par : `ftxui::Container::Tab()` de
"ftxui/component/component.hpp". Il prend une liste de composants et
n'en affiche qu'un seul. C'est utile pour implémenter une barre d'onglets.

[Vertical](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_vertical_8cpp-example.html):
  
![ezgif com-gif-maker (1)](https://user-images.githubusercontent.com/4759106/147250144-22ff044a-4773-4ff7-a49c-12ba4034acb4.gif)

[Horizontal](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_horizontal_8cpp-example.html):
  
  ![ezgif com-gif-maker (2)](https://user-images.githubusercontent.com/4759106/147250217-fe447e0f-7a99-4e08-948a-995087d9b40e.gif)


# Séparateur redimensionnable {#component-resizable-split}

Il définit une séparation horizontale ou verticale entre deux composants
enfants. La position du séparateur est variable et contrôlable à l'aide de la
souris. Il existe quatre séparateurs possibles :
- `ftxui::ResizableSplitLeft()`
- `ftxui::ResizableSplitRight()`
- `ftxui::ResizableSplitTop()`
- `ftxui::ResizableSplitBottom()`
de "ftxui/component/component.hpp"

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2resizable_split_8cpp-example.html):

![ezgif com-gif-maker](https://user-images.githubusercontent.com/4759106/147248372-c55512fe-9b96-4b08-a1df-d05cf2cae431.gif)  

@htmlonly
<script id="asciicast-tprMH2EdkUoMb7D2YxgMGgpzx" src="https://asciinema.org/a/tprMH2EdkUoMb7D2YxgMGgpzx.js" async></script>
@endhtmlonly

# Forcer un nouveau rendu de trame. {#component-force-redraw}

Généralement, `ftxui::ScreenInteractive::Loop()` est responsable de dessiner une
nouvelle trame chaque fois qu'un nouveau groupe d'événements (par exemple
clavier, souris, redimensionnement de fenêtre, etc.) a été traité. Cependant,
vous pourriez vouloir réagir à des événements arbitraires inconnus de FTXUI.
Pour ce faire, vous devez poster des événements à l'aide de
`ftxui::ScreenInteractive::PostEvent` (**ceci est thread-safe**) via un thread.
Vous devrez poster l'événement `ftxui::Event::Custom`.

Exemple:
```cpp
screen->PostEvent(Event::Custom);
```

Si vous n'avez pas besoin de traiter un nouvel événement, vous pouvez utiliser :
```cpp
screen->RequestAnimationFrame();
```
à la place.