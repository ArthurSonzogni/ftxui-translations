@page installation_nix Nix

> [!note]
> FTXUI 的作者对 Nix 不是很了解。此页面主要由 AI 生成。如果您有任何改进建议，请提交 PR。

## Nix Flake

FTXUI 附带一个 `flake.nix`，提供软件包和开发 shell。

### 构建库

```bash
nix build github:ArthurSonzogni/FTXUI
```

生成的软件包可通过 `result` 链接访问。

### 作为依赖项使用

将 FTXUI 添加到您的 flake 输入中：

```nix
{
  inputs.ftxui.url = "github:ArthurSonzogni/FTXUI";
}
```

然后在您的输出中引用 `ftxui.packages.<system>.ftxui`。

<div class="section_buttons">

| 上一步          |
|:------------------|
| [入门](getting-started.html) |

</div>
