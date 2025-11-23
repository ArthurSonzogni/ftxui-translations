@page installation_nix Nix

> [!note]
> L'auteur de FTXUI ne connaît pas très bien Nix. Cette page a été principalement
> générée par l'IA. Si vous avez des suggestions pour l'améliorer, veuillez ouvrir une
> PR.

## Nix Flake

FTXUI est livré avec un `flake.nix` fournissant à la fois des paquets et un shell de développement.

### Compiler la bibliothèque

```bash
nix build github:ArthurSonzogni/FTXUI
```

Le paquet résultant est accessible via le lien `result`.

### Utiliser comme dépendance

Ajoutez FTXUI à vos entrées flake :

```nix
{
  inputs.ftxui.url = "github:ArthurSonzogni/FTXUI";
}
```

Ensuite, référencez `ftxui.packages.<system>.ftxui` dans vos sorties.

<div class="section_buttons">

| Précédent          |
|:-------------------|
| [Démarrer](getting-started.html) |

</div>