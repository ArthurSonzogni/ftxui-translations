@page installation_meson Meson

@tableofcontents

FTXUI 支援 [Meson](https://mesonbuild.com/) 作為替代的建置系統。

## 先決條件

- 相容 C++17 的編譯器（GCC >= 7、Clang >= 5、MSVC >= 2017）
- [Meson](https://mesonbuild.com/Getting-meson.html) >= 1.1.0
- [Ninja](https://ninja-build.org/)（建議使用的後端）

## 快速開始

```bash
# Configure (library only)
meson setup builddir

# Build
ninja -C builddir

# Install
ninja -C builddir install
```

## 建置選項

| Option      | Default | Description                |
|-------------|---------|----------------------------|
| `examples`  | `false` | 建置範例應用程式           |
| `tests`     | `false` | 建置並執行測試             |

### 搭配範例建置

```bash
meson setup builddir -Dexamples=true
ninja -C builddir
```

### 搭配測試建置

```bash
meson setup builddir -Dtests=true
ninja -C builddir
meson test -C builddir
```

### 建置所有項目

```bash
meson setup builddir -Dexamples=true -Dtests=true
ninja -C builddir
meson test -C builddir
```

## 相依套件

### Google Test（僅測試需要）

當啟用測試時，FTXUI 會依賴 [Google Test](https://github.com/google/googletest)（v1.17.0）。

相依套件的解析策略與 CMake 建置相同：

1. **系統已安裝的 gtest**：如果透過 `pkg-config` 或系統路徑可以取得 `gtest`，就會直接使用它。
2. **自動下載（後備方案）**：如果系統上找不到，Meson 會透過 [WrapDB](https://mesonbuild.com/Wrapdb-projects.html) 使用 `subprojects/gtest.wrap` 檔案自動下載 gtest v1.17.0。

不需要任何手動介入——Meson 會透明地處理這件事。

## 函式庫目標

建置會產生三個函式庫：

| Library            | Description                          |
|--------------------|--------------------------------------|
| `libftxui-screen`  | 終端機渲染與輸入                     |
| `libftxui-dom`     | 版面配置與樣式元素                   |
| `libftxui-component` | 互動式 UI 元件                    |

它們之間的相依關係：
```
component → dom → screen
```

## 將 FTXUI 作為 Meson 子專案使用

若要在你自己的 Meson 專案中使用 FTXUI，請在 `subprojects/ftxui.wrap` 建立一個 wrap 檔案：

```ini
[wrap-git]
url = https://github.com/ArthurSonzogni/FTXUI.git
revision = v7.0.3

[provide]
ftxui-screen = ftxui_screen_dep
ftxui-dom = ftxui_dom_dep
ftxui-component = ftxui_component_dep
```

接著在你的 `meson.build` 中：

```meson
ftxui_component_dep = dependency('ftxui-component',
  fallback: ['ftxui', 'ftxui_component_dep'],
)

executable('my_app',
  'main.cpp',
  dependencies: ftxui_component_dep,
)
```

## 重新設定

若要在初始設定後變更選項：

```bash
meson configure builddir -Dtests=true
ninja -C builddir
```

## 乾淨建置

```bash
rm -rf builddir
meson setup builddir
```
