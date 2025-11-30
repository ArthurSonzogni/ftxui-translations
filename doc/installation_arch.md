@page installation_arch Arch Linux

FTXUI está empaquetado en el AUR. Instálalo usando un asistente de AUR:

```bash
yay -S ftxui
```

También puedes descargar manualmente el PKGBUILD desde <https://aur.archlinux.org/packages/ftxui>.

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

> [!note]
> Este es un paquete no oficial. Esto significa que no es mantenido por el equipo de FTXUI, sino por la comunidad. Los mantenedores del paquete parecen actualizar activamente el paquete a la última versión. ¡Gracias a los mantenedores por su trabajo!

<div class="section_buttons">

| Anterior          |
|:------------------|
| [Primeros Pasos](getting-started.html) |

</div>