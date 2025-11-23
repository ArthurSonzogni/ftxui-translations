Journal des modifications
=======================

Prochaine
========

### Documentation
- Correction de la sortie Doxygen cassée. Voir @markmandel dans #1029.
- Utilisation de Doxygen awesome. Ajout de notre propre thème.
- Division de la documentation en plusieurs pages.

### Compilation
- Fonctionnalité : Prise en charge des modules C++20. 
  Ceci nécessite :
  - L'utilisation du générateur Ninja ou MSVC
  - Un compilateur Clang/GCC/MSVC récent.
  - Cmake 3.28 ou plus.
  Utilisation :
  ```cpp
  import ftxui;
  import ftxui.component;
  import ftxui.dom;
  import ftxui.screen;
  import ftxui.util;
  ```
  Merci @mikomikotaishi pour la PR #1015.
- Suppression de la dépendance à 'pthread'.

### Composant
- Fonctionnalité : Gestion de l'entrée POSIX par tube.
  - Permet aux applications FTXUI de lire des données depuis stdin (lorsqu'elles sont pipées) tout en recevant des entrées clavier du terminal.
  - Activé par défaut.
  - Peut être désactivé en utilisant `ScreenInteractive::HandlePipedInput(false)`.
  - Uniquement disponible sur Linux et macOS.
  Merci @HarryPehkonen pour la PR #1094.
- Correction : `ScreenInteractive::FixedSize` écrase la sortie du terminal précédente.
  Merci @zozowell dans #1064.
- Correction du `ftxui::Slider` vertical. La touche "haut" diminuait auparavant la
  valeur. Merci @its-pablo dans #1093 pour avoir signalé le problème.

### Dom
- Correction d'un débordement d'entier dans `ComputeShrinkHard`. Merci @its-pablo dans #1137 pour
  avoir signalé et corrigé le problème.
- Ajout d'une spécialisation pour `vbox/hbox/dbox` afin de permettre un conteneur d'éléments comme
  entrée. Merci @nbusser dans #1117.

6.1.9 (2025-05-07)
------------

### Compilation
Si tout se passe bien (en attente), ftxui devrait apparaître dans le dépôt central Bazel.
Il peut être importé dans votre projet en utilisant les lignes suivantes :

**MODULE.bazel**
```bazel
bazel_dep(name = "ftxui", version = "6.1.9")
```

Merci @robinlinden et @kcc pour les revues.

### dom
- Correction de bug : Restauration du comportement de `dbox` de ftxui 5.0.0. Pour appliquer le mélange de couleurs d'arrière-plan entre les deux couches, un nouveau `dboxBlend` sera ajouté.

6.1.8 (2025-05-01)
------------------

### Compilation
- Fonctionnalité : Prise en charge du système de construction `bazel`. Voir #1032.
  Proposé par Kostya Serebryany @kcc

  **BUILD.bazel**
  ```bazel
  deps = [
    // Depend on the whole library:
    "@ftxui//:ftxui",

    // Choose a specific submodule:
    "@ftxui//:component",
    "@ftxui//:dom",
    "@ftxui//:screen",
  ]
  ```

### Composant
- Correction de bug : Correction d'un crash avec ResizeableSplit. Voir #1023.
  - Limite la taille de l'écran à la taille du terminal.
  - Interdit `ResizeableSplit` avec une taille négative.

### Dom
- Correction de bug : Interdit de spécifier une contrainte de taille négative. Voir #1023.


6.0.2 (2025-03-30)
-----

### Composant
- Correction de bug : Correction d'un crash majeur sur Windows affectant tous les composants. Voir #1020
- Correction de bug : Correction de focusRelative.

6.0.1 (2025-03-28)
-----

Identique à la v6.0.0.

En raison d'un problème, la balise v6.0.0 a été remplacée. Ce n'est pas une bonne pratique et cela affecte
les développeurs qui ont commencé à l'utiliser dans un court laps de temps. Soumettre une nouvelle
version avec le même contenu est la meilleure façon de corriger cela.

Voir #1017 et #1019.

6.0.0 (2025-03-23)
-----

### Composant
- Fonctionnalité : Ajout de la prise en charge de l'entrée brute. Permettant de détecter plus de touches.
- Fonctionnalité : Ajout de `ScreenInteractive::ForceHandleCtrlC(false)` pour permettre au composant
  de remplacer entièrement le gestionnaire `Ctrl+C` par défaut.
- Fonctionnalité : Ajout de `ScreenInteractive::ForceHandleCtrlZ(false)` pour permettre au composant
  de remplacer entièrement le gestionnaire `Ctrl+Z` par défaut.
- Fonctionnalité : Ajout des événements `Mouse::WeelLeft` et `Mouse::WeelRight` sur les terminaux
  pris en charge.
- Fonctionnalité : Ajout de `Event::DebugString()`.
- Fonctionnalité : Ajout de la prise en charge du mode insertion de `Input`. Ajout de l'option `InputOption::insert`.
  Ajouté par @mingsheng13.
- Fonctionnalité : Ajout de `DropdownOption` pour configurer le menu déroulant. Voir #826.
- Fonctionnalité : Ajout de la prise en charge de la sélection. Merci @clement-roblot. Voir #926.
  - Voir `ScreenInteractive::GetSelection()`.
  - Voir l'écouteur `ScreenInteractive::SelectionChange(...)`.
- Correction de bug/Changement de rupture : `Transition de la souris` :
  - Détecte quand la souris bouge, par opposition à être pressée.
    Le mouvement `Mouse::Moved` a été ajouté.
  - Faire glisser la souris avec le bouton gauche enfoncé évite maintenant d'activer
    plusieurs cases à cocher.
  - Quelques composants sont maintenant activés lorsque la souris est pressée,
  par opposition à être relâchée.
  Ceci corrige : https://github.com/ArthurSonzogni/FTXUI/issues/773
  Ceci corrige : https://github.com/ArthurSonzogni/FTXUI/issues/792
- Correction de bug : `mouse.control` est maintenant signalé correctement.
- Fonctionnalité : Ajout de `ScreenInteractive::FullscreenPrimaryScreen()`. Cela permet
  d'afficher un composant en plein écran sur l'écran principal, par opposition à l'
  écran alternatif.
- Correction de bug : `Input` `onchange` n'était pas appelé sur la touche de retour arrière ou de suppression.
  Corrigé par @chrysante dans chrysante dans la PR #776.
- Correction de bug : Restaure correctement la forme du curseur à la sortie. Voir #792.
- Correction de bug : Correction de la position du curseur lorsqu'il est dans la dernière colonne. Voir #831.
- Correction de bug : Correction de la navigation clavier de `ResizeableSplit`. Corrigé par #842.
- Correction de bug : Correction du focus de `Menu`. Voir #841
- Fonctionnalité : Ajout de `ComponentBase::Index()`. Cela permet d'obtenir l'index d'un
  composant dans son parent. Voir #932
- Fonctionnalité : Ajout de `EntryState::index`. Cela permet d'obtenir l'index d'une entrée de menu.
  Voir #932
- Fonctionnalité : Ajout de `SliderOption::on_change`. Cela permet de définir un rappel lorsque la
  valeur du curseur change. Voir #938.
- Correction de bug : Gère `Dropdown` sans entrées.
- Correction de bug : Correction d'un crash dans `LinearGradient` dû à la précision des flottants et à une erreur
          d'un pas. Voir #998.

### Dom
- Fonctionnalité : Ajout du décorateur `italic`. Par exemple :
  ```cpp
  auto italic_text = text("Italic text") | italic;
  ```
  ```cpp
  auto italic_text = italic(text("Italic text"));
  ```
  Proposé par @kenReneris dans #1009.
- Fonctionnalité : Ajout de `hscroll_indicator`. Il affiche un indicateur horizontal
  reflétant la position de défilement actuelle. Proposé par @ibrahimnasson dans
  [issue 752](https://github.com/ArthurSonzogni/FTXUI/issues/752)
- Fonctionnalité : Ajout de l'option `extend_beyond_screen` à `Dimension::Fit(..)`, permettant
  à l'élément d'être plus grand que l'écran. Proposé par @LordWhiro. Voir #572 et
  #949.
- Fonctionnalité : Ajout du support pour la sélection. Merci @clement-roblot. Voir #926.
  - Voir le décorateur `selectionColor`.
  - Voir le décorateur `selectionBackgroundColor`.
  - Voir le décorateur `selectionForegroundColor`.
  - Voir le décorateur `selectionStyle(style)`.
  - Voir le décorateur `selectionStyleReset`.
- Changement de rupture : Changement de la façon dont "focus"/"select" sont gérés. Cela corrige le
  comportement.
- Changement de rupture : `Component::OnRender()` devient la méthode à surcharger pour
  rendre un composant. Cela remplace `Component::Render()` qui est toujours utilisé
  pour appeler la méthode de rendu sur les enfants. Ce changement permet de corriger quelques
  problèmes liés à la gestion du focus.

### Écran
- Fonctionnalité : Ajout de `Box::IsEmpty()`.
- Fonctionnalité : Transparence des couleurs
    - Ajout de `Color::RGBA(r,g,b,a)`.
    - Ajout de `Color::HSVA(r,g,b,a)`.
    - Ajout de `Color::Blend(Color)`.
    - Ajout de `Color::IsOpaque()`

### Utilitaire
- Fonctionnalité : Prise en charge d'`Adapter` arbitraire pour `ConstStringListRef`. Voir #843.

### Compilation
- Prise en charge des constructions "unity/jumbo" de cmake. Corrigé par @ClausKlein.

5.0.0
-----

### Composant
- Rupture : L'énumération `MenuDirection` est renommée `Direction`.
- Rupture : L'énumération `GaugeDirection` est renommée `Direction`.
- Rupture : L'énumération `Direction` est renommée `WidthOrHeight`.
- Rupture : Suppression du constructeur/opérateur d'affectation de copie de `ComponentBase`.
- Rupture : `MenuOption::entries` est renommée `MenuOption::entries_option`.
- Rupture : `Ref<{Component}Option>` devient `{Component}Option` dans les constructeurs de composants.
- Fonctionnalité : `ResizeableSplit` prend désormais en charge un élément arbitraire comme séparateur.
- Fonctionnalité : `input` prend désormais en charge plusieurs lignes.
- Fonctionnalité : Le style de `input` est désormais personnalisable.
- Correction de bug : Prise en charge de F1-F5 depuis le terminal du système d'exploitation.
- Fonctionnalité : Ajout d'un constructeur basé sur une structure :
  ```cpp
  Component Button(ButtonOption options);
  Component Checkbox(CheckboxOption options);
  Component Input(InputOption options);
  Component Menu(MenuOption options);
  Component MenuEntry(MenuEntryOption options);
  Component Radiobox(RadioboxOption options);
  Component Slider(SliderOption<T> options);
  Component ResizableSplit(ResizableSplitOption options);
  ```
- Fonctionnalité : Ajout de `ScreenInteractive::TrackMouse(false)` pour désactiver le support de la souris.

### Dom
- Fonctionnalité : Ajout du décorateur `hyperlink`. Par exemple :
  ```cpp
  auto link = text("Click here") | hyperlink("https://github.com/FTXUI")
  ```
  Voir la [page OSC 8](https://gist.github.com/egmontkob/eb114294efbcd5adb1944c9f3cb5feda).
  Le support de FTXUI a été proposé par @aaleino dans [#662](https://github.com/ArthurSonzogni/FTXUI/issues/662).

### Écran
- Rupture : `WordBreakProperty` devient une énumération `uint8_t`. Cela entraîne une amélioration de 0,8 %
  des performances.
- Rupture : Suppression du constructeur de Pixel défini par l'utilisateur et de l'opérateur d'égalité.
- Performance : 19 % plus rapide sur les benchmarks.


### Compilation
- Vérifier la compatibilité des versions lors de l'utilisation de `cmake find_package()`.
- Ajouter l'option `FTXUI_DEV_WARNING` pour activer les avertissements lors de la construction de FTXUI.
- Désactiver par défaut `FTXUI_BUILD_DOCS`.
- Désactiver par défaut `FTXUI_BUILD_EXAMPLE`.

4.1.1
-----

### Composant
- Correction : Prise en charge des touches fléchées en mode application.
- Correction : Suppression d'une nouvelle ligne inutile lors de l'utilisation d'un écran alternatif.

### Dom
- Fonctionnalité : Ajout du style "pointillé" pour les bordures et les séparateurs :
  - Voir l'énumération `DASHED`, et les fonctions `separatorDashed()`, `borderDashed()`.
- Fonctionnalité : Ajout de bordures colorées.
  - Voir les fonctions : `borderStyled(BorderStyle, Color)` et `borderStyled(Color)`.
- Fonctionnalité : Ajout de `LinearGradient`. Il peut être utilisé dans `color` et `bgColor`.
- Amélioration : `Color::Interpolate()` utilise la correction gamma.
- Correction : Vérifier que la zone `graph` est positive.

### Compilation/Installation
- Utiliser la norme `CMAKE_CXX_STANDARD` définie globalement si elle est définie.
- Exposer le fichier `pkg-config`.
- Vérifier la compatibilité des versions lors de l'utilisation de `cmake find_package()`.

4.1.0 (Abandonné)
-----
Cette version est abandonnée et ne doit pas être utilisée. Elle a introduit un changement de rupture dans l'API.

4.0.0
-----

### DOM
- Fonctionnalité : Plus de styles :
  - `strikethrough`
  - `underlinedDouble`
- Fonctionnalité : Personnaliser le curseur. Ajouter les décorateurs suivants :
  - `focusCursorBlock`
  - `focusCursorBlockBlinking`
  - `focusCursorBar`
  - `focusCursorBarBlinking`
  - `focusCursorUnderline`
  - `focusCursorUnderlineBlinking`
- Correction de bug : Correction de `focus`/`select` lorsque le `vbox`/`hbox`/`dbox` contient un
  `flexbox`.
- Correction de bug : Correction de la zone sélectionnée/focalisée. Elle était auparavant 1 cellule plus grande/plus longue que
  demandé.
- Correction de bug : Transmettre la zone sélectionnée/focalisée de l'enfant dans la `gridbox`.
- Correction de bug : Correction des dimensions calculées incorrectes du `Canvas`.
- Correction de bug : Prise en charge de `vscroll_indicator` avec une taille intérieure nulle.
- Correction de bug : Correction de `vscroll_indicator` cachant la dernière colonne.

### Composant :
- Fonctionnalité : Ajout du composant `Modal`.
- Fonctionnalité : `Slider` prend en charge les références pour tous ses arguments.
- Fonctionnalité : `Slider` prend en charge `SliderOption`. Il prend en charge :
    - plusieurs directions.
    - plusieurs couleurs.
    - diverses valeurs (valeur, min, max, incrément).
- Fonctionnalité : Définir `ScreenInteractive::Exit()`.
- Fonctionnalité : Ajout de `Loop` pour donner aux développeurs un meilleur contrôle sur la boucle principale. Cela
  peut être utilisé pour intégrer FTXUI dans une autre boucle principale, sans en prendre le plein
  contrôle.
- Fonctionnalité : `Input` prend en charge CTRL+Gauche et CTRL+Droite.
- Fonctionnalité : Utilise une barre clignotante dans le composant `Input`.
- Amélioration : Le `Menu` conserve le focus lorsqu'une entrée est sélectionnée avec la
  souris.
- Correction de bug : Ajout de l'implémentation de `ButtonOption::Border()`. Elle était manquante.
- Correction de bug : Fournir la bonne touche pour F1-F4 et F11.
- Fonctionnalité : Ajout des décorateurs de composant `Hoverable`.

### Écran
- Fonctionnalité : Ajout de `Box::Union(a,b) -> Box`.
- Correction de bug : Correction de la réinitialisation de `dim` en conflit avec la réinitialisation de `bold`.
- Fonctionnalité : Ajout du support de redimensionnement d'écran `emscripten`.
- Correction de bug : Ajout du support Unicode 13 pour les caractères pleine largeur.
- Correction de bug : Correction du traitement par MSVC de la fonction `codecvt C++17` dépréciée comme une erreur.

### Compilation
- Support de l'utilisation de la version de google test fournie par le gestionnaire de paquets.

3.0.0
-----

### Compilation
- **rupture** : Le préfixe de la bibliothèque est maintenant revenu à "lib" (par défaut). Cela
    signifie que les utilisateurs non-cmake ne devraient pas lier contre "libftxui-dom" par exemple.

### Composant
- Module **Animations** ! Les composants peuvent implémenter la méthode `OnAnimation` et
  l'animation::Animator pour définir des propriétés animées.
  - `Menu` prend désormais en charge les animations.
  - `Button` prend désormais en charge les animations.
- Support de SIGTSTP. (ctrl+z).
- Support de l'envoi de tâches. `ScreenInteractive::Post(Task)`.
- `Menu` peut maintenant être utilisé dans les 4 directions, en utilisant `MenuOption.direction`.
- `Menu` peut afficher un soulignement animé, en utilisant
  `MenuOption.underline.enabled`.
- `Button` prend maintenant le focus dans le cadre.
- **rupture** Toutes les options utilisent maintenant une fonction de transformation.
- **rupture** Le composant `Toggle` est maintenant implémenté en utilisant `Menu`.
- **correction de bug** `Container::Tab` implémente `Focusable()`.
- **correction de bug** Implémentations par défaut améliorées des méthodes `Focusable()` et
  `ActiveChild()` de `ComponentBase`.
- **correction de bug** Convertit automatiquement les touches '\r' en '\n' pour les programmes Linux qui
  n'envoient pas le code correct pour la touche de retour, comme le 'bind'.
  https://github.com/ArthurSonzogni/FTXUI/issues/337
- Ajout de décorateurs pour les composants :
  - `operator|(Component, ComponentDecorator)`
  - `operator|(Component, ElementDecorator)`
  - `operator|=(Component, ComponentDecorator)`
  - `operator|=(Component, ElementDecorator)`
  - Ajout du décorateur `Maybe`.
  - Ajout du décorateur `CatchEvent`.
  - Ajout du décorateur `Renderer`.
- **rupture** suppression de l'en-tête "deprectated.hpp" et du support Input pour les chaînes
    larges.

### DOM:
- **rupture** : Le décorateur `inverted` bascule maintenant l'attribut inversé.
- Ajout de `gauge` pour les 4 directions. Expose l'API suivante :
```cpp
Element gauge(float ratio);
Element gaugeLeft(float ratio);
Element gaugeRight(float ratio);
Element gaugeUp(float ratio);
Element gaugeDown(float ratio);
Element gaugeDirection(float ratio, GaugeDirection);
```
- Ajout des éléments `separatorHSelector` et `separatorVSelector`. Ceux-ci peuvent être utilisés
  pour mettre en évidence une zone.
- Ajout du décorateur `automerge`. Cela permet aux caractères séparateurs d'être fusionnés
  avec d'autres à proximité.
- Correction de la fonction de rendu `Table`, pour permettre la fusion automatique des caractères.
- **Correction de bug** : L'indicateur de défilement vertical (`vscroll_indicator`) calcule maintenant son décalage et sa taille
  correctement.
- Ajout de `operator|=(Element, Decorator)`

### Écran:
- Ajout : `Color::Interpolate(lambda, color_a, color_b)`.

2.0.0
-----

### Fonctionnalités:

#### Écran
- Ajout de `automerge` au champ de bits `Pixel`. Cela contrôle maintenant quels pixels sont
  fusionnés automatiquement.

#### DOM:
- Ajout de la classe `Canvas` et de la fonction `ElementFrom('canvas')`. Ensemble, les utilisateurs de
  la bibliothèque peuvent dessiner en utilisant des caractères braille et des blocs.
- Support des éléments DOM `flexbox`. Ceci est construit symétriquement au HTML.
  Tous les attributs suivants sont pris en charge : `direction`, `wrap`, `justify-content`,
  `align-items`, `align-content`, `gap`.
- Ajout des aides d'éléments DOM basées sur `flexbox` :
  - `paragraph`
  - `paragraphAlignLeft`
  - `paragraphAlignCenter`
  - `paragraphAlignRight`
  - `paragraphAlignJustify`
- Ajout des éléments d'aide basés sur `flexbox` : `hflow()`, `vflow()`.
- Ajout : `focusPositionRelative` et `focusPosition`.
- Ajout du constructeur `Table` à partir d'un vecteur 2D d'éléments, au lieu d'une chaîne.

#### Composant
- Ajout du composant `collapsible`.
- Ajout de `ScreenInteractive::WithRestoredIO`. Ceci décore un rappel. Ceci
  l'exécute avec les hooks du terminal temporairement désinstallés. Ceci est utile si
  vous voulez exécuter des commandes en utilisant directement `stdin`/`stdout`/`stderr`.

### Bug

#### Table
- Le séparateur horizontal et vertical de la `table` est maintenant correctement étendu.

#### Composant
- `Input` ne devrait pas prendre le focus lorsqu'il est survolé par la souris.
- La modification des `Input` pendant l'événement `on_enter`/`on_change` fonctionne maintenant correctement.

### Changements de rupture :
- Le comportement de `paragraph` a été modifié. Il retourne maintenant un `Element`,
  au lieu d'une liste d'éléments.

0.11.1
------

# Composant
- Fonctionnalité : Prise en charge des boutons PageUp/PageDown/Home/End.
- Correction de bug : Vérifier que les éléments sélectionnés sont dans les limites pour `Dropdown`.

# Compilation
- Correction de bug : Empaqueter la bibliothèque en utilisant la "config Release". Pas le débogage.

0.11
----

## Flux de travail github
- Ajout des artefacts Windows et MacOS.
- Fusion de tous les workflows.

## Bug
- Sur les systèmes Unix, retour aux dimensions d'écran {80,25} en cas d'échec.

## CMake
- Support de la bibliothèque partagée, via l'option `BUILD_SHARED_LIBS`.
- Ajout de la version de la bibliothèque et des liens symboliques.

0.10 (2021-09-30)
--------------------

## Bug
- Correction de la fusion automatisée des bordures.

### Dom
- Classe `Table()` pour construire des tableaux stylisés.
   Voir https://github.com/ArthurSonzogni/FTXUI/discussions/228
- `vscroll_indicator`. Affiche un indicateur de barre de défilement sur la droite.
- `separatorEmpty`. Un séparateur qui ne dessine rien.
- `separatorFixed`. Un séparateur qui dessine le caractère fourni.

### Composant
- `Maybe` : Afficher un composant conditionnellement basé sur un booléen.
- `Dropdown` : Une liste de sélection déroulante.

0.9 (2021-09-26)
----------------

La version initiale où le journal des modifications a été écrit.

Cette version comprend :

### écran
- Style:
  - Gras.
  - Clignotement.
  - Estompé.
  - Inversé.
  - Souligné.
  - Couleur de premier plan.
  - Couleur d'arrière-plan.
- Prise en charge de l'Unicode UTF8.
  - Caractère pleine largeur : 测试.
  - Caractères combinés : a⃒
- Un tampon de pochoir (Stencil buffer).
- Fusion automatique des caractères de dessin de boîte.
- Détection des dimensions du terminal.

### DOM

- Élément :
  - `text` & `vtext`
  - `separator` et 5 variations.
  - `gauge`
  - `border` et 6 variations.
  - `window`
  - `spinner`
  - `paragraph` et `hflow`.

- Disposition :
  - `hbox`
  - `vbox`
  - `dbox`
  - `gridbox`
  - `frame` : Dessin à l'intérieur d'une zone virtuelle, potentiellement plus grande que la réelle.
  - `focus`, `select` : fait défiler la vue intérieure d'un cadre, pour qu'elle soit visible.
  - `flex` et 8 variations. `filler`
  
- Décorateurs :
  - `bold`
  - `dim`
  - `inverted`
  - `blink`
  - `color`
  - `bgcolor`
  - `clearunder`

### Composant

- Conteneur :
  - `Container::Vertical`
  - `Container::Horizontal`
  - `Container::Tab`
- `Button`
- `Checkbox`
- `Input`
- `Menu`
- `MenuEntry`
- `Radiobox`
- `Toggle`
- `Slider`
- `Renderer` et variations
- `CatchEvent`

### DIVERS

- Fuzzer
- Tests utilisant gtest.
- Documentation Doxygen
- IWYU
- 52 exemples.
- Support pour WebAssembly.
- Support pour Windows et solution de repli pour les terminaux défectueux.
