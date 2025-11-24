@page getting-started はじめに
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595256.jpg)

# FTXUIをインストールする

プロジェクトにFTXUIをセットアップするには、複数のビルドシステムとパッケージマネージャーの手順が記載されている[インストールガイド](installation.html)に従ってください。

# 最小限の例

以下のコードを`main.cpp`として保存します:

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

好みのビルドシステムを使用してビルドおよび実行します。  
不明な場合は、[インストールページ](installation.html)に記載されているいずれかの方法から始めてください。

期待される出力:

```
┌────┐┌────────────────────────────────────┐┌─────┐
│left││middle                              ││right│
└────┘└────────────────────────────────────┘└─────┘
```

# スターターテンプレート

完全に動作するプロジェクトの場合、公式のスターターリポジトリをクローンします:

```bash
git clone https://github.com/ArthurSonzogni/ftxui-starter
```

そのリポジトリの`README.md`にあるビルド手順に従ってください。

<div class="section_buttons">
 
| Previous                          | Next                    |
|:----------------------------------|------------------------:|
| [Introduction](index.html) | [Modules](modules.html) |

 
</div>