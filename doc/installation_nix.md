@page installation_nix Nix

> [!note]
> FTXUI 作者對 Nix 不是很了解。此頁面主要由 AI 生成。如果您有任何改進建議，請提交 PR。

## Nix Flake

FTXUI 附帶一個 `flake.nix`，提供套件和開發 shell。

### 建置函式庫

```bash
nix build github:ArthurSonzogni/FTXUI
```

產生的套件可透過 `result` 連結存取。

### 作為依賴項使用

將 FTXUI 添加到您的 flake 輸入：

```nix
{
  inputs.ftxui.url = "github:ArthurSonzogni/FTXUI";
}
```

然後在您的輸出中引用 `ftxui.packages.<system>.ftxui`。

<div class="section_buttons">

| 上一個          |
|:------------------|
| [入門](getting-started.html) |

</div>