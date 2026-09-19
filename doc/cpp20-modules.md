@page cpp20-modules Modules C++20


> [!WARNING]
> Cette fonctionnalité est encore en développement, et l'API pourrait changer dans les futures versions.
> Votre contribution est nécessaire pour nous aider à améliorer la compatibilité et l'utilisabilité
> des modules C++ dans FTXUI. Si vous rencontrez des problèmes ou avez des suggestions,
> veuillez ouvrir une issue.

FTXUI prend en charge expérimentalement les
[modules C++20](https://en.cppreference.com/w/cpp/language/modules) pour réduire
les temps de compilation et améliorer l'organisation du code. Chaque partie de la bibliothèque a un
module correspondant, divisé en partitions par chaque en-tête.

Utilisez l'option `FTXUI_BUILD_MODULES` pour compiler le projet FTXUI lui-même afin de fournir des modules C++20,
par exemple avec CMake et Ninja :

```sh
cmake \
    -DCMAKE_GENERATOR=Ninja \
    -DFTXUI_BUILD_MODULES=ON \
    ..

ninja
```

> [!NOTE]
> Pour utiliser les modules, vous avez besoin d'un compilateur compatible C++≥20, de CMake version 3.20 ou
> supérieure, et d'utiliser un générateur compatible comme Ninja. Notez que les générateurs Makefile
> **ne supportent pas les modules**.

Ensuite, dans votre propre code, vous pouvez consommer les modules et le code comme d'habitude :

```cpp
import ftxui;

using ftxui::App;
using ftxui::Button;
using ftxui::Component;

int main() {
  App app = App::TerminalOutput();
  Component button = Button("Click me", app.ExitLoopClosure());
  app.Loop(button);
  return 0;
}
```

Écrire `import ftxui;` équivaut à inclure tous les en-têtes `<ftxui/**/*.hpp>`, et fournit
l'intégralité de la bibliothèque via ce module singulier.

Pour trouver et lier correctement les modules avec CMake, utilisez `target_link_libraries` pour obtenir les bons
flags de compilateur, linker, etc.

```cmake
target_link_libraries(my_executable
    #...whatever...
    PRIVATE ftxui::modules
)
```

### Liste des modules

Alors que `import ftxui;` fournit l'intégralité de la bibliothèque, FTXUI est conçu en couches. Si vous n'avez besoin que de fonctionnalités spécifiques, vous pouvez importer directement les modules indépendants :

- `ftxui` (Module de commodité qui réexporte tout ce qui suit)
    - `ftxui.component` (Composants interactifs, événements et boucles d'événements)
    - `ftxui.dom` (Mise en page et style via les Elements)
    - `ftxui.screen` (Rendu terminal, pixels et couleurs)
    - `ftxui.util` (Utilitaires internes)

Par exemple :
```cpp
import ftxui.screen;
import ftxui.dom;

// Use only screen and dom functionalities...
```
