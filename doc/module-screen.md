@page module-screen ftxui / screen
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595348.png)

Le module `ftxui::screen` est la fondation de bas niveau. Il peut être utilisé
de manière autonome, mais il est principalement conçu pour être utilisé conjointement par
les modules [ftxui::dom](module-dom.html) et [ftxui::component](module-component.html).

Il fournit un @ref ftxui::Screen.

---

# ftxui::Screen

La classe @ref ftxui::Screen représente une grille 2D de caractères stylisés qui peut
être affichée dans un terminal.
Elle fournit des méthodes pour créer un écran, accéder aux cellules et afficher des éléments.

Vous pouvez accéder aux cellules individuelles (@ref ftxui::Cell) de l'écran en utilisant 
la méthode @ref ftxui::Screen::CellAt, qui renvoie une référence
à la cellule aux coordonnées spécifiées.

**Example**
```cpp
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>

void main() {
    auto screen = ftxui::Screen::Create(
        ftxui::Dimension::Full(),   // Use full terminal width
        ftxui::Dimension::Fixed(10) // Fixed height of 10 rows
    );

    // Access a specific cell at (10, 5)
    auto& cell = screen.CellAt(10, 5);

    // Set properties of the cell.
    cell.character = "X";
    cell.foreground_color = ftxui::Color::Red;
    cell.background_color = ftxui::Color::RGB(0, 255, 0);
    cell.bold = true; // Set bold style
    screen.Print(); // Print the screen to the terminal
}
```

> [!note]
> Si les coordonnées sont hors limites, une cellule factice est renvoyée.

L'écran peut être affiché dans le terminal en utilisant @ref ftxui::Screen::Print() ou
converti en une std::string avec @ref ftxui::Screen::ToString().

<div class="tabbed">
 
- <b class="tab-title">Print()</b>
  ```cpp
  auto screen = ...;
  screen.Print();
  ```
- <b class="tab-title">ToString()</b>
  ```cpp
  auto screen = ...;
  std::cout << screen.ToString();
  ```
 
</div>

Notez que vous pouvez réinitialiser la position du curseur en haut à gauche de
l'écran après l'impression en appelant @ref ftxui::Screen::ResetPosition().

**Example**
```cpp
auto screen = ...;
while(true) {
  // Drawing operations:
  ...
  
  // Print the screen to the terminal. Then reset the cursor position and the
  // screen content.
  std::cout << screen.ToString();
  std::cout << screen.ResetPosition(/*clear=*/true);
  std::cout << std::flush;

  // Sleep for a short duration to control the refresh rate.
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
```

---

# ftxui::Dimension

L'utilitaire @ref ftxui::Dimension contrôle le dimensionnement de l'écran :

* `Dimension::Full()` — utiliser toute la largeur ou hauteur du terminal
* `Dimension::Fit(element)` — redimensionner pour s'adapter à l'élément rendu @ref ftxui::Element
* `Dimension::Fixed(n)` — utiliser exactement `n` colonnes ou lignes

Ces valeurs doivent être passées à `ftxui::Screen::Create()`.

@ref ftxui::Screen::Create() fournit deux surcharges :

- `Screen::Create(Dimension)` définit la largeur et la hauteur avec le même type de dimension
- `Screen::Create(Dimension width, Dimension height)` permet un contrôle distinct par axe

```cpp
auto screen = ftxui::Screen::Create(
  ftxui::Dimension::Full(),      // width
  ftxui::Dimension::Fixed(10)    // height
);
```

Une fois créé, affichez un élément et le résultat :

```cpp
ftxui::Render(screen, element);
screen.Print();
```

---

# ftxui::Cell

Chaque cellule de la grille de l'écran est un @ref ftxui::Cell, qui contient :

- Point de code Unicode.
    - `character`
- @ref ftxui::Color:
    - `foreground_color`
    - `background_color`
- Booléens :
    - `blink`
    - `bold`
    - `dim`
    - `italic`
    - `inverted` (échange les couleurs de premier plan et d'arrière-plan)
    - `underlined`
    - `underlined_double`
    - `strikethrough`


```cpp
auto screen = ftxui::Screen::Create(
  ftxui::Dimension::Fixed(5),
  ftxui::Dimension::Fixed(5),
);

auto& cell = screen.CellAt(3, 3);
cell.character = "X";
cell.bold = true;
cell.foreground_color = ftxui::Color::Red;
cell.background_color = ftxui::Color::RGB(0, 255, 0);

screen.Print();
```

> [!note]
> `CellAt(x, y)` effectue une vérification des limites et renvoie une référence à la cellule
> aux coordonnées spécifiées. Si hors limites, une référence de cellule factice est
> renvoyée.


Chaque cellule de l'écran est un @ref ftxui::Cell. Vous pouvez les modifier en utilisant :

```cpp
auto& cell = screen.CellAt(x, y);
cell.character = "X";
cell.bold = true;
cell.foreground_color = Color::Red;
```

---

# ftxui::Color

La classe @ref ftxui::Color est utilisée pour définir les couleurs de premier plan et d'arrière-plan pour chaque @ref ftxui::Cell.

Elle prend en charge différents espaces colorimétriques et palettes prédéfinies. FTXUI se
repliera dynamiquement sur la couleur la plus proche disponible dans le terminal si la
couleur demandée n'est pas prise en charge par le terminal.

**Espaces colorimétriques**

- **Défaut** : `ftxui::Color::Default` (couleur par défaut du terminal)
- **Palette de 16 couleurs** [Demo](https://arthursonzogni.github.io/FTXUI/examples/?file=dom/color_gallery):
    - `ftxui::Color::Black`,
    - `ftxui::Color::Red`,
    - ...
- **Palette de 256 couleurs** [Demo](https://arthursonzogni.github.io/FTXUI/examples/?file=dom/color_palette256):
    - `ftxui::Color::Chartreuse1`,
    - `ftxui::Color::DarkViolet`,
    - ...
- **Couleur réelle** (24bit) [Demo](https://arthursonzogni.github.io/FTXUI/examples/?file=dom/color_truecolor_HSV:
    - `ftxui::Color::RGB(uint8_t red, uint8_t green, uint8_t blue)`
    - `ftxui::Color::HSV(uint8_t h, uint8_t s, uint8_t v)`.
    

> [!note]
> You can query the terminal capability using @ref ftxui::Terminal::ColorSupport();
>
> This can manually be set using @ref ftxui::Terminal::SetColorSupport().
