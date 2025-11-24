@page installation_nix Nix

> [!note]
> FTXUIの作者はNixについてあまり詳しくありません。このページはほとんどAIによって
> 生成されました。改善のための提案がありましたら、PRをオープンしてください。

## Nix Flake

FTXUIには、パッケージと開発シェルを提供する`flake.nix`が付属しています。

### ライブラリをビルドする

```bash
nix build github:ArthurSonzogni/FTXUI
```

結果として生成されるパッケージは`result`リンクからアクセスできます。

### 依存関係として使用する

FTXUIをflakeの入力に追加します。

```nix
{
  inputs.ftxui.url = "github:ArthurSonzogni/FTXUI";
}
```

次に、出力で`ftxui.packages.<system>.ftxui`を参照します。

<div class="section_buttons">

| 前へ          |
|:------------------|
| [入門](getting-started.html) |

</div>