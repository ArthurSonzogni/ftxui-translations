@page installation_meson Meson

@tableofcontents

FTXUI admite [Meson](https://mesonbuild.com/) como sistema de compilación alternativo.

## Prerrequisitos

- Compilador compatible con C++17 (GCC >= 7, Clang >= 5, MSVC >= 2017)
- [Meson](https://mesonbuild.com/Getting-meson.html) >= 1.1.0
- [Ninja](https://ninja-build.org/) (backend recomendado)

## Inicio rápido

```bash
# Configure (library only)
meson setup builddir

# Build
ninja -C builddir

# Install
ninja -C builddir install
```

## Opciones de compilación

| Opción      | Predeterminado | Descripción                |
|-------------|---------|----------------------------|
| `examples`  | `false` | Compilar aplicaciones de ejemplo |
| `tests`     | `false` | Compilar y ejecutar pruebas        |

### Compilar con ejemplos

```bash
meson setup builddir -Dexamples=true
ninja -C builddir
```

### Compilar con pruebas

```bash
meson setup builddir -Dtests=true
ninja -C builddir
meson test -C builddir
```

### Compilar todo

```bash
meson setup builddir -Dexamples=true -Dtests=true
ninja -C builddir
meson test -C builddir
```

## Dependencias

### Google Test (solo pruebas)

Cuando las pruebas están habilitadas, FTXUI depende de [Google Test](https://github.com/google/googletest) (v1.17.0).

La resolución de dependencias sigue la misma estrategia que la compilación con CMake:

1. **gtest instalado en el sistema**: Si `gtest` está disponible mediante `pkg-config` o rutas del sistema, se usará directamente.
2. **Descarga automática (alternativa)**: Si no se encuentra en el sistema, Meson descargará automáticamente gtest v1.17.0 mediante [WrapDB](https://mesonbuild.com/Wrapdb-projects.html) usando el archivo `subprojects/gtest.wrap`.

No se requiere intervención manual — Meson maneja esto de forma transparente.

## Objetivos de la biblioteca

La compilación produce tres bibliotecas:

| Biblioteca            | Descripción                          |
|--------------------|--------------------------------------|
| `libftxui-screen`  | Renderizado de terminal y entrada         |
| `libftxui-dom`     | Elementos de diseño y estilo          |
| `libftxui-component` | Componentes interactivos de UI          |

Dependencias entre ellas:
```
component → dom → screen
```

## Usar FTXUI como subproyecto de Meson

Para usar FTXUI en su propio proyecto Meson, cree un archivo wrap en `subprojects/ftxui.wrap`:

```ini
[wrap-git]
url = https://github.com/ArthurSonzogni/FTXUI.git
revision = v7.0.3

[provide]
ftxui-screen = ftxui_screen_dep
ftxui-dom = ftxui_dom_dep
ftxui-component = ftxui_component_dep
```

Luego, en su `meson.build`:

```meson
ftxui_component_dep = dependency('ftxui-component',
  fallback: ['ftxui', 'ftxui_component_dep'],
)

executable('my_app',
  'main.cpp',
  dependencies: ftxui_component_dep,
)
```

## Reconfigurar

Para cambiar las opciones después de la configuración inicial:

```bash
meson configure builddir -Dtests=true
ninja -C builddir
```

## Compilación limpia

```bash
rm -rf builddir
meson setup builddir
```
