@page installation_conan Conan
@tableofcontents

FTXUIは、Conanパッケージマネージャーを使用して簡単に取得し、プロジェクトに統合できます。

## Prerequisites

まず、Conanがシステムにインストールされていることを確認してください。インストールされていない場合は、pip経由でインストールできます。

```powershell
pip install conan
```
ConanはCMakeと連携することが多いため、CMakeもインストールする必要があります。ConanとCMakeの両方がインストールされていることを確認したら、例えば `ftxui-demo` というプロジェクトディレクトリを作成します。

```powershell
mkdir C:\ftxui-demo
cd C:\ftxui-demo
```

## Configuration

環境が正しくセットアップされていることを確認したら、Conan構成ファイル `conanfile.txt` を作成します。このファイルは、プロジェクトの依存関係を宣言するために使用されます。FTXUIのコミュニティが管理するパッケージは、[Conan Center](https://conan.io/center/recipes/ftxui) で見つけることができます。

> [!note]
> これは非公式のビルドスクリプトです。つまり、FTXUIチームではなく
> コミュニティによってメンテナンスされています。パッケージメンテナーは、最新のリリースに
> 積極的に更新しているようです。メンテナーの皆様に深く感謝いたします！

FTXUI also ships its own `conanfile.py` at the root of the repository, tested
by CI on every commit. It isn't published to Conan Center itself, but it can
be used to consume FTXUI directly from a local checkout via
`conan create .`, and could serve as the basis for an official Conan Center
recipe.

@todo If you are familiar with the process, please consider adding an "official" build script to Conan Center.
This could be a GitHub Action that automatically updates Conan Center upon new releases.

```ini
[requires]
ftxui/6.0.2

[generators]
CMakeDeps
CMakeToolchain

[layout]
cmake_layout
```

## Install Dependencies and Build

設定が完了したら、次のコマンドを実行してFTXUIとその依存関係をインストールします。

```powershell
conan install . --output-folder=build --build=missing
```

これにより、`ftxui/6.0.2` とそのすべての依存関係がConanのリモートリポジトリからダウンロードおよびインストールされます。

インストールが完了したら、プロジェクトディレクトリに `demo.cpp` ファイルを作成してテストできます。

```cpp
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>

int main() {
    using namespace ftxui;
    auto document = hbox({
        text(" Hello "),
        text("FTXUI ") | bold | color(Color::Red),
        text(" world! ")
    });
    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);
    std::cout << screen.ToString() << std::endl;
    return 0;
}
```

テストが成功したら、プロジェクトディレクトリに `CMakeLists.txt` ファイルを作成できます。

```cmake
cmake_minimum_required(VERSION 3.20)
project(ftxui-demo)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# Find the FTXUI package installed via Conan
find_package(ftxui CONFIG REQUIRED)

# Create the executable
add_executable(demo demo.cpp)

# Link the executable to the FTXUI library
target_link_libraries(demo PRIVATE ftxui::component)
```

@todo 中国大陆在这方面的下载可能会受限制，需要一个替代的方案

---

<div class="section_buttons">

| 前へ          |
|:------------------|
| [入門](getting-started.html) |

</div>