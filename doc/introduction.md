@mainpage Introduction
@tableofcontents

<img src="https://github.com/ArthurSonzogni/FTXUI/assets/4759106/6925b6da-0a7e-49d9-883c-c890e1f36007" alt="Image de démonstration"></img>

**FTXUI** est une bibliothèque C++ simple et multiplateforme pour les interfaces utilisateur basées sur terminal !

# Fonctionnalités
 * Style fonctionnel. Inspiré par
   [1](https://hackernoon.com/building-reactive-terminal-interfaces-in-c-d392ce34e649?gi=d9fb9ce35901)
   et [React](https://reactjs.org/)
 * Aucune dépendance
 * **Multiplateforme**.
 * Syntaxe simple et élégante (à mon avis)
 * Navigation au clavier et à la souris.
 * Prise en charge de [UTF8](https://en.wikipedia.org/wiki/UTF-8) et des [caractères pleine largeur](https://en.wikipedia.org/wiki/Halfwidth_and_fullwidth_forms) (→ 테스트)
 * Prise en charge des animations. [Démo 1](https://arthursonzogni.github.io/FTXUI/examples/?file=component/menu_underline_animated_gallery), [Démo 2](https://arthursonzogni.github.io/FTXUI/examples/?file=component/button_style)
 * Prise en charge du dessin. [Démo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/canvas_animated)
 * Apprenez par des [exemples](#documentation) et des [tutoriels](#documentation)
 * Plusieurs systèmes de construction et packages : 
 * Bonnes pratiques : documentation, tests, fuzzers, tests de performance, CI automatisée, packaging automatisé, etc...

# Exemple

```cpp
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

int main() {
  using namespace ftxui;

  // Create a simple document with three text elements.
  Element document = hbox({
    text("left")   | border,
    text("middle") | border | flex,
    text("right")  | border,
  });

  // Create a screen with full width and height fitting the document.
  auto screen = Screen::Create(
    Dimension::Full(),       // Width
    Dimension::Fit(document) // Height
  );

  // Render the document onto the screen.
  Render(screen, document);

  // Print the screen to the console.
  screen.Print();
}
```

Sortie attendue :

```
┌────┐┌────────────────────────────────────┐┌─────┐
│left││middle                              ││right│
└────┘└────────────────────────────────────┘└─────┘
```

# Plateformes prises en charge

- Linux
- MacOS
- Windows
- WebAssembly

<div class="section_buttons">
 
|                                  Suivant |
|-----------------------------------------:|
| [Démarrer](getting-started.html) |
 
</div>

@defgroup screen ftxui/screen

Veuillez consulter le [tutoriel](module-screen.html) du module `ftxui/screen`.

@defgroup dom ftxui/dom

Veuillez consulter le [tutoriel](module-dom.html) du module `ftxui/dom`.

@defgroup component ftxui/component

Veuillez consulter le [tutoriel](module-component.html) du module `ftxui/component`.