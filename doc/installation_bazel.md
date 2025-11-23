@page installation_bazel Bazel

FTXUI peut être intégré à votre projet en utilisant [Bazel](https://bazel.build)
avec Bzlmod (modules Bazel).

La bibliothèque est enregistrée dans le [Bazel Central Registry](https://registry.bazel.build/modules/ftxui)

**MODULE.bazel**
```starlark
bazel_dep(name = "ftxui", version = "6.1.9")
```

**BUILD.bazel**
```starlark
cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [
        "@ftxui//:component",
        "@ftxui//:dom",
        "@ftxui//:screen",
    ],
)
```

## Projet de démarrage

Vous pouvez utiliser le projet de démarrage Bazel officiel pour une configuration minimale fonctionnelle :

- [ftxui-bazel (projet de démarrage)](https://github.com/ArthurSonzogni/ftxui-bazel)
 
<div class="section_buttons">
 
| Précédent          |
|:------------------|
| [Démarrage rapide](getting-started.html) |
 
</div>