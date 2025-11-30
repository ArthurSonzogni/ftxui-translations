@page installation_manual Manual
@tableofcontents

## Construcción desde el código fuente (oficial)

Clona y construye el proyecto usando CMake:

```bash
git clone https://github.com/ArthurSonzogni/FTXUI.git
cd FTXUI
cmake -S . -B build -DFTXUI_ENABLE_INSTALL=ON -D
cmake --build build -j
sudo cmake --install build
```

Una vez instalado, puedes usarlo en tus proyectos CMake añadiendo lo siguiente a tu `CMakeLists.txt`:

```cmake
find_package(ftxui REQUIRED)
add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```


<div class="section_buttons">

| Anterior          |
|:------------------|
| [Primeros pasos](getting-started.html) |

</div>