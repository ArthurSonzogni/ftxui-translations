@page installation_bazel Bazel

FTXUI se puede integrar en tu proyecto usando [Bazel](https://bazel.build)
con Bzlmod (módulos de Bazel).

La librería está registrada en el [Registro Central de Bazel](https://registry.bazel.build/modules/ftxui)

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

## Proyecto de Inicio

Puedes usar el proyecto de inicio oficial de Bazel para una configuración mínima de trabajo:

- [ftxui-bazel (inicio)](https://github.com/ArthurSonzogni/ftxui-bazel)
 
<div class="section_buttons">
 
| Anterior          |
|:------------------|
| [Empezando](getting-started.html) |
 
</div>