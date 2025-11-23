@page installation_cmake CMake

@tableofcontents

Cette page explique comment dépendre de FTXUI en utilisant [CMake](https://cmake.org).

# Méthodes d'intégration

## Utilisation de FetchContent

Cette approche télécharge FTXUI au moment de la configuration et ne nécessite pas d'installation à l'échelle du système.

```cmake
include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9  # Replace with a version, tag, or commit hash
)

FetchContent_MakeAvailable(ftxui)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

Cela garantit des builds reproductibles et une gestion facile des dépendances.

## Utilisation de find_package

Si FTXUI est installé à l'échelle du système ou via un gestionnaire de paquets (par exemple vcpkg ou Conan), vous pouvez utiliser :

```cmake
find_package(ftxui REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

Assurez-vous que le paquet est visible dans votre `CMAKE_PREFIX_PATH`.

## Utilisation de git submodule

Vous pouvez également ajouter FTXUI en tant que [sous-module Git](https://git-scm.com/book/en/v2/Git-Tools-Submodules), en le conservant comme faisant partie de votre dépôt :

```cmake
git submodule add https://github.com/ArthurSonzogni/FTXUI external/ftxui
git submodule update --init --recursive
```

Lors du clonage d'un dépôt qui inclut déjà FTXUI en tant que sous-module, assurez-vous de récupérer les sous-modules avec :

```
git clone --recurse-submodules <your-repo>
# Or, if already cloned:
git submodule update --init --recursive
```

Ensuite, dans votre `CMakeLists.txt` :

```cmake
add_subdirectory(external/ftxui)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

Cette approche fonctionne bien si vous souhaitez intégrer FTXUI dans votre propre dépôt.

# Drapeaux CMake optionnels

FTXUI prend en charge les options CMake suivantes :

| Option                            | Description                   | Default |
| --------------------------------- | ----------------------------- | ------- |
| FTXUI_BUILD_EXAMPLES              | Construire les exemples inclus | OFF     |
| FTXUI_BUILD_DOCS                  | Construire la documentation   | OFF     |
| FTXUI_BUILD_TESTS                 | Activer les tests             | OFF     |
| FTXUI_ENABLE_INSTALL              | Générer les cibles d'installation | ON      |
| FTXUI_MICROSOFT_TERMINAL_FALLBACK | Améliorer la compatibilité Windows | ON/OFF  |

Pour activer une option :

```
cmake -DFTXUI_BUILD_EXAMPLES=ON ..
```

# Vérification de l'intégration

Pour confirmer que la configuration fonctionne, construisez et exécutez un exemple minimal.
Si vous avez besoin d'un modèle complet, consultez : [ftxui-starter](https://github.com/ArthurSonzogni/ftxui-starter)

<div class="section_buttons">
 
| Précédent          |
|:------------------|
| [Démarrage rapide](getting-started.html) |
 
</div>