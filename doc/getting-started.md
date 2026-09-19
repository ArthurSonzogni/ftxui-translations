@page getting-started Démarrer
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595256.jpg)

FTXUI est une bibliothèque C++ fonctionnelle pour les interfaces utilisateur basées sur le terminal. Elle est organisée en trois modules principaux, chacun s'appuyant sur le précédent.

# Les Trois Modules

1.  **Screen** : Le niveau le plus bas. Il gère l'état du terminal, les couleurs et la grille de caractères brute.
2.  **DOM** : Le moteur de mise en page. Il fournit un ensemble d'`Element` qui peuvent être composés pour créer des mises en page complexes et réactives.
3.  **Component** : La couche interactive. Elle gère les entrées de l'utilisateur (clavier, souris) et gère la boucle principale de l'application.

# Installation et Dépendance

Pour configurer FTXUI dans votre projet, suivez le [guide d'installation](installation.html). 

La méthode la plus recommandée pour les utilisateurs de CMake est d'utiliser `FetchContent`. Ajoutez ceci à votre `CMakeLists.txt` :

```cmake
include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main # ou une version spécifique comme v7.0.3
)
FetchContent_MakeAvailable(ftxui)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE ftxui::ftxui)
```

# Votre Première Interface Statique (DOM)

Le module `DOM` vous permet de décrire votre interface utilisateur de manière déclarative. Composer des éléments est aussi simple qu'imbriquer des appels de fonction ou utiliser l'opérateur pipe pour les décorateurs.

Enregistrez ceci sous le nom de `main.cpp` :

```cpp
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

int main() {
  using namespace ftxui;

  // Définit la structure du document
  Element document = vbox({
    text("FTXUI Getting Started") | bold | center,
    separator(),
    hbox({
      text("Left Panel") | border,
      vbox({
        text("Main Content Area") | flex,
        separator(),
        text("Footer Information") | dim,
      }) | border | flex,
    }) | flex,
  });

  // Crée l'écran et effectue le rendu
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}
```

# Ajouter de l'Interactivité (Component)

Pour gérer les entrées de l'utilisateur et créer une application dynamique, utilisez le module `Component` et la classe `App`. Les composants gèrent leur propre état et peuvent être composés à l'aide de conteneurs.

```cpp
#include <ftxui/component/component.hpp>
#include <ftxui/component/app.hpp>
#include <ftxui/dom/elements.hpp>

int main() {
  using namespace ftxui;

  std::vector<std::string> entries = {
      "Entry 1",
      "Entry 2",
      "Entry 3",
  };
  int selected = 0;

  // Crée un composant menu
  auto menu = Menu(&entries, &selected);

  // Vous pouvez décorer les composants en utilisant l'opérateur pipe.
  auto component = menu | border;

  // Démarre la boucle principale
  auto app = App::TerminalOutput();
  app.Loop(component);

  return 0;
}
```

# Prochaines Étapes

*   Parcourez les [Exemples](https://arthursonzogni.github.io/FTXUI/examples/) pour voir ce qui est possible.
*   Consultez le [Modèle de Démarrage](https://github.com/ArthurSonzogni/ftxui-starter) pour une structure de projet préconfigurée.

<div class="section_buttons">
 
| Précédent                          | Suivant                    |
|:----------------------------------|------------------------:|
| [Introduction](index.html) | [Modules](modules.html) |

 
</div>
