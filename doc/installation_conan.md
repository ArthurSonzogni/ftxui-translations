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

## 配置

確保環境設定正確後，建立一個 Conan 配置檔 `conanfile.txt`。此檔案用於宣告您的專案依賴項。FTXUI 的社群維護套件可在 [Conan Center](https://conan.io/center/recipes/ftxui) 上找到。

> [!note]
> 這是一個非官方的建置腳本。這表示它不是由 FTXUI
> 團隊維護，而是由社群維護。套件維護者似乎會積極更新它
> 到最新版本。非常感謝維護者的工作！

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

## 安裝依賴項並建置

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

@todo 考慮到中國多數地區使用Conan很有可能遇到各種網絡問題，我想做一個定制的版本說明，但是我對conan的了解有限再加上沒有找到合適的資料，因此這個計劃短暫的被擱置了，如果您知道方法，歡迎在[中文版本](xiaoditx.girhub.io/public/docs/ftxui%E4%B8%AD%E6%96%87%E7%BF%BB%E8%AF%91/installation/conan/)的下方留下評論以提醒我

---

<div class="section_buttons">

| 上一個          |
|:------------------|
| [入門](getting-started.html) |

</div>