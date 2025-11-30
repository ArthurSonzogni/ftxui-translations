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

Usa la opción FTXUI_BUILD_MODULES para construir el propio proyecto FTXUI y proporcionar módulos C++20,
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

using ftxui::Button;
using ftxui::ScreenInteractive;

int main() {
  auto screen = ScreenInteractive::TerminalOutput();
  auto button = Button("Click me", screen.QuitClosure());
  screen.Loop(button);
  return 0;
}
```

Nota, el módulo de conveniencia `ftxui` que simplemente agrupa todos los módulos:

```cpp
export import ftxui.component;
export import ftxui.dom;
export import ftxui.screen;
export import ftxui.util;
```
Puedes importar solo el módulo o módulos que necesites si lo deseas.

Para encontrar y enlazar correctamente los módulos con CMake, usa `target_link_libraries` para obtener las banderas
de compilador, enlazador, etc. correctas.

```cmake
target_link_libraries(my_executable
    #...whatever...
    PRIVATE ftxui::modules
)
```

### Lista de módulos

Los módulos hacen referencia directa a la cabecera correspondiente, o a un grupo de
cabeceras relacionadas para proporcionar una interfaz más conveniente. Los siguientes módulos
están disponibles:

- `ftxui`
    - `ftxui.component`
    - `ftxui.dom`
    - `ftxui.screen`
    - `ftxui.util`