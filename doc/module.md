# ftxui {#ftxui}

![title-img](https://nsm09.casimages.com/img/2025/05/30//2505300816063242518595251.jpg)

FTXUI est organisé en trois modules, chacun s'appuyant sur le précédent :

1. [ftxui/screen](#module-screen) - Rendu de bas niveau
2. [ftxui/dom](#module-dom) - Mise en page et composition
3. [ftxui/component](#module-component) - Interaction utilisateur

---

# ftxui/screen

Définit :

- **`ftxui::Screen`** : une grille 2D de caractères stylisés.
- **`ftxui::Pixel`** : l'unité de rendu.
- Des assistants comme `ftxui::Color` et `Dimension`.

À utiliser pour le dessin direct et le style du terminal.

<div class="section_buttons">
 
|                                  Suivant |
|--------------------------------------:|
| [Documentation](module-screen.html) |
 
</div>


---

# ftxui/dom

Fournit :

- **`ftxui::Element`** : une structure arborescente pour la mise en page et l'interface utilisateur.
- Des éléments composables et réactifs.
- `Render()` pour dessiner sur un `Screen`.

Idéal pour les interfaces utilisateur structurées et stylisées.

<div class="section_buttons">

|                                  Suivant |
|--------------------------------------:|
| [Documentation](module-dom.html) |

</div>


---
# ftxui/component

Ajoute :

- **`ftxui::Component`** : des widgets interactifs et avec état.
- Intégrés : `Checkbox`, `Input`, `Menu`, `Button`.
- Prend en charge l'entrée clavier/curseur et la composition.

À utiliser pour les applications interactives.

<div class="section_buttons">

|                                  Suivant |
|--------------------------------------:|
| [Documentation](module-component.html) |

</div>

---

Les modules peuvent être utilisés indépendamment ou ensemble : `screen → dom → component`.