@page installation_amalgamated アマルガメーション
@tableofcontents

## アマルガメーション版

アマルガメーション版は、ライブラリの単一ヘッダー（`ftxui.hpp`）および単一ソース
（`ftxui.cpp`）版です。パッケージマネージャーや複雑なビルドシステムを使いたくない場合など、
FTXUIをプロジェクトに統合する代替方法です。

真の単一ファイル・ヘッダーオンリー版（`ftxui_all.hpp`）も利用可能です。

### 入手方法は？

アマルガメーションファイルはいくつかの方法で入手できます:

1. **GitHub リリース（推奨）**: [最新リリース](https://github.com/ArthurSonzogni/FTXUI/releases)から
   `ftxui-amalgamated.zip` をダウンロードしてください。
   *注意: これはバージョン7.0.0から正式に利用可能になります。それ以前の
   バージョンや7.0.0リリース以前は、以下のCIアーティファクトを使用してください。*
   これには `ftxui.hpp`、`ftxui.cpp`、およびヘッダーオンリーの `ftxui_all.hpp` が含まれます。
2. **CI アーティファクト**: `main` からの絶対最新版が欲しい場合（または
   正式な7.0.0リリース以前の場合）、[Amalgamateワークフロー](https://github.com/ArthurSonzogni/FTXUI/actions/workflows/amalgamate.yaml)
   からアーティファクトをダウンロードできます。
3. **手動生成**: クローンしたリポジトリのルートから、以下のコマンドを実行してください:
   ```bash
   ./tools/amalgamate.py
   ```

### 使い方は？

#### オプション1: ヘッダー/ソースの分割（ビルド速度のために推奨）

`ftxui.hpp` と `ftxui.cpp` をプロジェクトにコピーしてください。`ftxui.cpp` を
ソースリストに含めてください。

#### オプション2: ヘッダーオンリー

`ftxui_all.hpp` をプロジェクトにコピーしてください。**ちょうど1つの** C++ファイルで、
ヘッダーをインクルードする前に `FTXUI_IMPLEMENTATION` を定義してください:

```cpp
#define FTXUI_IMPLEMENTATION
#include "ftxui_all.hpp"
```

他のファイルでは、単純に定義なしでインクルードするだけです:

```cpp
#include "ftxui_all.hpp"
```

### コンパイル

プロジェクトをコンパイルする際は、C++17を使用し、スレッドライブラリを
リンクしていることを確認してください。

```bash
g++ -std=c++17 your_main.cpp [ftxui.cpp] -o your_app -lpthread
```

<div class="section_buttons">

| 前へ          |
|:------------------|
| [インストール](installation.html) |

</div>
