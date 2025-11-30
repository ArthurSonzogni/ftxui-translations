<p align="center">
  <img src="https://github.com/ArthurSonzogni/FTXUI/assets/4759106/6925b6da-0a7e-49d9-883c-c890e1f36007" alt="Demo image"></img>
  <br/>
  <a href="#"><img src="https://img.shields.io/badge/c++-%2300599C.svg?style=flat&logo=c%2B%2B&logoColor=white"></img></a>
  <a href="http://opensource.org/licenses/MIT"><img src="https://img.shields.io/github/license/arthursonzogni/FTXUI?color=black"></img></a>
  <a href="#"><img src="https://img.shields.io/github/stars/ArthurSonzogni/FTXUI"></img></a>
  <a href="#"><img src="https://img.shields.io/github/forks/ArthurSonzogni/FTXUI"></img></a>
  <a href="#"><img src="https://img.shields.io/github/repo-size/ArthurSonzogni/FTXUI"></img></a>
  <a href="https://github.com/ArthurSonzogni/FTXUI/graphs/contributors"><img src="https://img.shields.io/github/contributors/arthursonzogni/FTXUI?color=blue"></img></a>
  <br/>
  <a href="https://github.com/ArthurSonzogni/FTXUI/issues"><img src="https://img.shields.io/github/issues/ArthurSonzogni/FTXUI"></img></a>
  <a href="https://repology.org/project/ftxui/versions"><img src="https://repology.org/badge/latest-versions/ftxui.svg" alt="latest packaged version(s)"></a>
  <a href="https://codecov.io/gh/ArthurSonzogni/FTXUI">
    <img src="https://codecov.io/gh/ArthurSonzogni/FTXUI/branch/master/graph/badge.svg?token=C41FdRpNVA"/>
  </a>


  <br/>
  <a href="https://arthursonzogni.github.io/FTXUI/">Documentación</a> ·
  <a href="https://github.com/ArthurSonzogni/FTXUI/issues">Reportar un Error</a> ·
  <a href="https://arthursonzogni.github.io/FTXUI/examples/">Ejemplos</a> .
  <a href="https://github.com/ArthurSonzogni/FTXUI/issues">Solicitar una Característica</a> ·
  <a href="https://github.com/ArthurSonzogni/FTXUI/pulls">Enviar una Solicitud de Extracción</a>

</p>

# FTXUI

<i>Interfaz de Usuario de Terminal Funcional (X)</i>

¡Una sencilla biblioteca C++ multiplataforma para interfaces de usuario basadas en terminal!

