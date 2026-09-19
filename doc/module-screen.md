@page module-screen ftxui / screen
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595348.png)

El módulo `ftxui::screen` es la base de bajo nivel. Puede utilizarse
de forma independiente, pero está diseñado principalmente para ser utilizado junto con los
módulos [ftxui::dom](module-dom.html) y [ftxui::component](module-component.html).

Proporciona una @ref ftxui::Screen.

---

# ftxui::Screen

La clase @ref ftxui::Screen representa una cuadrícula 2D de caracteres con estilo que se
pueden renderizar en una terminal.  
Proporciona métodos para crear una pantalla, acceder a celdas y renderizar elementos.

Puede acceder a las celdas individuales (@ref ftxui::Cell) de la pantalla usando
el método @ref ftxui::Screen::CellAt, que devuelve una referencia
a la celda en las coordenadas especificadas.

**Ejemplo**
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
> Si las coordenadas están fuera de los límites, se devuelve una celda ficticia.

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

Tenga en cuenta que puede restablecer la posición del cursor a la esquina superior izquierda de la
pantalla después de imprimir llamando a @ref ftxui::Screen::ResetPosition().

**Ejemplo**
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

Cada celda en la cuadrícula de la pantalla es un @ref ftxui::Cell, que contiene:

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
> `CellAt(x, y)` realiza una verificación de límites y devuelve una referencia a la celda
> en la coordenada especificada. Si está fuera de los límites, se devuelve una referencia
> a una celda ficticia.


Cada celda en la pantalla es un @ref ftxui::Cell. Puede modificarlas usando:

```cpp
auto& cell = screen.CellAt(x, y);
cell.character = "X";
cell.bold = true;
cell.foreground_color = Color::Red;
```

---

# ftxui::Color

La clase @ref ftxui::Color se usa para definir los colores de primer plano y de fondo de cada @ref ftxui::Cell.

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
> Puede consultar la capacidad de la terminal usando @ref ftxui::Terminal::ColorSupport();
>
> Esto se puede establecer manualmente usando @ref ftxui::Terminal::SetColorSupport().
