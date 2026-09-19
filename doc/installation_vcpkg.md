@page installation_vcpkg Vcpkg
@tableofcontents

# Paquete Vcpkg

FTXUI está disponible en el [registro de Vcpkg](https://vcpkg.link/ports/ftxui)

Para usarlo, puedes añadir lo siguiente a tu `vcpkg.json`:

```json
{
  "name": "your-project",
  "version-string": "0.1.0",
  "dependencies": [
    {
        "name": "ftxui",
        "version>=": "7.0.3"
    }
  ]
}
```

# Instalar FTXUI usando Vcpkg
```bash
vcpkg install --triplet x64-linux  # or x64-windows / arm64-osx etc.
```

# Configura tu sistema de compilación.
Si estás usando CMake, puedes usar lo siguiente en tu `CMakeLists.txt`:

**CMakeLists.txt**
```cmake
cmake_minimum_required(VERSION 3.15)
project(my_project)

# Make sure vcpkg toolchain file is passed at configure time
find_package(ftxui CONFIG REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)
```

**main.cpp**
```cpp
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>

int main() {
  using namespace ftxui;

  auto screen = App::TerminalOutput();
  auto button = Button("Click me", [] { std::cout << "Clicked!\n"; });

  screen.Loop(button);
}
```

**Configurar y construir el proyecto**
```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
./build/main
```




<div class="section_buttons">
 
| Anterior          |
|:------------------|
| [Primeros pasos](getting-started.html) |
 
</div>
