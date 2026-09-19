@page installation_amalgamated Amalgamado
@tableofcontents

## Versión amalgamada

La versión amalgamada es una versión de un solo header (`ftxui.hpp`) y una sola
fuente (`ftxui.cpp`) de la biblioteca. Es una forma alternativa de integrar
FTXUI en tu proyecto, especialmente si no quieres usar un gestor de paquetes
o un sistema de compilación complejo.

También está disponible una versión verdaderamente de un solo archivo, solo header (`ftxui_all.hpp`).

### ¿Cómo obtenerla?

Puedes obtener los archivos amalgamados de varias formas:

1. **GitHub Releases (Recomendado)**: Descarga el `ftxui-amalgamated.zip`
   desde la [última versión](https://github.com/ArthurSonzogni/FTXUI/releases).
   *Nota: Esto estará disponible oficialmente a partir de la versión 7.0.0. Para
   versiones anteriores o antes del lanzamiento de 7.0.0, utiliza los artefactos de CI
   a continuación.*
   Contiene `ftxui.hpp`, `ftxui.cpp`, y el header-only `ftxui_all.hpp`.
2. **Artefactos de CI**: Si quieres la versión absolutamente más reciente de `main` (o
   antes del lanzamiento oficial de 7.0.0), puedes descargar los artefactos del
   [flujo de trabajo Amalgamate](https://github.com/ArthurSonzogni/FTXUI/actions/workflows/amalgamate.yaml).
3. **Generación manual**: Ejecuta el siguiente comando desde la raíz del
   repositorio clonado:
   ```bash
   ./tools/amalgamate.py
   ```

### ¿Cómo usarla?

#### Opción 1: Header/Source separados (Recomendado para velocidad de compilación)

Copia `ftxui.hpp` y `ftxui.cpp` en tu proyecto. Incluye `ftxui.cpp` en tu
lista de fuentes.

#### Opción 2: Solo Header

Copia `ftxui_all.hpp` en tu proyecto. En **exactamente un** archivo C++, define
`FTXUI_IMPLEMENTATION` antes de incluir el header:

```cpp
#define FTXUI_IMPLEMENTATION
#include "ftxui_all.hpp"
```

En otros archivos, simplemente puedes incluirlo sin la definición:

```cpp
#include "ftxui_all.hpp"
```

### Compilación

Al compilar tu proyecto, asegúrate de estar usando C++17 y de enlazar contra
hilos (threads).

```bash
g++ -std=c++17 your_main.cpp [ftxui.cpp] -o your_app -lpthread
```

<div class="section_buttons">

| Anterior          |
|:------------------|
| [Instalación](installation.html) |

</div>
