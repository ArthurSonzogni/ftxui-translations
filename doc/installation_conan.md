@page installation_conan Conan
@tableofcontents

FTXUI se puede obtener e integrar fácilmente en tu proyecto usando el gestor de paquetes Conan.

## Prerrequisitos

Primero, asegúrate de que Conan esté instalado en tu sistema. Si no, puedes instalarlo usando pip:

```powershell
pip install conan
```
Conan suele funcionar junto con CMake, por lo que también necesitarás tener CMake instalado. Una vez que hayas confirmado que tanto Conan como CMake están instalados, crea un directorio de proyecto, por ejemplo, `ftxui-demo`:

```powershell
mkdir C:\ftxui-demo
cd C:\ftxui-demo
```

## Configuración

Después de asegurarte de que tu entorno está configurado correctamente, crea un archivo de configuración de Conan `conanfile.txt`. Este archivo se utiliza para declarar las dependencias de tu proyecto. El paquete de FTXUI mantenido por la comunidad se puede encontrar en [Conan Center](https://conan.io/center/recipes/ftxui).

> [!note]
> Este es un script de construcción no oficial. Esto significa que no es mantenido por el equipo de FTXUI
> sino por la comunidad. El mantenedor del paquete parece actualizarlo activamente a las últimas versiones. ¡Muchas gracias al mantenedor por su trabajo!

FTXUI también incluye su propio `conanfile.py` en la raíz del repositorio, probado
por CI en cada commit. No está publicado en el propio Conan Center, pero se
puede usar para consumir FTXUI directamente desde un checkout local mediante
`conan create .`, y podría servir como base para una receta oficial de Conan
Center.

@todo Si estás familiarizado con el proceso, por favor considera añadir un script de construcción "oficial" a Conan Center.
Esto podría ser una GitHub Action que actualice automáticamente Conan Center con cada nueva versión.

```ini
[requires]
ftxui/6.0.2

[generators]
CMakeDeps
CMakeToolchain

[layout]
cmake_layout
```

## Instalar Dependencias y Construir

Una vez configurado, ejecuta el siguiente comando para instalar FTXUI y sus dependencias:

```powershell
conan install . --output-folder=build --build=missing
```

Esto descargará e instalará `ftxui/6.0.2` junto con todas sus dependencias desde los repositorios remotos de Conan.

Una vez completada la instalación, puedes probarla creando un archivo `demo.cpp` en el directorio de tu proyecto:

```cpp
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>

int main() {
    using namespace ftxui;
    auto document = hbox({
        text(" Hello "),
        text("FTXUI ") | bold | color(Color::Red),
        text(" world! ")
    });
    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);
    std::cout << screen.ToString() << std::endl;
    return 0;
}
```

Si la prueba es exitosa, puedes crear un archivo `CMakeLists.txt` en el directorio del proyecto:

```cmake
cmake_minimum_required(VERSION 3.20)
project(ftxui-demo)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# Find the FTXUI package installed via Conan
find_package(ftxui CONFIG REQUIRED)

# Create the executable
add_executable(demo demo.cpp)

# Link the executable to the FTXUI library
target_link_libraries(demo PRIVATE ftxui::component)
```

@todo 中国大陆在这方面的下载可能会受限制，需要一个替代的方案

---

<div class="section_buttons">

| Anterior          |
|:------------------|
| [Primeros pasos](getting-started.html) |

</div>