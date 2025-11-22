@page installation_cmake CMake

@tableofcontents

此页面解释了如何使用 [CMake](https://cmake.org) 依赖 FTXUI。

# 集成方法

## 使用 FetchContent

此方法在配置时下载 FTXUI，不需要系统范围安装。

```cmake
include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9  # 替换为版本、标签或提交哈希
)

FetchContent_MakeAvailable(ftxui)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

这确保了可重现的构建和简单的依赖管理。

## 使用 find_package

如果 FTXUI 已在系统范围内安装或通过包管理器（例如 vcpkg 或 Conan）安装，您可以使用：

```cmake
find_package(ftxui REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

请确保该包在您的 `CMAKE_PREFIX_PATH` 中可见。

## 使用 git submodule

您还可以将 FTXUI 添加为 [Git 子模块](https://git-scm.com/book/en/v2/Git-Tools-Submodules)，将其作为存储库的一部分：

```cmake
git submodule add https://github.com/ArthurSonzogni/FTXUI external/ftxui
git submodule update --init --recursive
```

当克隆已包含 FTXUI 作为子模块的存储库时，请确保使用以下命令获取子模块：

```
git clone --recurse-submodules <your-repo>
# 或者，如果已克隆：
git submodule update --init --recursive
```

然后在您的 `CMakeLists.txt` 中：

```cmake
add_subdirectory(external/ftxui)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

如果您想将 FTXUI 供应商化到您自己的存储库中，此方法非常有效。

# 可选 CMake 标志

FTXUI 支持以下 CMake 选项：

| 选项                            | 描述                   | 默认值 |
| --------------------------------- | ----------------------------- | ------- |
| FTXUI_BUILD_EXAMPLES              | 构建捆绑示例        | OFF     |
| FTXUI_BUILD_DOCS                  | 构建文档       | OFF     |
| FTXUI_BUILD_TESTS                 | 启用测试                  | OFF     |
| FTXUI_ENABLE_INSTALL              | 生成安装目标      | ON      |
| FTXUI_MICROSOFT_TERMINAL_FALLBACK | 改善 Windows 兼容性 | ON/OFF  |

要启用某个选项：

```
cmake -DFTXUI_BUILD_EXAMPLES=ON ..
```

# 验证集成

要确认设置正常工作，请构建并运行一个最小示例。
如果您需要完整的模板，请参阅：[ftxui-starter](https://github.com/ArthurSonzogni/ftxui-starter)

<div class="section_buttons">

| 上一步          |
|:------------------|
| [入门](getting-started.html) |

</div>
