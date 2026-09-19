@page getting-started 快速上手
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595256.jpg)

FTXUI 是一個函數式的 C++ 終端使用者介面函式庫。它分為三個主要模組，每個模組都建立在前一個模組之上。

# 三大模組

1.  **Screen**：最底層。處理終端機的狀態、顏色與原始字元網格。
2.  **DOM**：版面配置引擎。提供一組 `Element`，可組合成複雜且具響應性的版面。
3.  **Component**：互動層。處理使用者輸入（鍵盤、滑鼠）並管理應用程式的主迴圈。

# 安裝與相依性

若要在您的專案中設置 FTXUI，請參閱[安裝指南](installation.html)。

對於 CMake 使用者，最推薦的方式是使用 `FetchContent`。將以下內容加入您的 `CMakeLists.txt`：

```cmake
include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main # or a specific version like v7.0.3
)
FetchContent_MakeAvailable(ftxui)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE ftxui::ftxui)
```

# 您的第一個靜態 UI（DOM）

`DOM` 模組讓您能以宣告式的方式描述 UI。組合元素就像巢狀函式呼叫一樣簡單，或使用管線運算子來套用修飾器。

將此存為 `main.cpp`：

```cpp
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

int main() {
  using namespace ftxui;

  // Define the document structure
  Element document = vbox({
    text("FTXUI Getting Started") | bold | center,
    separator(),
    hbox({
      text("Left Panel") | border,
      vbox({
        text("Main Content Area") | flex,
        separator(),
        text("Footer Information") | dim,
      }) | border | flex,
    }) | flex,
  });

  // Create the screen and render
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}
```

# 加入互動性（Component）

若要處理使用者輸入並建立動態應用程式，請使用 `Component` 模組與 `App` 類別。元件會管理自身的狀態，並可透過容器進行組合。

```cpp
#include <ftxui/component/component.hpp>
#include <ftxui/component/app.hpp>
#include <ftxui/dom/elements.hpp>

int main() {
  using namespace ftxui;

  std::vector<std::string> entries = {
      "Entry 1",
      "Entry 2",
      "Entry 3",
  };
  int selected = 0;

  // Create a menu component
  auto menu = Menu(&entries, &selected);

  // You can decorate components using the pipe operator.
  auto component = menu | border;

  // Start the main loop
  auto app = App::TerminalOutput();
  app.Loop(component);

  return 0;
}
```

# 下一步

*   瀏覽[範例](https://arthursonzogni.github.io/FTXUI/examples/)以了解可能的用法。
*   查看[入門範本](https://github.com/ArthurSonzogni/ftxui-starter)以取得預先配置好的專案結構。

<div class="section_buttons">
 
| 上一頁                          | 下一頁                    |
|:----------------------------------|------------------------:|
| [介紹](index.html) | [模組](modules.html) |

 
</div>