## Características
 * Estilo funcional. Inspirado en
   [1](https://hackernoon.com/building-reactive-terminal-interfaces-in-c-d392ce34e649?gi=d9fb9ce35901)
   y [React](https://reactjs.org/)
 * Sintaxis simple y elegante (en mi opinión)
 * Navegación con teclado y ratón.
 * Soporte para [UTF8](https://en.wikipedia.org/wiki/UTF-8) y [caracteres de ancho completo](https://en.wikipedia.org/wiki/Halfwidth_and_fullwidth_forms) (→ 测试)
 * Soporte para animaciones. [Demo 1](https://arthursonzogni.github.io/FTXUI/examples/?file=component/menu_underline_animated_gallery), [Demo 2](https://arthursonzogni.github.io/FTXUI/examples/?file=component/button_style)
 * Soporte para dibujo. [Demo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/canvas_animated)
 * Sin dependencias.
 * [Soporte para módulos C++20](https://arthursonzogni.github.io/FTXUI/cpp20-modules.html)
 * **Multiplataforma**: Linux/MacOS (objetivo principal), WebAssembly, Windows (¡Gracias a los colaboradores!).
 * Aprende con [ejemplos](#documentacion) y [tutoriales](#documentacion)
 * Múltiples paquetes:
     - CMake [FetchContent]([https://bewagner.net/programming/2020/05/02/cmake-fetchcontent/](https://cmake.org/cmake/help/latest/module/FetchContent.html)) (preferido)
     - [Bazel](https://registry.bazel.build/modules/ftxui)
     - [vcpkg](https://vcpkg.link/ports/ftxui)
     - [Conan](https://conan.io/center/recipes/ftxui) [paquete Debian](https://tracker.debian.org/pkg/ftxui)
     - [Paquete Ubuntu](https://launchpad.net/ubuntu/+source/ftxui)
     - [Arch Linux](https://aur.archlinux.org/packages/ftxui/)
    - [OpenSUSE](https://build.opensuse.org/package/show/devel:libraries:c_c++/ftxui)
    - [XMake](https://xmake.io) repositorio [paquete](https://github.com/xmake-io/xmake-repo/blob/dev/packages/f/ftxui/xmake.lua)
    - [Nix](https://github.com/ArthurSonzogni/FTXUI/blob/main/flake.nix)
 * Buenas prácticas: documentación, pruebas, fuzzers, pruebas de rendimiento, CI automatizado, empaquetado automatizado, etc...

## Documentación

- [Inicio CMake](https://github.com/ArthurSonzogni/ftxui-starter)
- [Inicio Bazel](https://github.com/ArthurSonzogni/ftxui-bazel)
- [Documentación](https://arthursonzogni.github.io/FTXUI/)
- [Ejemplos (WebAssembly)](https://arthursonzogni.github.io/FTXUI/examples/)
- [Compilar usando CMake](https://arthursonzogni.github.io/FTXUI/#build-cmake)
- [Compilar usando Bazel](https://arthursonzogni.github.io/FTXUI/#build-bazel)

## Ejemplo
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

## Galería corta

#### DOM

Este módulo define un conjunto jerárquico de Elementos. Un Elemento gestiona el diseño y puede ser adaptable a las dimensiones del terminal.

Se declaran en [<ftxui/dom/elements.hpp>](https://arthursonzogni.github.io/FTXUI/elements_8hpp_source.html)

<details><summary>Diseño</summary>

Los Elementos se pueden organizar juntos:
  - horizontalmente con `hbox`
  - verticalmente con `vbox`
  - dentro de una cuadrícula con `gridbox`
  - envolver en una dirección usando `flexbox`.

Los Elementos pueden volverse flexibles usando el decorador `flex`.

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2dom_2vbox_hbox_8cpp-example.html) usando `hbox`, `vbox` y `filler`.

![image](https://user-images.githubusercontent.com/4759106/147242524-7103b5d9-1a92-4e2d-ac70-b3d6740061e3.png)


[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2dom_2gridbox_8cpp-example.html) usando gridbox:

![image](https://user-images.githubusercontent.com/4759106/147242972-0db1f2e9-0790-496f-86e6-ed2c604f7a73.png)

[Ejemplo](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/hflow.cpp) usando flexbox:

![image](https://user-images.githubusercontent.com/4759106/147243064-780ac7cc-605b-475f-94b8-cf7c4aed03a5.png)

[Ver](https://arthursonzogni.github.io/FTXUI/examples_2dom_2hflow_8cpp-example.html) también esta [demostración](https://arthursonzogni.github.io/FTXUI/examples/?file=component/flexbox).

</details>

<details><summary>Estilo</summary>

Un elemento puede ser decorado usando las funciones:
  - `bold`
  - `italic`
  - `dim`
  - `inverted`
  - `underlined`
  - `underlinedDouble`
  - `blink`
  - `strikethrough`
  - `color`
  - `bgcolor`
  - `hyperlink`

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2dom_2style_gallery_8cpp-example.html)

![image](https://user-images.githubusercontent.com/4759106/147244118-380bf834-9e33-40df-9ff0-07c10f2598ef.png)

FTXUI soporta el operador pipe. Significa: `decorator1(decorator2(element))` y `element | decorator1 | decorator2` pueden ser usados.

</details>

<details><summary>Colores</summary>

FTXUI soporta todas las paletas de colores:

[Galería](https://arthursonzogni.github.io/FTXUI/examples_2dom_2color_gallery_8cpp-example.html) de colores:
![image](https://user-images.githubusercontent.com/4759106/147248595-04c7245a-5b85-4544-809d-a5984fc6f9e7.png)

</details>

<details><summary>Borde y separador</summary>

Usa el decorador `border` y el elemento `separator()` para subdividir tu UI:

```cpp
auto document = vbox({
    text("top"),
    separator(),
    text("bottom"),
}) | border;

```

[Demo](https://arthursonzogni.github.io/FTXUI/examples_2dom_2separator_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147244514-4135f24b-fb8e-4067-8896-bc53545583f7.png)

</details>

<details><summary>Texto y párrafo</summary>

Un fragmento de texto simple se representa usando `text("contenido")`.

Para admitir el ajuste de texto siguiendo los espacios, se proporcionan las siguientes funciones:
```cpp
Element paragraph(std::string text);
Element paragraphAlignLeft(std::string text);
Element paragraphAlignRight(std::string text);
Element paragraphAlignCenter(std::string text);
Element paragraphAlignJustify(std::string text);
```

[Ejemplo de párrafo](https://arthursonzogni.github.io/FTXUI/examples_2dom_2paragraph_8cpp-example.html)

![ezgif com-gif-maker (4)](https://user-images.githubusercontent.com/4759106/147251370-983a06e7-6f41-4113-92b8-942f43d34d06.gif)

</details>

<details><summary>Tabla</summary>

Una clase para estilizar fácilmente una tabla de datos.

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2dom_2table_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147250766-77d8ec9e-cf2b-486d-9866-1fd9f1bd2e6b.png)

</details>

<details><summary>Canvas</summary>

Se pueden realizar dibujos en un Canvas, usando caracteres braille, de bloque o simples:

[Ejemplo](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/canvas.cpp) simple:

![image](https://user-images.githubusercontent.com/4759106/147245843-76cc62fb-ccb4-421b-aacf-939f9afb42fe.png)

[Ejemplos](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/component/canvas_animated.cpp) complejos:

![ezgif com-gif-maker (3)](https://user-images.githubusercontent.com/4759106/147250538-783a8246-98e0-4a25-b032-3bd3710549d1.gif)
</details>

#### Componente

`ftxui/component` produce UI dinámicas, reactivas a la entrada del usuario. Define un conjunto de `ftxui::Component`. Un componente reacciona a Eventos (teclado, ratón, redimensionamiento, ...) y se renderiza como un Elemento (ver sección anterior).

Los componentes preconstruidos se declaran en [<ftxui/component/component.hpp>](https://arthursonzogni.github.io/FTXUI/component_8hpp_source.html)

<details><summary>Galería</summary>

[Galería](https://arthursonzogni.github.io/FTXUI/examples_2component_2gallery_8cpp-example.html) de múltiples componentes. ([demo](https://arthursonzogni.github.io/FTXUI/examples/?file=component/gallery))

![image](https://user-images.githubusercontent.com/4759106/147247330-b60beb9f-e665-48b4-81c0-4b01ee95bc66.png)

</details>

<details><summary>Radiobox</summary>

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2radiobox_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147246401-809d14a5-6621-4e36-8dd9-a2d75ef2a94e.png)

</details>

<details><summary>Checkbox</summary>

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2checkbox_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147246646-b86926a9-1ef9-4efb-af98-48a9b62acd81.png)

</details>

<details><summary>Entrada</summary>

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2input_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147247671-f1d6f606-1845-4e94-a4a0-d4273e9ae6bd.png)

</details>

<details><summary>Interruptor</summary>

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2toggle_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147249383-e2201cf1-b7b8-4a5a-916f-d761e3e7ae40.png)

</details>


<details><summary>Deslizador</summary>

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2slider_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147249265-7e2cad75-082c-436e-affe-44a550c480ab.png)

</details>


<details><summary>Menú</summary>

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2menu_8cpp-example.html):

![image](https://user-images.githubusercontent.com/4759106/147247822-0035fd6f-bb13-4b3a-b057-77eb9291582f.png)

</details>


<details><summary>División Redimensionable</summary>

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2resizable_split_8cpp-example.html):

![ezgif com-gif-maker](https://user-images.githubusercontent.com/4759106/147248372-c55512fe-9b96-4b08-a1df-d05cf2cae431.gif)
</details>


<details><summary>Desplegable</summary>

[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2component_2dropdown_8cpp-example.html):

![youtube-video-gif (3)](https://user-images.githubusercontent.com/4759106/147246982-1e821751-531c-4e1f-bc37-2fa290e143cd.gif)

</details>

<details><summary>Pestaña</summary>

[Vertical](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_vertical_8cpp-example.html):

![ezgif com-gif-maker (1)](https://user-images.githubusercontent.com/4759106/147250144-22ff044a-4773-4ff7-a49c-12ba4034acb4.gif)

[Horizontal](https://arthursonzogni.github.io/FTXUI/examples_2component_2tab_horizontal_8cpp-example.html):

  ![ezgif com-gif-maker (2)](https://user-images.githubusercontent.com/4759106/147250217-fe447e0f-7a99-4e08-948a-995087d9b40e.gif)



</details>

## Bibliotecas para FTXUI
- *¿Quieres compartir un Componente útil para FTXUI? Siéntete libre de añadir el tuyo aquí*
- [ftxui-grid-container](https://github.com/mingsheng13/grid-container-ftxui)
- [ftxui-ip-input](https://github.com/mingsheng13/ip-input-ftxui)
- [ftxui-image-view](https://github.com/ljrrjl/ftxui-image-view.git): Para Visualización de Imágenes.


## Proyectos usando FTXUI

Siéntete libre de añadir tus proyectos aquí:
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
- [keywords](https://github.com/Oakamoore/keywords) ([Jugar versión web :heart:](https://oakamoore.itch.io/keywords))
- [FTB - explorador de archivos de terminal](https://github.com/Cyxuan0311/FTB)
- [openJuice](https://github.com/mikomikotaishi/openJuice)
- [SHOOT!](https://github.com/ShingZhanho/ENGG1340-Project-25Spring)
- [VerifySN (Herramienta de Hash Rápido)](https://github.com/d06i/verifySN)
- [tic-tac-toe](https://github.com/birland/tic-tac-toe)
- [typing-speed-test](https://github.com/ymcx/typing-speed-test)

### [cpp-best-practices/game_jam](https://github.com/cpp-best-practices/game_jam)

Varios juegos usando FTXUI han sido creados durante la Game Jam:
- [TermBreaker](https://github.com/ArthurSonzogni/termBreaker) [**[Jugar versión web]**](https://arthursonzogni.com/TermBreaker/)
- [Minesweeper Marathon](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/minesweeper_marathon.md) [**[Jugar versión web]**](https://barlasgarden.com/minesweeper/index.html)
- [Grand Rounds](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/grandrounds.md)
- [LightsRound](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/LightsRound.v.0.1.0.md)
- [DanteO](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/danteo.md)
- [Sumo](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/sumo.md)
- [Drag Me aROUND](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/drag_me_around.md)
- [DisarmSelfDestruct](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/DisarmSelfDestruct.md)
- [TheWorld](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/TheWorld.md)
- [smoothlife](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/smoothlife.md)
- [Consu](https://github.com/cpp-best-practices/game_jam/blob/main/Jam1_April_2022/consu.md)



## Compilar usando CMake

Se recomienda **encarecidamente** usar CMake FetchContent para depender de FTXUI, así puedes especificar qué commit deseas usar.
```cmake
include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

target_link_libraries(your_target PRIVATE
    # Chose a submodule
    ftxui::component
    ftxui::dom
    ftxui::screen
)
```

# Compilar usando Bazel

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


# Compilar con algo más:
Si no lo haces, FTXUI puede ser usado desde los siguientes paquetes:
- CMake [FetchContent]([https://bewagner.net/programming/2020/05/02/cmake-fetchcontent/](https://cmake.org/cmake/help/latest/module/FetchContent.html)) (preferido),
- [Bazel](https://registry.bazel.build/modules/ftxui),
- [vcpkg](https://vcpkg.link/ports/ftxui),
- [Conan](https://conan.io/center/recipes/ftxui)
- [Paquete Debian](https://tracker.debian.org/pkg/ftxui),
- [Paquete Ubuntu](https://launchpad.net/ubuntu/+source/ftxui),
- [Arch Linux](https://aur.archlinux.com/packages/ftxui/),
- [OpenSUSE](https://build.opensuse.org/package/show/devel:libraries:c_c++/ftxui),
[Nix](https://github.com/ArthurSonzogni/FTXUI/blob/main/flake.nix),
[![Packaging status](https://repology.org/badge/vertical-allrepos/libftxui.svg)](https://repology.org/project/libftxui/versions)


Si eliges compilar y enlazar FTXUI tú mismo, `ftxui-component` debe ser el primero en el orden de enlace en relación con las otras bibliotecas de FTXUI, es decir,
```bash
g++ . . . -lftxui-component -lftxui-dom -lftxui-screen . . .
```

Para compilar FTXUI con módulos, consulta la [documentación](https://arthursonzogni.github.io/FTXUI/cpp20-modules.html)

## Contribuidores

<a href="https://github.com/ArthurSonzogni/FTXUI/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=ArthurSonzogni/FTXUI" />
</a>