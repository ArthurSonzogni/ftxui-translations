@page installation_xmake XMake
@tableofcontents

## Paquet XMake (Non officiel)

FTXUI est disponible dans le [dépôt xmake](https://github.com/xmake-io/xmake-repo/blob/dev/packages/f/ftxui/xmake.lua)

Exemple d'extrait de `xmake.lua` :

```lua
add_requires("ftxui", {system = false})

target("demo")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("ftxui")
```

Référez-vous à la [documentation XMake](https://xmake.io) pour plus d'options.

> [!note]
> Ceci est un paquet **non officiel**. Cela signifie qu'il n'est pas maintenu par l'équipe FTXUI, mais par la communauté.

---

> [!note]
> L'auteur de FTXUI n'a pas beaucoup de connaissances sur openSUSE. Cette page a été
> principalement générée par l'IA. Si vous avez des suggestions pour l'améliorer, veuillez
> ouvrir une Pull Request.

---

<div class="section_buttons">

| Précédent          |
|:------------------|
| [Démarrage rapide](getting-started.html) |

</div>