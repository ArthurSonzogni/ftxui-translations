@page installation_cmake CMake

@tableofcontents

Esta página explica cómo depender de FTXUI usando [CMake](https://cmake.org).

# Métodos de Integración

## Usando FetchContent

Este enfoque descarga FTXUI en tiempo de configuración y no requiere una instalación en todo el sistema.

```cmake 
include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v7.0.3  # Replace with a version, tag, or commit hash
)

FetchContent_MakeAvailable(ftxui)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

Esto asegura compilaciones reproducibles y una fácil gestión de dependencias.

## Usando find_package

Si FTXUI está instalado en todo el sistema o mediante un gestor de paquetes (por ejemplo, vcpkg o Conan), puede usar:

```cmake 
find_package(ftxui REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

Asegúrese de que el paquete sea visible en su `CMAKE_PREFIX_PATH`.

## Usando git submodule

También puede agregar FTXUI como un [submódulo de Git](https://git-scm.com/book/en/v2/Git-Tools-Submodules), manteniéndolo como parte de su repositorio:

```cmake
git submodule add https://github.com/ArthurSonzogni/FTXUI external/ftxui
git submodule update --init --recursive
```

Al clonar un repositorio que ya incluye FTXUI como submódulo, asegúrese de obtener los submódulos con:

```
git clone --recurse-submodules <your-repo>
# Or, if already cloned:
git submodule update --init --recursive
```

Luego, en su `CMakeLists.txt`:

```cmake
add_subdirectory(external/ftxui)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

Este enfoque funciona bien si desea incluir FTXUI en su propio repositorio.

# Banderas CMake Opcionales

FTXUI admite las siguientes opciones de CMake:

| Option                            | Description                   | Default |
| --------------------------------- | ----------------------------- | ------- |
| FTXUI_BUILD_EXAMPLES              | Build bundled examples        | OFF     |
| FTXUI_BUILD_DOCS                  | Build the documentation       | OFF     |
| FTXUI_BUILD_TESTS                 | Enable tests                  | OFF     |
| FTXUI_ENABLE_INSTALL              | Generate install targets      | ON      |

Para habilitar una opción:

```
cmake -DFTXUI_BUILD_EXAMPLES=ON ..
```

# Verificando la Integración

Para confirmar que la configuración funciona, compile y ejecute un ejemplo mínimo.
Si necesita una plantilla completa, consulte: [ftxui-starter](https://github.com/ArthurSonzogni/ftxui-starter)

<div class="section_buttons">
 
| Anterior          |
|:------------------|
| [Primeros pasos](getting-started.html) |
 
</div>
