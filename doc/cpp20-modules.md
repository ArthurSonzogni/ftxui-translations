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

Use the `FTXUI_BUILD_MODULES` option to build the FTXUI project itself to provide C++20 modules,
for example with CMake and Ninja:

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

Writing `import ftxui;` is equivalent to including all `<ftxui/**/*.hpp>` headers, and provides
the entire library through the singular module.

Para encontrar y enlazar correctamente los módulos con CMake, usa `target_link_libraries` para obtener las banderas
de compilador, enlazador, etc. correctas.

```cmake
target_link_libraries(my_executable
    #...whatever...
    PRIVATE ftxui::modules
)
```

### Lista de módulos

While `import ftxui;` provides the entire library, FTXUI is designed in layers. If you only need specific functionalities, you can import the independent modules directly:

- `ftxui` (Convenience module that re-exports all of the below)
    - `ftxui.component` (Interactive components, events, and event loops)
    - `ftxui.dom` (Layout and styling via Elements)
    - `ftxui.screen` (Terminal rendering, pixels, and colors)
    - `ftxui.util` (Internal utilities)

For example:
```cpp
import ftxui.screen;
import ftxui.dom;

// Use only screen and dom functionalities...
```
