@page installation_arch Arch Linux

FTXUI est packagé sur l'AUR. Installez-le en utilisant un assistant AUR :

```bash
yay -S ftxui
```

Vous pouvez également télécharger manuellement le PKGBUILD depuis <https://aur.archlinux.org/packages/ftxui>.

Une fois installé, vous pouvez l'utiliser dans vos projets CMake en ajoutant ce qui suit à votre `CMakeLists.txt` :

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
> Ceci est un package non officiel. Cela signifie qu'il n'est pas maintenu par l'équipe FTXUI, mais par la communauté. Les mainteneurs du package semblent le mettre à jour activement vers la dernière version. Merci aux mainteneurs pour leur travail !

<div class="section_buttons">

| Précédent          |
|:------------------|
| [Démarrer](getting-started.html) |

</div>