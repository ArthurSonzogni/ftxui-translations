@page getting-started Démarrer
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595256.jpg)

# Installer FTXUI

Pour configurer FTXUI dans votre projet, suivez le [guide d'installation](installation.html), qui fournit des instructions pour plusieurs systèmes de construction et gestionnaires de paquets.

# Exemple Minimal

Enregistrez le code suivant sous le nom de `main.cpp` :

```cpp
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

int main() {
  using namespace ftxui;

  Element document = hbox({
    text("left")   | border,
    text("middle") | border | flex,
    text("right")  | border,
  });

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();
}
```

Compilez et exécutez-le en utilisant votre système de construction préféré.  
En cas de doute, commencez par l'une des méthodes décrites sur la [page d'installation](installation.html).

Sortie attendue :

```
┌────┐┌────────────────────────────────────┐┌─────┐
│left││middle                              ││right│
└────┘└────────────────────────────────────┘└─────┘
```

# Modèle de Démarrage

Pour un projet complet et fonctionnel, clonez le dépôt de démarrage officiel :

```bash
git clone https://github.com/ArthurSonzogni/ftxui-starter
```

Suivez les instructions de compilation dans le fichier `README.md` de ce dépôt.

<div class="section_buttons">
 
| Précédent                          | Suivant                    |
|:----------------------------------|------------------------:|
| [Introduction](index.html) | [Modules](modules.html) |

 
</div>