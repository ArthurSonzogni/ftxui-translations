@page module-dom ftxui / dom
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595347.png)

Ce module définit un ensemble hiérarchique de `ftxui::Element`. Un élément gère
la disposition et peut être réactif aux changements de dimensions du terminal.
Considérez l'exemple suivant où ce module est utilisé pour créer une disposition
simple avec un certain nombre d'opérateurs :

La section @subpage module-dom-examples fournit une collection d'exemples.

**Exemple :**
```cpp
namespace ftxui {
    ...

// Définir le document
Element document = vbox({
  text("La fenêtre") | bold | color(Color::Blue),
  gauge(0.5)
  text("Le pied de page")
});

// Ajouter une bordure, en appelant la fonction décoratrice `ftxui::border`.
document = border(document);

// Ajouter une autre bordure, en utilisant l'opérateur pipe.
document = document | border.

// Ajouter une autre bordure, en utilisant l'opérateur |=.
document |= border

...
}
```

**Liste des éléments**

La liste de tous les éléments est incluse et peut être accédée en incluant le
fichier d'en-tête correspondant :
```cpp
#include <ftxui/dom/elements.hpp>
```

\include{strip} "ftxui/dom/elements.hpp"

# text # {#dom-text}

Le widget le plus simple. Il affiche un texte.
```cpp
text("Je suis un morceau de texte");
```
```bash
I am a piece of text.
```

# vtext {#dom-vtext}

Identique à `ftxui::text`, mais affiché verticalement.

Code:
```cpp
vtext("HELLO");
```

Terminal output:
```bash
H
E
L
L
O
```

# paragraph {#dom-paragraph}

Similaire à `ftxui::text`, mais les mots individuels sont enveloppés sur
plusieurs lignes, en fonction de la largeur de son conteneur.

Exemple de code :
```cpp
paragraph("Un très long texte")
```

