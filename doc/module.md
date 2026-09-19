# ftxui {#ftxui}

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595251.jpg)

FTXUI está organizado en tres módulos, cada uno construyendo sobre el anterior:

1. [ftxui/screen](#module-screen) - Renderizado de bajo nivel
2. [ftxui/dom](#module-dom) - Diseño y composición
3. [ftxui/component](#module-component) - Interacción del usuario

Alternatively, you can use the **`ftxui`** umbrella module which includes everything:

- **Header**: `#include <ftxui/ftxui.hpp>`
- **CMake target**: `ftxui::ftxui`
- **Bazel target**: `@ftxui//:ftxui`

---

# ftxui/screen

Define:

- **`ftxui::Screen`**: a 2D grid of styled characters.
- **`ftxui::Cell`**: the unit of rendering.
- Helpers like `ftxui::Color` and `Dimension`.

Úsalo para el dibujo directo en el terminal y el estilo.

<div class="section_buttons">
 
|                                  Siguiente |
|--------------------------------------:|
| [Documentación](module-screen.html) |
 
</div>


---

# ftxui/dom

Proporciona:

- **`ftxui::Element`**: una estructura de árbol para el diseño y la interfaz de usuario.
- Elementos componibles y responsivos.
- `Render()` para dibujar en una `Screen`.

Ideal para interfaces de usuario estructuradas y con estilo.

<div class="section_buttons">

|                                  Siguiente |
|--------------------------------------:|
| [Documentación](module-dom.html) |

</div>


---
# ftxui/component

Añade:

- **`ftxui::Component`**: widgets interactivos con estado.
- Incorporados: `Checkbox`, `Input`, `Menu`, `Button`.
- Soporta entrada de teclado/cursor y composición.

Úsalo para aplicaciones interactivas.

<div class="section_buttons">

|                                  Siguiente |
|--------------------------------------:|
| [Documentación](module-component.html) |

</div>

---

Modules can be used independently, or together: `screen → dom → component`.
