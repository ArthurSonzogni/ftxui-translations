@page installation_bazel Bazel

FTXUI 可以使用 [Bazel](https://bazel.build) 與 Bzlmod (Bazel 模組) 整合到您的專案中。

該函式庫已在 [Bazel 中央註冊表](https://registry.bazel.build/modules/ftxui)中註冊。

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

## 入門專案

您可以使用官方 Bazel 入門專案來取得最少的工作設定：

- [ftxui-bazel (入門專案)](https://github.com/ArthurSonzogni/ftxui-bazel)
 
<div class="section_buttons">
 
| 上一個          |
|:------------------|
| [入門](getting-started.html) |
 
</div>