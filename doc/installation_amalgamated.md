@page installation_amalgamated Amalgamated
@tableofcontents

## Version amalgamée

La version amalgamée est une version en un seul en-tête (`ftxui.hpp`) et une
seule source (`ftxui.cpp`) de la bibliothèque. C'est une autre façon
d'intégrer FTXUI dans votre projet, particulièrement si vous ne voulez pas
utiliser de gestionnaire de paquets ou un système de build complexe.

Une version vraiment mono-fichier et header-only (`ftxui_all.hpp`) est aussi
disponible.

### Comment l'obtenir ?

Vous pouvez obtenir les fichiers amalgamés de plusieurs façons :

1. **GitHub Releases (Recommandé)** : Téléchargez le `ftxui-amalgamated.zip`
   depuis la [dernière release](https://github.com/ArthurSonzogni/FTXUI/releases).
   *Remarque : Ceci ne sera officiellement disponible qu'à partir de la version
   7.0.0. Pour les versions plus anciennes ou avant la sortie de la 7.0.0,
   veuillez utiliser les artefacts de la CI ci-dessous.*
   Il contient `ftxui.hpp`, `ftxui.cpp`, et le header-only `ftxui_all.hpp`.
2. **Artefacts de la CI** : Si vous voulez l'absolue dernière version de
   `main` (ou avant la sortie officielle de la 7.0.0), vous pouvez télécharger
   les artefacts depuis le
   [workflow Amalgamate](https://github.com/ArthurSonzogni/FTXUI/actions/workflows/amalgamate.yaml).
3. **Génération manuelle** : Exécutez la commande suivante depuis la racine du
   dépôt cloné :
   ```bash
   ./tools/amalgamate.py
   ```

### Comment l'utiliser ?

#### Option 1 : En-tête/Source séparés (Recommandé pour la vitesse de build)

Copiez `ftxui.hpp` et `ftxui.cpp` dans votre projet. Incluez `ftxui.cpp` dans
votre liste de sources.

#### Option 2 : Header-Only

Copiez `ftxui_all.hpp` dans votre projet. Dans **exactement un** fichier C++,
définissez `FTXUI_IMPLEMENTATION` avant d'inclure l'en-tête :

```cpp
#define FTXUI_IMPLEMENTATION
#include "ftxui_all.hpp"
```

Dans les autres fichiers, vous pouvez simplement l'inclure sans la
définition :

```cpp
#include "ftxui_all.hpp"
```

### Compilation

Lors de la compilation de votre projet, assurez-vous d'utiliser C++17 et de
lier les threads.

```bash
g++ -std=c++17 your_main.cpp [ftxui.cpp] -o your_app -lpthread
```

<div class="section_buttons">

| Précédent          |
|:------------------|
| [Installation](installation.html) |

</div>
