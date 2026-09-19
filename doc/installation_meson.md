@page installation_meson Meson

@tableofcontents

FTXUI supporte [Meson](https://mesonbuild.com/) comme système de build
alternatif.

## Prérequis

- Un compilateur compatible C++17 (GCC >= 7, Clang >= 5, MSVC >= 2017)
- [Meson](https://mesonbuild.com/Getting-meson.html) >= 1.1.0
- [Ninja](https://ninja-build.org/) (backend recommandé)

## Démarrage rapide

```bash
# Configurer (bibliothèque seulement)
meson setup builddir

# Compiler
ninja -C builddir

# Installer
ninja -C builddir install
```

## Options de build

| Option      | Défaut | Description                       |
|-------------|--------|------------------------------------|
| `examples`  | `false` | Compiler les exemples d'applications |
| `tests`     | `false` | Compiler et exécuter les tests       |

### Compiler avec les exemples

```bash
meson setup builddir -Dexamples=true
ninja -C builddir
```

### Compiler avec les tests

```bash
meson setup builddir -Dtests=true
ninja -C builddir
meson test -C builddir
```

### Tout compiler

```bash
meson setup builddir -Dexamples=true -Dtests=true
ninja -C builddir
meson test -C builddir
```

## Dépendances

### Google Test (tests uniquement)

Lorsque les tests sont activés, FTXUI dépend de
[Google Test](https://github.com/google/googletest) (v1.17.0).

La résolution de la dépendance suit la même stratégie que le build CMake :

1. **gtest installé sur le système** : Si `gtest` est disponible via
   `pkg-config` ou les chemins système, il sera utilisé directement.
2. **Téléchargement automatique (solution de repli)** : S'il n'est pas trouvé
   sur le système, Meson téléchargera automatiquement gtest v1.17.0 via le
   [WrapDB](https://mesonbuild.com/Wrapdb-projects.html) en utilisant le
   fichier `subprojects/gtest.wrap`.

Aucune intervention manuelle n'est nécessaire — Meson gère cela de manière
transparente.

## Cibles de la bibliothèque

Le build produit trois bibliothèques :

| Bibliothèque       | Description                            |
|--------------------|-----------------------------------------|
| `libftxui-screen`  | Rendu terminal et entrées               |
| `libftxui-dom`     | Éléments de mise en page et de style    |
| `libftxui-component` | Composants d'interface interactifs    |

Dépendances entre elles :
```
component → dom → screen
```

## Utiliser FTXUI comme sous-projet Meson

Pour utiliser FTXUI dans votre propre projet Meson, créez un fichier wrap à
`subprojects/ftxui.wrap` :

```ini
[wrap-git]
url = https://github.com/ArthurSonzogni/FTXUI.git
revision = v7.0.3

[provide]
ftxui-screen = ftxui_screen_dep
ftxui-dom = ftxui_dom_dep
ftxui-component = ftxui_component_dep
```

Puis dans votre `meson.build` :

```meson
ftxui_component_dep = dependency('ftxui-component',
  fallback: ['ftxui', 'ftxui_component_dep'],
)

executable('my_app',
  'main.cpp',
  dependencies: ftxui_component_dep,
)
```

## Reconfiguration

Pour changer les options après la configuration initiale :

```bash
meson configure builddir -Dtests=true
ninja -C builddir
```

## Build propre

```bash
rm -rf builddir
meson setup builddir
```
