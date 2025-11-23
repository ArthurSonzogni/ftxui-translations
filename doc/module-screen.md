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
Elle fournit des méthodes pour créer un écran, accéder aux pixels et afficher des éléments.

Vous pouvez accéder aux cellules individuelles (@ref ftxui::Pixel) de l'écran en utilisant 
la méthode @ref ftxui::Screen::PixelAt, qui renvoie une référence
au pixel aux coordonnées spécifiées.

**Example**
```cpp
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>

void main() {
    auto screen = ftxui::Screen::Create(
        ftxui::Dimension::Full(),   // Utiliser toute la largeur du terminal
        ftxui::Dimension::Fixed(10) // Hauteur fixe de 10 lignes
    );

    // Accéder à un pixel spécifique en (10, 5)
    auto& pixel = screen.PixelAt(10, 5);

    // Définir les propriétés du pixel.
    pixel.character = U'X';
    pixel.foreground_color = ftxui::Color::Red;
    pixel.background_color = ftxui::Color::RGB(0, 255, 0);
    pixel.bold = true; // Définir le style gras
    screen.Print(); // Afficher l'écran dans le terminal
}
```

> [!note]
> Si les coordonnées sont hors limites, un pixel factice est renvoyé.

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
l'écran après l'impression en appelant @ref ftxui::Screen::ResetCursorPosition().

**Example**
```cpp
auto screen = ...;
while(true) {
  // Opérations de dessin :
  ...
  
  // Affiche l'écran dans le terminal. Puis réinitialise la position du curseur et le
  // contenu de l'écran.
  std::cout << screen.ToString();
  std::cout << screen.ResetCursorPosition(/*clear=*/true);
  std::cout << std::flush;

  // Met en pause pendant une courte durée pour contrôler le taux de rafraîchissement.
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
  ftxui::Dimension::Full(),      // largeur
  ftxui::Dimension::Fixed(10)    // hauteur
);
```

Une fois créé, affichez un élément et le résultat :

```cpp
ftxui::Render(screen, element);
screen.Print();
```

---

# ftxui::Pixel

Chaque cellule de la grille de l'écran est un @ref ftxui::Pixel, qui contient :

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

auto& pixel = screen.PixelAt(3, 3);
pixel.character = U'X';
pixel.bold = true;
pixel.foreground_color = ftxui::Color::Red;
pixel.background_color = ftxui::Color::RGB(0, 255, 0);

screen.Print();
```

> [!note]
> `PixelAt(x, y)` effectue une vérification des limites et renvoie une référence au pixel
> aux coordonnées spécifiées. Si hors limites, une référence de pixel factice est
> renvoyée.


Chaque cellule de l'écran est un @ref ftxui::Pixel. Vous pouvez les modifier en utilisant :

```cpp
auto& pixel = screen.PixelAt(x, y);
pixel.character = U'X';
pixel.bold = true;
pixel.foreground_color = Color::Red;
```

---

# ftxui::Color

La classe @ref ftxui::Color est utilisée pour définir les couleurs de premier plan et d'arrière-plan pour chaque @ref ftxui::Pixel.

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
> Vous pouvez interroger la capacité du terminal en utilisant @ref ftxui::Terminal::ColorSupport();
>
> Ceci peut être défini manuellement en utilisant @ref ftxui::Terminal::SetColorSupport().
