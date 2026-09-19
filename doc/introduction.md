@mainpage Introducción
@tableofcontents

<img src="https://github.com/ArthurSonzogni/FTXUI/assets/4759106/6925b6da-0a7e-49d9-883c-c890e1f36007" alt="Imagen de demostración"></img>

**FTXUI** es una sencilla biblioteca C++ multiplataforma para interfaces de usuario basadas en terminal.

# Características
 * Estilo funcional. Inspirado en
   [1](https://hackernoon.com/building-reactive-terminal-interfaces-in-c-d392ce34e649?gi=d9fb9ce35901)
   y [React](https://reactjs.org/)
 * Sin dependencias
 * **Multiplataforma**.
 * Sintaxis simple y elegante (en mi opinión)
 * Navegación con teclado y mouse.
 * Compatible con [UTF8](https://en.wikipedia.org/wiki/UTF-8) y [caracteres de ancho completo](https://en.wikipedia.org/wiki/Halfwidth_and_fullwidth_forms) (→ 测试)
 * Compatible con animaciones. [Demo 1](https://arthursonzogni.github.io/FTXUI/examples/?file=component/menu_underline_animated_gallery), [Demo 2](https://arthursonzogni.github.io/FTXUI/examples/?file=component/button_style)
 * Compatible con dibujo. [Demo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/canvas_animated)
 * Aprenda con la guía de [instalación](installation.html) y de [primeros pasos](getting-started.html).
 * Múltiples sistemas de compilación y paquetes: 
 * Buenas prácticas: documentación, pruebas, fuzzers, pruebas de rendimiento, CI automatizado, empaquetado automatizado, etc...

# Ejemplo

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

Salida esperada:

```
┌────┐┌────────────────────────────────────┐┌─────┐
│left││middle                              ││right│
└────┘└────────────────────────────────────┘└─────┘
```

# Plataformas soportadas

- Linux
- MacOS
- Windows
- WebAssembly

<div class="section_buttons">
 
|                                  Siguiente |
|--------------------------------------:|
| [Primeros pasos](getting-started.html) |
 
</div>

@defgroup screen ftxui/screen

Por favor, consulta el [tutorial](module-screen.html) del módulo `ftxui/screen`.

@defgroup dom ftxui/dom

Por favor, consulta el [tutorial](module-dom.html) del módulo `ftxui/dom`.

@defgroup component ftxui/component

Consulte el [tutorial](module-component.html) del módulo
`ftxui/component`.
