@page installation_debian Debian/Ubuntu

## Paquetes Debian y Ubuntu (No Oficiales)

Los paquetes pre-construidos son proporcionados por las distribuciones. Instale con:

```bash
sudo apt install libftxui-dev
```

Los siguientes paquetes están disponibles:
- `ftxui-doc`
- `ftxui-examples`
- `libftxui-component<version>`
- `libftxui-dev`
- `libftxui-dom<version>`
- `libftxui-screen<version>`

Una vez instalado, puede usarlo en sus proyectos CMake añadiendo lo siguiente a su `CMakeLists.txt`:

```cmake
find_package(ftxui REQUIRED)
add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

> [!note]
> Este es un paquete **no oficial**. Esto significa que no es mantenido por el equipo de FTXUI, sino por la comunidad. 

<div class="section_buttons">

| Anterior          |
|:------------------|
| [Empezando](getting-started.html) |

</div>