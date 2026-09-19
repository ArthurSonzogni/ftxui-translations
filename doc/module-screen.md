@page module-screen ftxui / screen
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595348.png)

El módulo `ftxui::screen` es la base de bajo nivel. Puede utilizarse
de forma independiente, pero está diseñado principalmente para ser utilizado junto con los
módulos [ftxui::dom](module-dom.html) y [ftxui::component](module-component.html).

Proporciona una @ref ftxui::Screen.

---

# ftxui::Screen

The @ref ftxui::Screen class represents a 2D grid of styled characters that can
be rendered to a terminal.  
It provides methods to create a screen, access cells, and render elements.

You can access the individual cells (@ref ftxui::Cell) of the screen using 
the @ref ftxui::Screen::CellAt method, which returns a reference
to the cell at the specified coordinates.

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
> If the coordinates are out of bounds, a dummy cell is returned.

La pantalla puede imprimirse en el terminal usando @ref ftxui::Screen::Print() o
convertirse a un std::string con @ref ftxui::Screen::ToString().

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

Note that you can reset the cursor position to the top-left corner of the
screen after printing by calling @ref ftxui::Screen::ResetPosition().

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

La utilidad @ref ftxui::Dimension controla el tamaño de la pantalla:

* `Dimension::Full()` — usar el ancho o alto completo del terminal
* `Dimension::Fit(element)` — ajustar el tamaño para que quepa el @ref ftxui::Element renderizado
* `Dimension::Fixed(n)` — usar exactamente `n` columnas o filas

Estos valores deben pasarse a `ftxui::Screen::Create()`.

@ref ftxui::Screen::Create() proporciona dos sobrecargas:

- `Screen::Create(Dimension)` establece tanto el ancho como la altura al mismo tipo de dimensión
- `Screen::Create(Dimension width, Dimension height)` permite un control distinto por eje

```cpp
auto screen = ftxui::Screen::Create(
  ftxui::Dimension::Full(),      // width
  ftxui::Dimension::Fixed(10)    // height
);
```

Una vez creado, renderiza un elemento y muestra el resultado:

```cpp
ftxui::Render(screen, element);
screen.Print();
```

---

# ftxui::Cell

Each cell in the screen grid is a @ref ftxui::Cell, which holds:

- Punto de código Unicode.
    - `character`
- @ref ftxui::Color:
    - `foreground_color`
    - `background_color`
- Booleanos:
    - `blink`
    - `bold`
    - `dim`
    - `italic`
    - `inverted` (intercambiar colores de primer plano y fondo)
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
> `CellAt(x, y)` performs bounds checking and returns a reference to the cell
> at the specified coordinate. If out-of-bounds, a dummy cell reference is
> returned.


Each cell in the screen is a @ref ftxui::Cell. You can modify them using:

```cpp
auto& cell = screen.CellAt(x, y);
cell.character = "X";
cell.bold = true;
cell.foreground_color = Color::Red;
```

---

# ftxui::Color

The @ref ftxui::Color class is used to define foreground and background colors for each @ref ftxui::Cell.

Soporta varios espacios de color y paletas predefinidas. FTXUI
recurrirá dinámicamente al color disponible más cercano en el terminal si el
color solicitado no es compatible con el terminal.

**Espacios de Color**

- **Predeterminado**: `ftxui::Color::Default` (color predeterminado del terminal)
- **Paleta de 16 colores** [Demo](https://arthursonzogni.github.io/FTXUI/examples/?file=dom/color_gallery):
    - `ftxui::Color::Black`,
    - `ftxui::Color::Red`,
    - ...
- **Paleta de 256 colores** [Demo](https://arthursonzogni.github.io/FTXUI/examples/?file=dom/color_palette256):
    - `ftxui::Color::Chartreuse1`,
    - `ftxui::Color::DarkViolet`,
    - ...
- **Color verdadero** (24 bits) [Demo](https://arthursonzogni.github.io/FTXUI/examples/?file=dom/color_truecolor_HSV:
    - `ftxui::Color::RGB(uint8_t red, uint8_t green, uint8_t blue)`
    - `ftxui::Color::HSV(uint8_t h, uint8_t s, uint8_t v)`.
    

> [!note]
> You can query the terminal capability using @ref ftxui::Terminal::ColorSupport();
>
> This can manually be set using @ref ftxui::Terminal::SetColorSupport().
