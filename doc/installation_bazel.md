@page installation_bazel Bazel

FTXUI 可以使用 [Bazel](https://bazel.build)
和 Bzlmod (Bazel 模块) 集成到您的项目中。

该库已在 [Bazel 中央注册表](https://registry.bazel.build/modules/ftxui) 中注册。

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

## 启动项目

您可以使用官方 Bazel 启动项目来获取最小工作设置：

- [ftxui-bazel (启动器)](https://github.com/ArthurSonzogni/ftxui-bazel)
 
<div class="section_buttons">
 
| 上一页          |
|:------------------|
| [入门](getting-started.html) |
 
</div>
