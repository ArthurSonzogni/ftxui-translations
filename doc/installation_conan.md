@page installation_conan Conan
@tableofcontents

FTXUI 可以使用 Conan 套件管理器輕鬆取得並整合到您的專案中。

## 先決條件

首先，請確保您的系統已安裝 Conan。如果沒有，可以透過 pip 安裝：

```powershell
pip install conan
```
Conan 通常與 CMake 協同工作，因此您也需要安裝 CMake。確認 Conan 和 CMake 都已安裝後，建立一個專案目錄，例如 `ftxui-demo`：

```powershell
mkdir C:\ftxui-demo
cd C:\ftxui-demo
```

## 設定

確保環境設定正確後，建立一個 Conan 配置檔 `conanfile.txt`。此檔案用於宣告您的專案依賴項。FTXUI 的社群維護套件可在 [Conan Center](https://conan.io/center/recipes/ftxui) 上找到。

> [!note]
> 這是一個非官方的建置腳本。這表示它不是由 FTXUI
> 團隊維護，而是由社群維護。套件維護者似乎會積極更新它
> 到最新版本。非常感謝維護者的工作！

FTXUI 也在儲存庫的根目錄提供自己的 `conanfile.py`，並在每次 commit 時由 CI 測試。
它本身並未發布到 Conan Center，但可以
用來透過 `conan create .` 直接從本機的簽出（checkout）使用 FTXUI，
並可以作為官方 Conan Center
recipe 的基礎。

@todo 如果你熟悉這個流程，請考慮為 Conan Center 新增一個「官方」建置腳本。
這可以是一個在有新版本發布時自動更新 Conan Center 的 GitHub Action。

```ini
[requires]
ftxui/6.0.2

[generators]
CMakeDeps
CMakeToolchain

[layout]
cmake_layout
```

## 安裝相依套件並建置

配置完成後，執行以下命令來安裝 FTXUI 及其依賴項：

```powershell
conan install . --output-folder=build --build=missing
```

這將從 Conan 的遠端儲存庫下載並安裝 `ftxui/6.0.2` 及其所有依賴項。

安裝完成後，您可以透過在專案目錄中建立 `demo.cpp` 檔案來進行測試：

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

如果測試成功，您就可以在專案目錄中建立 `CMakeLists.txt` 檔案：

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

| 上一個          |
|:------------------|
| [入門](getting-started.html) |

</div>