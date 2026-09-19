@page cpp20-modules Módulos C++20


> [!WARNING]
> Esta característica aún está en desarrollo, y la API puede cambiar en futuras versiones.
> Se necesita tu contribución para ayudarnos a mejorar la compatibilidad y usabilidad
> de los módulos C++ en FTXUI. Si encuentras algún problema o tienes sugerencias,
> por favor, abre un issue.

FTXUI soporta experimentalmente
[módulos C++20](https://en.cppreference.com/w/cpp/language/modules) para reducir
los tiempos de compilación y mejorar la organización del código. Cada parte de la biblioteca tiene un
módulo correspondiente, dividido en particiones por cada cabecera.

Usa la opción `FTXUI_BUILD_MODULES` para compilar el propio proyecto FTXUI y proporcionar módulos C++20,
por ejemplo con CMake y Ninja:

```sh
cmake \
    -DCMAKE_GENERATOR=Ninja \
    -DFTXUI_BUILD_MODULES=ON \
    ..

ninja
```

> [!NOTE]
> Para usar módulos, necesitas un compilador compatible con C++≥20, CMake versión 3.20 o
> superior, y usar un generador compatible como Ninja. Ten en cuenta que los generadores Makefile
> **no soportan módulos**.

Luego, en tu propio código puedes consumir los módulos y el código de forma normal:

```cpp
import ftxui;

using ftxui::App;
using ftxui::Button;
using ftxui::Component;

int main() {
  App app = App::TerminalOutput();
  Component button = Button("Click me", app.ExitLoopClosure());
  app.Loop(button);
  return 0;
}
```

Escribir `import ftxui;` es equivalente a incluir todos los headers `<ftxui/**/*.hpp>`, y proporciona
toda la biblioteca a través del módulo singular.

Para encontrar y enlazar correctamente los módulos con CMake, usa `target_link_libraries` para obtener las banderas
de compilador, enlazador, etc. correctas.

```cmake
target_link_libraries(my_executable
    #...whatever...
    PRIVATE ftxui::modules
)
```

### Lista de módulos

Aunque `import ftxui;` proporciona toda la biblioteca, FTXUI está diseñado en capas. Si solo necesitas funcionalidades específicas, puedes importar los módulos independientes directamente:

- `ftxui` (Módulo de conveniencia que re-exporta todo lo anterior)
    - `ftxui.component` (Componentes interactivos, eventos y bucles de eventos)
    - `ftxui.dom` (Diseño y estilo mediante Elements)
    - `ftxui.screen` (Renderizado de terminal, píxeles y colores)
    - `ftxui.util` (Utilidades internas)

Por ejemplo:
```cpp
import ftxui.screen;
import ftxui.dom;

// Use only screen and dom functionalities...
```
