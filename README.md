<p align="center">
  <img src="https://github.com/ArthurSonzogni/FTXUI/assets/4759106/6925b6da-0a7e-49d9-883c-c890e1f36007" alt="Image de démonstration"></img>
  <br/>
  <a href="#"><img src="https://img.shields.io/badge/c++-%2300599C.svg?style=flat&logo=c%2B%2B&logoColor=white"></img></a>
  <a href="http://opensource.org/licenses/MIT"><img src="https://img.shields.io/github/license/arthursonzogni/FTXUI?color=black"></img></a>
  <a href="#"><img src="https://img.shields.io/github/stars/ArthurSonzogni/FTXUI"></img></a>
  <a href="#"><img src="https://img.shields.io/github/forks/ArthurSonzogni/FTXUI"></img></a>
  <a href="#"><img src="https://img.shields.io/github/repo-size/ArthurSonzogni/FTXUI"></img></a>
  <a href="https://github.com/ArthurSonzogni/FTXUI/graphs/contributors"><img src="https://img.shields.io/github/contributors/arthursonzogni/FTXUI?color=blue"></img></a>
  <br/>
  <a href="https://github.com/ArthurSonzogni/FTXUI/issues"><img src="https://img.shields.io/github/issues/ArthurSonzogni/FTXUI"></img></a>
  <a href="https://repology.org/project/ftxui/versions"><img src="https://repology.org/badge/latest-versions/ftxui.svg" alt="dernières versions empaquetées"></a>
  <a href="https://codecov.io/gh/ArthurSonzogni/FTXUI">
    <img src="https://codecov.io/gh/ArthurSonzogni/FTXUI/branch/master/graph/badge.svg?token=C41FdRpNVA"/>
  </a>

  
  <br/>
  <a href="https://arthursonzogni.github.io/FTXUI/">Documentation</a> ·
  <a href="https://github.com/ArthurSonzogni/FTXUI/issues">Signaler un Bug</a> ·
  <a href="https://arthursonzogni.github.io/FTXUI/examples/">Exemples</a> ·
  <a href="https://github.com/ArthurSonzogni/FTXUI/issues">Demander une Fonctionnalité</a> ·
  <a href="https://github.com/ArthurSonzogni/FTXUI/pulls">Envoyer une Pull Request</a>

</p>

# FTXUI

<i>Interface Utilisateur de Terminal Fonctionnelle (X)</i>

Une bibliothèque C++ simple et multiplateforme pour les interfaces utilisateur basées sur le terminal !

