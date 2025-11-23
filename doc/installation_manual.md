@page installation_manual Manuel
@tableofcontents

## Compilation depuis les sources (Officiel)

Cloner et compiler le projet en utilisant CMake:

```bash
git clone https://github.com/ArthurSonzogni/FTXUI.git
cd FTXUI
cmake -S . -B build -DFTXUI_ENABLE_INSTALL=ON -D
cmake --build build -j
sudo cmake --install build
```

Une fois installé, vous pouvez l'utiliser dans vos projets CMake en ajoutant ce qui suit à votre `CMakeLists.txt`:

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

| Précédent         |
|:------------------|
| [Démarrage rapide](getting-started.html) |

</div>
