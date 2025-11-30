@page installation_xmake XMake
@tableofcontents

## Paquete XMake (No oficial)

FTXUI está disponible en el [xmake-repo](https://github.com/xmake-io/xmake-repo/blob/dev/packages/f/ftxui/xmake.lua)

Fragmento de ejemplo de `xmake.lua`:

```lua
add_requires("ftxui", {system = false})

target("demo")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("ftxui")
```

Consulte la [documentación de XMake](https://xmake.io) para más opciones.

> [!note]
> Este es un paquete **no oficial**. Esto significa que no es mantenido por el
> equipo de FTXUI, sino por la comunidad.

---

> [!note]
> El autor de FTXUI no tiene muchos conocimientos sobre openSUSE. Esta página ha sido
> generada principalmente por IA. Si tienes alguna sugerencia para mejorarla, por favor abre
> un PR.

---

<div class="section_buttons">

| Anterior          |
|:------------------|
| [Primeros pasos](getting-started.html) |

</div>