## Fonctionnalités
 * Style fonctionnel. Inspiré par
   [1](https://hackernoon.com/building-reactive-terminal-interfaces-in-c-d392ce34e649?gi=d9fb9ce35901)
   et [React](https://reactjs.org/)
 * Syntaxe simple et élégante (à mon avis)
 * Navigation au clavier et à la souris.
 * Prise en charge de [UTF8](https://en.wikipedia.org/wiki/UTF-8) et des [caractères pleine chasse](https://en.wikipedia.org/wiki/Halfwidth_and_fullwidth_forms) (→ 测试)
 * Prise en charge des animations. [Démo 1](https://arthursonzogni.github.io/FTXUI/examples/?file=component/menu_underline_animated_gallery), [Démo 2](https://arthursonzogni.github.io/FTXUI/examples/?file=component/button_style)
 * Prise en charge du dessin. [Démo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/canvas_animated)
 * Aucune dépendance.
 * [Prise en charge des modules C++20](https://arthursonzogni.github.io/FTXUI/cpp20-modules.html)
 * **Multiplateforme**: Linux/MacOS (cible principale), WebAssembly, Windows (Merci aux contributeurs !).
 * Apprenez par les [exemples](#documentation) et les [tutoriels](#documentation)
 * Plusieurs packages:
     - CMake [FetchContent]([https://bewagner.net/programming/2020/05/02/cmake-fetchcontent/](https://cmake.org/cmake/help/latest/module/FetchContent.html)) (préféré)
     - [Bazel](https://registry.bazel.build/modules/ftxui)
     - [vcpkg](https://vcpkg.link/ports/ftxui)
     - [Conan](https://conan.io/center/recipes/ftxui) [Package Debian](https://tracker.debian.org/pkg/ftxui)
     - [Package Ubuntu](https://launchpad.net/ubuntu/+source/ftxui)
     - [Arch Linux](https://aur.archlinux.org/packages/ftxui/)
    - [OpenSUSE](https://build.opensuse.org/package/show/devel:libraries:c_c++/ftxui)
    - Dépôt [XMake](https://xmake.io) [package](https://github.com/xmake-io/xmake-repo/blob/dev/packages/f/ftxui/xmake.lua)
    - [Nix](https://github.com/ArthurSonzogni/FTXUI/blob/main/flake.nix)
 * Bonnes pratiques : documentation, tests, fuzzers, tests de performance, CI automatisée, packaging automatisé, etc...

## Documentation

- [Démarrage rapide CMake](https://github.com/ArthurSonzogni/ftxui-starter)
- [Démarrage rapide Bazel](https://github.com/ArthurSonzogni/ftxui-bazel)
- [Documentation](https://arthursonzogni.github.io/FTXUI/)
- [Exemples (WebAssembly)](https://arthursonzogni.github.io/FTXUI/examples/)
- [Compiler avec CMake](https://arthursonzogni.github.io/FTXUI/#build-cmake)
- [Compiler avec Bazel](https://arthursonzogni.github.io/FTXUI/#build-bazel)

## Exemple
~~~cpp
    vbox({
      hbox({
        text("one") | border,
        text("two") | border | flex,
        text("three") | border | flex,
      }),

      gauge(0.25) | color(Color::Red),
      gauge(0.50) | color(Color::White),
      gauge(0.75) | color(Color::Blue),
    });
~~~

![image](https://github.com/ArthurSonzogni/FTXUI/assets/4759106/569bf043-4e85-4245-aad5-2324572135c4)

## Petite galerie

#### DOM

Ce module définit un ensemble hiérarchique d'Éléments. Un Élément gère la mise en page et peut être réactif aux dimensions du terminal.

Ils sont déclarés dans [<ftxui/dom/elements.hpp>](https://arthursonzogni.github.io/FTXUI/elements_8hpp_source.html)
  
<details><summary>Mise en page</summary>

Les Éléments peuvent être arrangés ensemble :
  - horizontalement avec `hbox`
  - verticalement avec `vbox`
  - dans une grille avec `gridbox`
  - enroulés le long d'une direction en utilisant `flexbox`.
  
Les Éléments peuvent devenir flexibles en utilisant le décorateur `flex`.
  
[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2dom_2vbox_hbox_8cpp-example.html) utilisant `hbox`, `vbox` et `filler`.

![image](https://user-images.githubusercontent.com/4759106/147242524-7103b5d9-1a92-4e2d-ac70-b3d6740061e3.png)
  
  
[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2dom_2gridbox_8cpp-example.html) utilisant gridbox :

![image](https://user-images.githubusercontent.com/4759106/147242972-0db1f2e9-0790-496f-86e6-ed2c604f7a73.png)

[Exemple](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/hflow.cpp) utilisant flexbox :

![image](https://user-images.githubusercontent.com/4759106/147243064-780ac7cc-605b-475f-94b8-cf7c4aed03a5.png)

[Voir](https://arthursonzogni.github.io/FTXUI/examples_2dom_2hflow_8cpp-example.html) aussi cette [démo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/flexbox).

</details>

<details><summary>Style</summary>

Un élément peut être décoré en utilisant les fonctions :
  - `bold` (gras)
  - `italic` (italique)
  - `dim` (atténué)
  - `inverted` (inversé)
  - `underlined` (souligné)
  - `underlinedDouble` (doublement souligné)
  - `blink` (clignotant)
  - `strikethrough` (barré)
  - `color` (couleur)
  - `bgcolor` (couleur de fond)
  - `hyperlink` (lien hypertexte)

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2dom_2style_gallery_8cpp-example.html)

![image](https://user-images.githubusercontent.com/4759106/147244118-380bf834-9e33-40df-9ff0-07c10f2598ef.png)
  
FTXUI prend en charge l'opérateur pipe. Cela signifie que `decorator1(decorator2(element))` et `element | decorator1 | decorator2` peuvent être utilisés.
  
</details>

<details><summary>Couleurs</summary>

FTXUI prend en charge toutes les palettes de couleurs :

[Galerie](https://arthursonzogni.github.io/FTXUI/examples_2dom_2color_gallery_8cpp-example.html) de couleurs :
![image](https://user-images.githubusercontent.com/4759106/147248595-04c7245a-5b85-4544-809d-a5984fc6f9e7.png)

</details>
  
<details><summary>Bordure et séparateur</summary>

Utilisez le décorateur `border` et l'élément `separator()` pour subdiviser votre interface utilisateur :
  
```cpp
auto document = vbox({
    text("top"),
    separator(),
    text("bottom"),
}) | border;

```

[Démo](https://arthursonzogni.github.io/FTXUI/examples_2dom_2separator_8cpp-example.html) :
  
![image](https://user-images.githubusercontent.com/4759106/147244514-4135f24b-fb8e-4067-8896-bc53545583f7.png)
  
</details>

<details><summary>Texte et paragraphe</summary>

Un simple morceau de texte est représenté en utilisant `text("contenu")`.

Pour prendre en charge le retour à la ligne automatique en fonction des espaces, les fonctions suivantes sont fournies :
```cpp
Element paragraph(std::string text);
Element paragraphAlignLeft(std::string text);
Element paragraphAlignRight(std::string text);
Element paragraphAlignCenter(std::string text);
Element paragraphAlignJustify(std::string text);
```
  
[Exemple de paragraphe](https://arthursonzogni.github.io/FTXUI/examples_2dom_2paragraph_8cpp-example.html)
  
![ezgif com-gif-maker (4)](https://user-images.githubusercontent.com/4759106/147251370-983a06e7-6f41-4113-92b8-942f43d34d06.gif)

</details>

<details><summary>Table</summary>

Une classe pour styliser facilement un tableau de données.

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2dom_2table_8cpp-example.html) :
  
![image](https://user-images.githubusercontent.com/4759106/147250766-77d8ec9e-cf2b-486d-9866-1fd9f1bd2e6b.png)

</details>

<details><summary>Canvas</summary>

Le dessin peut être effectué sur un Canvas, en utilisant le braille, les blocs ou de simples caractères :
  
[Exemple simple](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/canvas.cpp) :
  
![image](https://user-images.githubusercontent.com/4759106/147245843-76cc62fb-ccb4-421b-aacf-939f9afb42fe.png)

[Exemples complexes](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/component/canvas_animated.cpp) :
  
![ezgif com-gif-maker (3)](https://user-images.githubusercontent.com/4759106/147250538-783a8246-98e0-4a25-b032-3bd3710549d1.gif)  
</details>

#### Composant

`ftxui/component` produit une interface utilisateur dynamique, réactive aux entrées de l'utilisateur. Il définit un ensemble de `ftxui::Component`. Un composant réagit aux Événements (clavier, souris, redimensionnement, ...) et se rend en tant qu'Élément (voir section précédente).

Les composants pré-construits sont déclarés dans [<ftxui/component/component.hpp>](https://arthursonzogni.github.io/FTXUI/component_8hpp_source.html)

<details><summary>Galerie</summary>

[Galerie](https://arthursonzogni.github.io/FTXUI/examples_2component_2gallery_8cpp-example.html) de plusieurs composants. ([démo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/gallery))

![image](https://user-images.githubusercontent.com/4759106/147247330-b60beb9f-e665-48b4-81c0-4b01ee95bc66.png)

</details>

<details><summary>Radiobox</summary>

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2radiobox_8cpp-example.html) :
  
![image](https://user-images.githubusercontent.com/4759106/147246401-809d14a5-6621-4e36-8dd9-a2d75ef2a94e.png)

</details>

<details><summary>Checkbox</summary>

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2checkbox_8cpp-example.html) :

![image](https://user-images.githubusercontent.com/4759106/147246646-b86926a9-1ef9-4efb-af98-48a9b62acd81.png)

</details>

<details><summary>Input</summary>

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2input_8cpp-example.html) :

![image](https://user-images.githubusercontent.com/4759106/147247671-f1d6f606-1845-4e94-a4a0-d4273e9ae6bd.png)

</details>

<details><summary>Toggle</summary>

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2toggle_8cpp-example.html) :

![image](https://user-images.githubusercontent.com/4759106/147249383-e2201cf1-b7b8-4a5a-916f-d761e3e7ae40.png)

</details>


<details><summary>Slider</summary>

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2slider_8cpp-example.html) :

![image](https://user-images.githubusercontent.com/4759106/147249265-7e2cad75-082c-436e-affe-44a550c480ab.png)

</details>


<details><summary>Menu</summary>

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2menu_8cpp-example.html) :

![image](https://user-images.githubusercontent.com/4759106/147247822-0035fd6f-bb13-4b3a-b057-77eb9291582f.png)

</details>


<details><summary>ResizableSplit</summary>

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2resizable_split_8cpp-example.html) :

![ezgif com-gif-maker](https://user-images.githubusercontent.com/4759106/147248372-c55512fe-9b96-4b08-a1df-d05cf2cae431.gif)  
</details>


<details><summary>Dropdown</summary>

[Exemple](https://arthursonzogni.github.io/FTXUI/examples_2component_2dropdown_8cpp-example.html) :

![youtube-video-gif (3)](https://user-images.githubusercontent.com/4759106/147246982-1e821751-531c-4e1f-bc37-2fa290e143cd.gif)

</details>

<details><summary>Tab</summary>

[Vertical](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_vertical_8cpp-example.html) :
  
![ezgif com-gif-maker (1)](https://user-images.githubusercontent.com/4759106/147250144-22ff044a-4773-4ff7-a49c-12ba4034acb4.gif)

[Horizontal](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_horizontal_8cpp-example.html) :
  
  ![ezgif com-gif-maker (2)](https://user-images.githubusercontent.com/4759106/147250217-fe447e0f-7a99-4e08-948a-995087d9b40e.gif)

  

</details>

## Bibliothèques pour FTXUI
- *Vous voulez partager un composant utile pour FTXUI ? N'hésitez pas à ajouter le vôtre ici*
- [ftxui-grid-container](https://github.com/mingsheng13/grid-container-ftxui)
- [ftxui-ip-input](https://github.com/mingsheng13/ip-input-ftxui)
- [ftxui-image-view](https://github.com/ljrrjl/ftxui-image-view.git): Pour l'affichage d'images.


## Projets utilisant FTXUI
N'hésitez pas à ajouter vos projets ici :
- [json-tui](https://github.com/ArthurSonzogni/json-tui)
- [git-tui](https://github.com/ArthurSonzogni/git-tui)
- [ostree-tui](https://github.com/AP-Sensing/ostree-tui)
- [rgb-tui](https://github.com/ArthurSonzogni/rgb-tui)
- [chrome-log-beautifier](https://github.com/ArthurSonzogni/chrome-log-beautifier)
- [x86-64 CPU Architecture Simulation](https://github.com/AnisBdz/CPU)
- [ltuiny](https://github.com/adrianoviana87/ltuiny)
- [i3-termdialogs](https://github.com/mibli/i3-termdialogs)
- [simpPRU](https://github.com/VedantParanjape/simpPRU)
- [Pigeon ROS TUI](https://github.com/PigeonSensei/Pigeon_ros_tui)
- [hastur](https://github.com/robinlinden/hastur)
- [CryptoCalculator](https://github.com/brevis/CryptoCalculator)
- [todoman](https://github.com/aaleino/todoman)
- [TimeAccumulator](https://github.com/asari555/TimeAccumulator)
- [vantage](https://github.com/gokulmaxi/vantage)
- [tabdeeli](https://github.com/typon/tabdeeli)
- [tiles](https://github.com/tusharpm/tiles)
- [cachyos-cli-installer](https://github.com/cachyos/new-cli-installer)
- [beagle-config](https://github.com/SAtacker/beagle-config)
- [turing_cmd](https://github.com/DanArmor/turing_cmd)
- [StartUp](https://github.com/StubbornVegeta/StartUp)
- [eCAL monitor](https://github.com/eclipse-ecal/ecal)
- [Path Finder](https://github.com/Ruebled/Path_Finder)
- [rw-tui](https://github.com/LeeKyuHyuk/rw-tui)
- [resource-monitor](https://github.com/catalincd/resource-monitor)
- [ftxuiFileReader](https://github.com/J0sephDavis/ftxuiFileReader)
- [ftxui_CPUMeter](https://github.com/tzzzzzzzx/ftxui_CPUMeter)
- [Captain's log](https://github.com/nikoladucak/caps-log)
- [FTowerX](https://github.com/MhmRhm/FTowerX)
- [Caravan](https://github.com/r3w0p/caravan)
- [Step-Writer](https://github.com/BrianAnakPintar/step-writer)
- [XJ music](https://github.com/xjmusic/xjmusic)
- [UDP chat](https://github.com/Sergeydigl3/udp-chat-tui)
- [2048-cpp](https://github.com/Chessom/2048-cpp)
- [Memory game](https://github.com/mikolajlubiak/memory)
- [Terminal Animation](https://github.com/mikolajlubiak/terminal_animation)
- [pciex](https://github.com/s0nx/pciex)
- [Fallout terminal hacking](https://github.com/gshigin/yet-another-fallout-terminal-hacking-game)
- [Lazylist](https://github.com/zhuyongqi9/lazylist)
- [TUISIC](https://github.com/Dark-Kernel/tuisic)
- [inLimbo](https://github.com/nots1dd/inLimbo)
- [BestEdrOfTheMarket](https://github.com/Xacone/BestEdrOfTheMarket)
- [terminal-rain](https://github.com/Oakamoore/terminal-rain)
- [keywords](https://github.com/Oakamoore/keywords) ([Jouer à la version web :heart:](https://oakamoore.itch.io/keywords))
- [FTB - navigateur de fichiers terminal](https://github.com/Cyxuan0311/FTB)
- [openJuice](https://github.com/mikomikotaishi/openJuice)
- [SHOOT!](https://github.com/ShingZhanho/ENGG1340-Project-25Spring)
- [VerifySN (Outil de hachage rapide)](https://github.com/d06i/verifySN)
- [tic-tac-toe](https://github.com/birland/tic-tac-toe)
- [typing-speed-test](https://github.com/ymcx/typing-speed-test)

### [cpp-best-practices/game_jam](https://github.com/cpp-best-practices/game_jam)

Plusieurs jeux utilisant FTXUI ont été créés pendant la Game Jam :
- [TermBreaker](https://github.com/ArthurSonzogni/termBreaker) [**[Jouer à la version web]**](https://arthursonzogni.com/TermBreaker/)
- [Minesweeper Marathon](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/minesweeper_marathon.md) [**[Jouer à la version web]**](https://barlasgarden.com/minesweeper/index.html)
- [Grand Rounds](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/grandrounds.md)
- [LightsRound](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/LightsRound.v.0.1.0.md)
- [DanteO](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/danteo.md)
- [Sumo](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/sumo.md)
- [Drag Me aROUND](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/drag_me_around.md)
- [DisarmSelfDestruct](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/DisarmSelfDestruct.md)
- [TheWorld](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/TheWorld.md)
- [smoothlife](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/smoothlife.md)
- [Consu](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/consu.md)



## Compiler avec CMake

Il est **fortement** recommandé d'utiliser CMake FetchContent pour dépendre de FTXUI afin de pouvoir spécifier le commit sur lequel vous souhaitez dépendre.
```cmake
include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

target_link_libraries(your_target PRIVATE
    # Choisissez un sous-module
    ftxui::component
    ftxui::dom
    ftxui::screen
)
```

# Compiler avec Bazel

**MODULE.bazel**
```starlark
bazel_dep(
    name = "ftxui",
    version = "v6.1.9",
)
```

**BUILD.bazel**
```starlark
cc_binary(
    name = "your_target",
    srcs = ["your_source.cc"],
    deps = [
        "@ftxui//:component",
        "@ftxui//:dom",
        "@ftxui//:screen",
    ],
)
```


# Compiler avec autre chose :
Si vous ne le faites pas, FTXUI peut être utilisé à partir des packages suivants :
- CMake [FetchContent]([https://bewagner.net/programming/2020/05/02/cmake-fetchcontent/](https://cmake.org/cmake/help/latest/module/FetchContent.html)) (préféré),
- [Bazel](https://registry.bazel.build/modules/ftxui),
- [vcpkg](https://vcpkg.link/ports/ftxui),
- [Conan](https://conan.io/center/recipes/ftxui)
- [Package Debian](https://tracker.debian.org/pkg/ftxui),
- [Package Ubuntu](https://launchpad.net/ubuntu/+source/ftxui),
- [Arch Linux](https://aur.archlinux.org/packages/ftxui/),
- [OpenSUSE](https://build.opensuse.org/package/show/devel:libraries:c_c++/ftxui),
[Nix](https://github.com/ArthurSonzogni/FTXUI/blob/main/flake.nix),
[![Statut du packaging](https://repology.org/badge/vertical-allrepos/libftxui.svg)](https://repology.org/project/libftxui/versions)


Si vous choisissez de compiler et de lier FTXUI vous-même, `ftxui-component` doit être en premier dans l'ordre de liaison par rapport aux autres bibliothèques FTXUI, c'est-à-dire
```bash
g++ . . . -lftxui-component -lftxui-dom -lftxui-screen . . .
```

Pour compiler FTXUI avec des modules, consultez la [documentation](https://arthursonzogni.github.io/FTXUI/cpp20-modules.html)

## Contributeurs

<a href="https://github.com/ArthurSonzogni/FTXUI/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=ArthurSonzogni/FTXUI" />
</a>