@page getting-started Getting Started
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595256.jpg)

# 安裝 FTXUI

要在您的專案中設定 FTXUI，請遵循[安裝指南](installation.html)，其中提供了多種建置系統和套件管理器的說明。

# 最小範例

將以下程式碼儲存為 `main.cpp`：

```cpp
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

int main() {
  using namespace ftxui;

  Element document = hbox({
    text("left")   | border,
    text("middle") | border | flex,
    text("right")  | border,
  });

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();
}
```

使用您偏好的建置系統建置並執行它。
如果不明確，請從[安裝頁面](installation.html)中描述的方法之一開始。

預期輸出：

```
┌────┐┌────────────────────────────────────┐┌─────┐
│left││middle                              ││right│
└────┘└────────────────────────────────────┘└─────┘
```

# 入門範本

對於一個完整可用的專案，請克隆官方入門儲存庫：

```bash
git clone https://github.com/ArthurSonzogni/ftxui-starter
```

遵循該儲存庫 `README.md` 中的建置說明。

<div class="section_buttons">
 
| 上一頁                          | 下一頁                    |
|:----------------------------------|------------------------:|
| [介紹](index.html) | [模組](modules.html) |

 
</div>
