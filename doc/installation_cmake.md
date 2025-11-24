@page installation_cmake CMake

@tableofcontents

このページでは、[CMake](https://cmake.org) を使用して FTXUI に依存する方法を説明します。

# 統合方法

## FetchContent の使用

このアプローチは、設定時に FTXUI をダウンロードし、システム全体のインストールを必要としません。

```cmake 
include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9  # Replace with a version, tag, or commit hash
)

FetchContent_MakeAvailable(ftxui)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

これにより、再現可能なビルドと簡単な依存関係管理が保証されます。

## find_package の使用

FTXUI がシステム全体に、またはパッケージマネージャー (vcpkg や Conan など) を介してインストールされている場合、次のように使用できます。

```cmake 
find_package(ftxui REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

パッケージが `CMAKE_PREFIX_PATH` で表示されることを確認してください。

## git submodule の使用

FTXUI をリポジトリの一部として保持する [Git サブモジュール](https://git-scm.com/book/en/v2/Git-Tools-Submodules) として追加することもできます。

```cmake
git submodule add https://github.com/ArthurSonzogni/FTXUI external/ftxui
git submodule update --init --recursive
```

FTXUI をサブモジュールとして既に含むリポジトリをクローンする場合は、次のコマンドでサブモジュールをフェッチしてください。

```
git clone --recurse-submodules <your-repo>
# Or, if already cloned:
git submodule update --init --recursive
```

次に、`CMakeLists.txt` で：

```cmake
add_subdirectory(external/ftxui)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

このアプローチは、FTXUI を独自のリポジトリにベンダーとして含めたい場合にうまく機能します。

# オプションの CMake フラグ

FTXUI は次の CMake オプションをサポートしています。

| オプション | 説明 | デフォルト |
| --------------------------------- | ----------------------------- | ------- |
| FTXUI_BUILD_EXAMPLES | バンドルされた例をビルドします | OFF |
| FTXUI_BUILD_DOCS | ドキュメントをビルドします | OFF |
| FTXUI_BUILD_TESTS | テストを有効にします | OFF |
| FTXUI_ENABLE_INSTALL | インストールターゲットを生成します | ON |
| FTXUI_MICROSOFT_TERMINAL_FALLBACK | Windows の互換性を向上させます | ON/OFF |

オプションを有効にするには：

```
cmake -DFTXUI_BUILD_EXAMPLES=ON ..
```

# 統合の検証

セットアップが機能していることを確認するには、最小限の例をビルドして実行します。
完全なテンプレートが必要な場合は、[ftxui-starter](https://github.com/ArthurSonzogni/ftxui-starter) を参照してください。

<div class="section_buttons">
 
| 前へ |
|:------------------|
| [開始方法](getting-started.html) |
 
</div>