![ezgif com-gif-maker (4)](https://user-images.githubusercontent.com/4759106/147251370-983a06e7-6f41-4113-92b8-942f43d34d06.gif)

Pour un exemple plus détaillé, reportez-vous à [exemple détaillé](https://arthursonzogni.github.io/FTXUI/examples_2dom_2paragraph_8cpp-example.html).
Paragraph inclut également un certain nombre d'autres variantes comme indiqué ci-dessous :
```cpp
namespace ftxui {
    Element paragraph(std::string text);
    Element paragraphAlignLeft(std::string text);
    Element paragraphAlignRight(std::string text);
    Element paragraphAlignCenter(std::string text);
    Element paragraphAlignJustify(std::string text);
}
```


# border {#dom-border}

Ajoute une bordure autour d'un élément.

Code :
```cpp
border(text("L'élément"))
```

Sortie terminale :
```bash
┌───────────┐
│The element│
└───────────┘
```

> [!note]
> Vous pouvez obtenir le même comportement en utilisant l'opérateur pipe.
> 
> Code :
> ```cpp
> text("L'élément") | border
> ```

Border est également disponible dans une variété de styles, comme indiqué ci-dessous :
```cpp
namespace ftxui {
    Element border(Element);
    Element borderLight(Element);
    Element borderHeavy(Element);
    Element borderDouble(Element);
    Element borderRounded(Element);
    Element borderEmpty(Element);
    Decorator borderStyled(BorderStyle);
    Decorator borderWith(Pixel);
}
```


# window # {#dom-window}

Une `ftxui::window` est une `ftxui::border`, mais avec un en-tête
supplémentaire. Pour ajouter une fenêtre autour d'un élément, enveloppez-le
et spécifiez une chaîne de caractères comme en-tête.
Code :
```cpp
window("La fenêtre", text("L'élément"))
```

Sortie terminale :
```bash
┌The window─┐
│The element│
└───────────┘
```

# separator {#dom-separator}

Affiche une ligne verticale/horizontale pour diviser visuellement le contenu
d'un conteneur en deux.

Code :
```cpp
border(
  hbox({
    text("Gauche"), 
    separator(),
    text("Droite")
  })
)
```

Sortie terminale :
```bash
┌────┬─────┐
│left│right│
└────┴─────┘
```


Les séparateurs sont disponibles dans une variété de styles, comme indiqué ci-dessous :
```cpp
namespace ftxui {
    Element separator(void);
    Element separatorLight();
    Element separatorHeavy();
    Element separatorDouble();
    Element separatorEmpty();
    Element separatorStyled(BorderStyle);
    Element separator(Pixel);
    Element separatorCharacter(std::string);
    Element separatorHSelector(float left,
                               float right,
                               Color background,
                               Color foreground);
    Element separatorVSelector(float up,
                               float down,
                               Color background,
                               Color foreground);
}
```

# gauge {#dom-gauge}

Ceci est un élément visuel qui représente un ratio de progression.

Code :
```cpp
border(gauge(0.5))
```

Sortie terminale :
```bash
┌────────────────────────────────────────────────────────────────────────────┐
│██████████████████████████████████████                                      │
└────────────────────────────────────────────────────────────────────────────┘
```

Les jauges peuvent être affichées dans de nombreuses orientations, comme indiqué ci-dessous :
```cpp
namespace {
    Element gauge(float ratio);
    Element gaugeLeft(float ratio);
    Element gaugeRight(float ratio);
    Element gaugeUp(float ratio);
    Element gaugeDown(float ratio);
    Element gaugeDirection(float ratio, GaugeDirection);
}
```

# graph {#dom-graph}

@htmlonly
<script id="asciicast-223726" src="https://asciinema.org/a/223726.js" async></script>
@endhtmlonly

Voir :
```cpp
Element graph(GraphFunction);
```

# Couleurs {#dom-colors}

La plupart des consoles de terminal peuvent afficher du texte coloré et des
arrière-plans colorés. FTXUI prend en charge toutes les palettes de couleurs :
```cpp
Decorator color(Color);
Decorator bgcolor(Color);
```


[Galerie](https://arthursonzogni.github.io/FTXUI/examples_2dom_2color_gallery_8cpp-example.html) de couleurs :
![image](https://user-images.githubusercontent.com/4759106/147248595-04c7245a-5b85-4544-809d-a5984fc6f9e7.png)

## Palette16 #{#dom-colors-palette-16}

Sur la plupart des terminaux, les couleurs suivantes sont prises en charge :
- Par défaut

- Noir
- GrisFoncé
- GrisClair

- Blanc

- Bleu
- BleuClair

- Cyan
- CyanClair

- Vert
- VertClair

- Magenta
- MagentaClair

- Rouge
- RougeClair

- Jaune
- JauneClair

Exemple d'utilisation des couleurs ci-dessus avec l'opérateur pipe :
```cpp
text("Premier plan bleu") | color(Color::Blue);
text("Arrière-plan bleu") | bgcolor(Color::Blue);
text("Noir sur blanc") | color(Color::Black) | bgcolor(Color::White);
```

## Palette256 #{#dom-colors-palette-256}

Sur les terminaux prenant en charge 256 couleurs.
@htmlonly
<script id="asciicast-OAUc3n6QrkmrLt7XEEb8AzbLt" src="https://asciinema.org/a/OAUc3n6QrkmrLt7XEEb8AzbLt.js" async></script>
@endhtmlonly

```cpp
text("RoseVif") | color(Color::HotPink);
```

## TrueColor #{#dom-colors-true-color}

Sur les terminaux prenant en charge TrueColor, vous pouvez utiliser directement
l'espace colorimétrique RVB 24 bits :

Utilisez les constructeurs ci-dessous pour spécifier les valeurs **RVB** ou **TSV** de votre
couleur :

Il existe deux constructeurs :
```cpp
ftxui::Color::RGB(uint8_t red, uint8_t green, uint8_t blue);
ftxui::Color::HSV(uint8_t hue, uint8_t saturation, uint8_t value);
```

@htmlonly
<script id="asciicast-dk5r8IcCH0aFIIgWG0keSEHMG" src="https://asciinema.org/a/dk5r8IcCH0aFIIgWG0keSEHMG.js" async></script>
<script id="asciicast-xwzzghmqcqzIuyLwCpQFEqbEu" src="https://asciinema.org/a/xwzzghmqcqzIuyLwCpQFEqbEu.js" async></script>
@endhtmlonly

# LinearGradient #{#dom-linear-gradient}

FTXUI prend en charge le dégradé linéaire. Soit au premier plan, soit à l'arrière-plan.

```cpp
Decorator color(const LinearGradient&);
Decorator bgcolor(const LinearGradient&);
```

Un `ftxui::LinearGradient` est défini par un angle en degrés et une liste de
points de couleur.
```cpp
auto gradient = LinearGradient()
  .Angle(45)
  .AddStop(0.0, Color::Red)
  .AddStop(0.5, Color::Green)
  .AddStop(1.0, Color::Blue);
```

Vous pouvez également utiliser des constructeurs simplifiés :
```cpp
LinearGradient(Color::Red, Color::Blue);
```
```cpp
LinearGradient(45, Color::Red, Color::Blue);
```

Voir la [démo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/linear_gradient_gallery).


# Style {#dom-style}
En plus du texte coloré et des arrière-plans colorés. De nombreux terminaux
prennent en charge les effets de texte tels que : `bold`, `italic`, `dim`,
`underlined`, `inverted`, `blink`.

```cpp
Element bold(Element);
Element italic(Element);
Element dim(Element);
Element inverted(Element);
Element underlined(Element);
Element underlinedDouble(Element);
Element strikethrough(Element);
Element blink(Element);
Decorator color(Color);
Decorator bgcolor(Color);
Decorator colorgrad(LinearGradient);
Decorator bgcolorgrad(LinearGradient);
```

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2dom_2style_gallery_8cpp-example.html)

![image](https://user-images.githubusercontent.com/4759106/147244118-380bf834-9e33-40df-9ff0-07c10f2598ef.png)

Pour utiliser ces effets, enveloppez simplement vos éléments avec l'effet désiré :
```cpp
underlined(bold(text("Ce texte est en gras et souligné")))
```

Alternativement, utilisez l'opérateur pipe pour le chaîner sur votre élément :
```cpp
text("Ce texte est en gras") | bold | underlined
```

# Layout {#dom-layout}

Permet d'organiser les éléments des manières suivantes :
  - **Horizontalement** avec `ftxui::hbox`
  - **Verticalement** avec `ftxui::vbox`
  - **À l'intérieur d'une grille** avec `ftxui::gridbox`
  - **Enveloppé dans une direction** en utilisant le `ftxui::flexbox`.
  
[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2dom_2vbox_hbox_8cpp-example.html) utilisant `ftxui::hbox`, `ftxui::vbox` et `ftxui::filler`.

![image](https://user-images.githubusercontent.com/4759106/147242524-7103b5d9-1a92-4e2d-ac70-b3d6740061e3.png)
  
  
[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2dom_2gridbox_8cpp-example.htmlp)
utilisant `ftxui::gridbox` :

![image](https://user-images.githubusercontent.com/4759106/147242972-0db1f2e9-0790-496f-86e6-ed2c604f7a73.png)

[Exemple](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/hflow.cpp)
utilisant flexbox :

![image](https://user-images.githubusercontent.com/4759106/147243064-780ac7cc-605b-475f-94b8-cf7c4aed03a5.png)

Consultez cet
[exemple](https://arthursonzogni.github.io/FTXUI/examples_2dom_2hflow_8cpp-example.html)
et la
[démo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/flexbox) associée.

L'élément peut également devenir flexible en utilisant le décorateur `ftxui::flex`.

Code :
```cpp
  hbox({
    text("gauche") | border ,
    text("milieu") | border | flex,
    text("droite") | border,
  });
```
Sortie terminale :
```bash
┌────┐┌─────────────────────────────────────────────────────┐┌─────┐
│left││middle                                               ││right│
└────┘└─────────────────────────────────────────────────────┘└─────┘
```

Code :
```cpp
  hbox({
    text("gauche") | border ,
    text("milieu") | border | flex,
    text("droite") | border | flex,
  });
```

Sortie terminale :
```bash
┌────┐┌───────────────────────────────┐┌───────────────────────────────┐
│left││middle                         ││right                          │
└────┘└───────────────────────────────┘└───────────────────────────────┘
```

# Table {#dom-table}

Permet un formatage facile des données sous une forme visuelle de tableau ordonné.

[Exemple de code](https://arthursonzogni.github.io/FTXUI/examples_2dom_2table_8cpp-example.html):
  
![image](https://user-images.githubusercontent.com/4759106/147250766-77d8ec9e-cf2b-486d-9866-1fd9f1bd2e6b.png)

# Canvas {#dom-canvas}

Voir l'API [<ftxui/dom/canvas.hpp>](./canvas_8hpp_source.html)

```cpp
  auto c = Canvas(100, 100);
  c.DrawPointLine(10, 10, 80, 10, Color::Red);
  auto element = canvas(c);
```

Le dessin peut être effectué sur un `ftxui::Canvas`, en utilisant des caractères
braille, des blocs ou de simples caractères :
  
[Exemple](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/canvas.cpp) simple :
  
![image](https://user-images.githubusercontent.com/4759106/147245843-76cc62fb-ccb4-421b-aacf-939f9afb42fe.png)

[Exemple](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/component/canvas_animated.cpp) complexe :
  
![ezgif com-gif-maker (3)](https://user-images.githubusercontent.com/4759106/147250538-783a8246-98e0-4a25-b032-3bd3710549d1.gif)
