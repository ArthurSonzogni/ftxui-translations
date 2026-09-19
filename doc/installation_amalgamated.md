@page installation_amalgamated 合併版
@tableofcontents

## 合併版

合併版是函式庫的單一標頭檔（`ftxui.hpp`）與單一原始檔
（`ftxui.cpp`）版本。這是將 FTXUI 整合到您的專案中的另一種方式，
特別適合不想使用套件管理員或複雜建置系統的情況。

也提供了真正單一檔案的純標頭版本（`ftxui_all.hpp`）。

### 如何取得？

您可以透過幾種方式取得合併版檔案：

1. **GitHub Releases（建議）**：從
   [最新版本](https://github.com/ArthurSonzogni/FTXUI/releases)下載 `ftxui-amalgamated.zip`。
   *注意：此功能將自 7.0.0 版起正式提供。若使用
   較舊版本或在 7.0.0 版釋出前，請改用下方的 CI 產物。*
   內含 `ftxui.hpp`、`ftxui.cpp`，以及純標頭版本 `ftxui_all.hpp`。
2. **CI 產物**：若您想取得 `main` 分支的最新版本（或
   在 7.0.0 正式版釋出之前），可從
   [Amalgamate workflow](https://github.com/ArthurSonzogni/FTXUI/actions/workflows/amalgamate.yaml) 下載產物。
3. **手動產生**：在複製下來的儲存庫根目錄執行以下指令：
   ```bash
   ./tools/amalgamate.py
   ```

### 如何使用？

#### 選項 1：分離的標頭／原始碼（建議用於加快建置速度）

將 `ftxui.hpp` 與 `ftxui.cpp` 複製到您的專案中。並將 `ftxui.cpp` 加入您的
原始碼清單。

#### 選項 2：純標頭

將 `ftxui_all.hpp` 複製到您的專案中。在**恰好一個** C++ 檔案中，於包含此標頭檔之前
定義 `FTXUI_IMPLEMENTATION`：

```cpp
#define FTXUI_IMPLEMENTATION
#include "ftxui_all.hpp"
```

在其他檔案中，您可以直接包含它而不需定義該巨集：

```cpp
#include "ftxui_all.hpp"
```

### 編譯

編譯您的專案時，請確保使用 C++17 並連結執行緒函式庫。

```bash
g++ -std=c++17 your_main.cpp [ftxui.cpp] -o your_app -lpthread
```

<div class="section_buttons">

| 上一頁          |
|:------------------|
| [安裝](installation.html) |

</div>
