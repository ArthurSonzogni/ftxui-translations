@page installation_meson Meson

@tableofcontents

FTXUIは代替のビルドシステムとして[Meson](https://mesonbuild.com/)をサポートしています。

## 前提条件

- C++17対応コンパイラ（GCC >= 7、Clang >= 5、MSVC >= 2017）
- [Meson](https://mesonbuild.com/Getting-meson.html) >= 1.1.0
- [Ninja](https://ninja-build.org/)（推奨バックエンド）

## クイックスタート

```bash
# Configure (library only)
meson setup builddir

# Build
ninja -C builddir

# Install
ninja -C builddir install
```

## ビルドオプション

| オプション   | デフォルト | 説明                       |
|-------------|---------|----------------------------|
| `examples`  | `false` | サンプルアプリケーションをビルドする |
| `tests`     | `false` | テストをビルドして実行する    |

### サンプルを含めてビルドする

```bash
meson setup builddir -Dexamples=true
ninja -C builddir
```

### テストを含めてビルドする

```bash
meson setup builddir -Dtests=true
ninja -C builddir
meson test -C builddir
```

### すべてをビルドする

```bash
meson setup builddir -Dexamples=true -Dtests=true
ninja -C builddir
meson test -C builddir
```

## 依存関係

### Google Test（テストのみ）

テストが有効な場合、FTXUIは[Google Test](https://github.com/google/googletest)（v1.17.0）に依存します。

依存関係の解決はCMakeビルドと同じ戦略に従います:

1. **システムにインストールされたgtest**: `gtest`が`pkg-config`やシステムパス経由で利用可能な場合、それが直接使用されます。
2. **自動ダウンロード（フォールバック）**: システム上に見つからない場合、Mesonは`subprojects/gtest.wrap`ファイルを使用して[WrapDB](https://mesonbuild.com/Wrapdb-projects.html)経由でgtest v1.17.0を自動的にダウンロードします。

手動での操作は不要です — Mesonがこれを透過的に処理します。

## ライブラリターゲット

このビルドは3つのライブラリを生成します:

| ライブラリ            | 説明                                  |
|--------------------|--------------------------------------|
| `libftxui-screen`  | ターミナルの描画と入力               |
| `libftxui-dom`     | レイアウトとスタイリング要素          |
| `libftxui-component` | インタラクティブなUIコンポーネント |

それらの間の依存関係:
```
component → dom → screen
```

## FTXUIをMesonサブプロジェクトとして使用する

自分のMesonプロジェクトでFTXUIを使用するには、`subprojects/ftxui.wrap`にwrapファイルを作成します:

```ini
[wrap-git]
url = https://github.com/ArthurSonzogni/FTXUI.git
revision = v7.0.3

[provide]
ftxui-screen = ftxui_screen_dep
ftxui-dom = ftxui_dom_dep
ftxui-component = ftxui_component_dep
```

次に、あなたの`meson.build`で:

```meson
ftxui_component_dep = dependency('ftxui-component',
  fallback: ['ftxui', 'ftxui_component_dep'],
)

executable('my_app',
  'main.cpp',
  dependencies: ftxui_component_dep,
)
```

## 再設定

初期設定後にオプションを変更するには:

```bash
meson configure builddir -Dtests=true
ninja -C builddir
```

## クリーンビルド

```bash
rm -rf builddir
meson setup builddir
```
