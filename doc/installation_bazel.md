@page installation_bazel Bazel

FTXUIは、Bzlmod（Bazelモジュール）を使用して[Bazel](https://bazel.build)でプロジェクトに統合できます。

ライブラリは[Bazel Central Registry](https://registry.bazel.build/modules/ftxui)に登録されています。

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

## スタータープロジェクト

最小限の動作設定には、公式のBazelスタータープロジェクトを使用できます。

- [ftxui-bazel (starter)](https://github.com/ArthurSonzogni/ftxui-bazel)

<div class="section_buttons">

| Previous          |
|:------------------|
| [Getting Started](getting-started.html) |

</div>