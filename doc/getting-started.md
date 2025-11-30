@page getting-started Primeros Pasos
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595256.jpg)

# Instalar FTXUI

Para configurar FTXUI en tu proyecto, sigue la [guía de instalación](installation.html), que proporciona instrucciones para múltiples sistemas de compilación y gestores de paquetes.

# Ejemplo Mínimo

Guarda el siguiente código como `main.cpp`:

```cpp
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

int main() {
  using namespace ftxui;

  Element document = hbox({
    text("left")   | border,
    text("middle") | border | flex,
    text("right")  | border,
  });

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();
}
```

Compílalo y ejecútalo usando tu sistema de compilación preferido.  
Si no estás seguro, comienza con uno de los métodos descritos en la [página de instalación](installation.html).

Salida esperada:

```
┌────┐┌────────────────────────────────────┐┌─────┐
│left││middle                              ││right│
└────┘└────────────────────────────────────┘└─────┘
```

# Plantilla de Inicio

Para un proyecto completo y funcional, clona el repositorio oficial de inicio:

```bash
git clone https://github.com/ArthurSonzogni/ftxui-starter
```

Sigue las instrucciones de compilación en el `README.md` de ese repositorio.

<div class="section_buttons">
 
| Anterior                          | Siguiente                    |
|:----------------------------------|------------------------:|
| [Introducción](index.html) | [Módulos](modules.html) |

 
</div>