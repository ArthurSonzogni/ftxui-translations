@page installation_debian Debian/Ubuntu

## Paquets Debian et Ubuntu (Non officiels)

Des paquets pré-construits sont fournis par les distributions. Installez avec :

```bash
sudo apt install libftxui-dev
```

Les paquets suivants sont disponibles :
- `ftxui-doc`
- `ftxui-examples`
- `libftxui-component<version>`
- `libftxui-dev`
- `libftxui-dom<version>`
- `libftxui-screen<version>`

Une fois installé, vous pouvez l'utiliser dans vos projets CMake en ajoutant ce qui suit à
votre `CMakeLists.txt` :

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
> Ceci est un paquet **non officiel**. Cela signifie qu'il n'est pas maintenu par l'équipe FTXUI, mais par la communauté.

<div class="section_buttons">

| Précédent          |
|:------------------|
| [Démarrer](getting-started.html) |

</div>