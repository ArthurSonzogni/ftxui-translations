@page getting-started 入门
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595256.jpg)

# 安装 FTXUI

要在您的项目中设置 FTXUI，请遵循[安装指南](installation.html)，其中提供了适用于多种构建系统和包管理器的说明。

# 最小示例

将以下代码保存为 `main.cpp`：

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

使用您首选的构建系统构建并运行它。  
如果不确定，请从[安装页面](installation.html)中描述的方法之一开始。

预期输出：

```
┌────┐┌────────────────────────────────────┐┌─────┐
│left││middle                              ││right│
└────┘└────────────────────────────────────┘└─────┘
```

# 入门模板

对于一个完整的工作项目，克隆官方入门存储库：

```bash
git clone https://github.com/ArthurSonzogni/ftxui-starter
```

遵循该存储库 `README.md` 中的构建说明。

<div class="section_buttons">
 
| 上一个                          | 下一个                    |
|:----------------------------------|------------------------:|
| [简介](index.html) | [模块](modules.html) |

 
</div>