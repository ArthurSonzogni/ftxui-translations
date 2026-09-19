@page getting-started Primeros Pasos
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595256.jpg)

FTXUI es una biblioteca funcional de C++ para interfaces de usuario basadas en terminal. Está organizada en tres módulos principales, cada uno construyéndose sobre el anterior.

# Los Tres Módulos

1.  **Screen**: El nivel más bajo. Gestiona el estado del terminal, los colores y la cuadrícula de caracteres en bruto.
2.  **DOM**: El motor de diseño. Proporciona un conjunto de `Element`s que se pueden componer para crear diseños complejos y responsivos.
3.  **Component**: La capa interactiva. Gestiona la entrada del usuario (teclado, ratón) y administra el bucle principal de la aplicación.

# Instalación y Dependencia

Para configurar FTXUI en tu proyecto, sigue la [guía de instalación](installation.html). 

La forma más recomendada para usuarios de CMake es usar `FetchContent`. Agrega esto a tu `CMakeLists.txt`:

```cmake
include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main # or a specific version like v7.0.3
)
FetchContent_MakeAvailable(ftxui)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE ftxui::ftxui)
```

# Tu Primera UI Estática (DOM)

El módulo `DOM` te permite describir tu UI de manera declarativa. Componer elementos es tan simple como anidar llamadas a funciones o usar el operador pipe para los decoradores.

Guarda esto como `main.cpp`:

```cpp
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

int main() {
  using namespace ftxui;

  // Define the document structure
  Element document = vbox({
    text("FTXUI Getting Started") | bold | center,
    separator(),
    hbox({
      text("Left Panel") | border,
      vbox({
        text("Main Content Area") | flex,
        separator(),
        text("Footer Information") | dim,
      }) | border | flex,
    }) | flex,
  });

  // Create the screen and render
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}
```

# Agregando Interactividad (Component)

Para gestionar la entrada del usuario y crear una aplicación dinámica, usa el módulo `Component` y la clase `App`. Los componentes gestionan su propio estado y pueden componerse usando contenedores.

```cpp
#include <ftxui/component/component.hpp>
#include <ftxui/component/app.hpp>
#include <ftxui/dom/elements.hpp>

int main() {
  using namespace ftxui;

  std::vector<std::string> entries = {
      "Entry 1",
      "Entry 2",
      "Entry 3",
  };
  int selected = 0;

  // Create a menu component
  auto menu = Menu(&entries, &selected);

  // You can decorate components using the pipe operator.
  auto component = menu | border;

  // Start the main loop
  auto app = App::TerminalOutput();
  app.Loop(component);

  return 0;
}
```

# Próximos Pasos

*   Explora los [Ejemplos](https://arthursonzogni.github.io/FTXUI/examples/) para ver lo que es posible.
*   Consulta la [Plantilla de Inicio](https://github.com/ArthurSonzogni/ftxui-starter) para una estructura de proyecto preconfigurada.

<div class="section_buttons">
 
| Anterior                          | Siguiente                    |
|:----------------------------------|------------------------:|
| [Introducción](index.html) | [Módulos](modules.html) |

 
</div>
