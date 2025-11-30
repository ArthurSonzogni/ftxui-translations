@page installation_nix Nix

> [!note]
> El autor de FTXUI no tiene muchos conocimientos sobre Nix. Esta página ha sido generada principalmente
> por IA. Si tienes alguna sugerencia para mejorarla, por favor abre un
> PR.

## Nix Flake

FTXUI se distribuye con un `flake.nix` que proporciona tanto paquetes como un shell de desarrollo.

### Construir la biblioteca

```bash
nix build github:ArthurSonzogni/FTXUI
```

El paquete resultante es accesible a través del enlace `result`.

### Usar como dependencia

Añade FTXUI a tus entradas de flake:

```nix
{
  inputs.ftxui.url = "github:ArthurSonzogni/FTXUI";
}
```

Luego, haz referencia a `ftxui.packages.<system>.ftxui` en tus salidas.

<div class="section_buttons">

| Anterior          |
|:------------------|
| [Empezando](getting-started.html) |

</div>