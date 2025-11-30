@page module-screen ftxui / screen
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595348.png)

El módulo `ftxui::screen` es la base de bajo nivel. Puede utilizarse
de forma independiente, pero está diseñado principalmente para ser utilizado junto con los
módulos [ftxui::dom](module-dom.html) y [ftxui::component](module-component.html).

Proporciona una @ref ftxui::Screen.

---

# ftxui::Screen

La clase @ref ftxui::Screen representa una cuadrícula 2D de caracteres con estilo que puede
renderizarse en un terminal.  
Proporciona métodos para crear una pantalla, acceder a los píxeles y renderizar elementos.

Puedes acceder a las celdas individuales (@ref ftxui::Pixel) de la pantalla usando 
el método @ref ftxui::Screen::PixelAt, que devuelve una referencia
al píxel en las coordenadas especificadas.

**Example**
```cpp
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>

void main() {
    auto screen = ftxui::Screen::Create(
        ftxui::Dimension::Full(),   // Usar todo el ancho del terminal
        ftxui::Dimension::Fixed(10) // Altura fija de 10 filas
    );

    // Acceder a un píxel específico en (10, 5)
    auto& pixel = screen.PixelAt(10, 5);

    // Establecer propiedades del píxel.
    pixel.character = U'X';
    pixel.foreground_color = ftxui::Color::Red;
    pixel.background_color = ftxui::Color::RGB(0, 255, 0);
    pixel.bold = true; // Establecer estilo en negrita
    screen.Print(); // Imprimir la pantalla en el terminal
}
```

> [!note]
> Si las coordenadas están fuera de los límites, se devuelve un píxel ficticio.

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

Ten en cuenta que puedes restablecer la posición del cursor a la esquina superior izquierda de la
pantalla después de imprimir, llamando a @ref ftxui::Screen::ResetCursorPosition().

**Example**
```cpp
auto screen = ...;
while(true) {
  // Operaciones de dibujo:
  ...
  
  // Imprimir la pantalla en el terminal. Luego restablecer la posición del cursor y el
  // contenido de la pantalla.
  std::cout << screen.ToString();
  std::cout << screen.ResetCursorPosition(/*clear=*/true);
  std::cout << std::flush;

  // Dormir por un corto período para controlar la frecuencia de actualización.
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
  ftxui::Dimension::Full(),      // ancho
  ftxui::Dimension::Fixed(10)    // alto
);
```

Una vez creado, renderiza un elemento y muestra el resultado:

```cpp
ftxui::Render(screen, element);
screen.Print();
```

---

# ftxui::Pixel

Cada celda en la cuadrícula de la pantalla es un @ref ftxui::Pixel, que contiene:

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

auto& pixel = screen.PixelAt(3, 3);
pixel.character = U'X';
pixel.bold = true;
pixel.foreground_color = ftxui::Color::Red;
pixel.background_color = ftxui::Color::RGB(0, 255, 0);

screen.Print();
```

> [!note]
> `PixelAt(x, y)` realiza una comprobación de límites y devuelve una referencia al píxel
> en las coordenadas especificadas. Si está fuera de los límites, se devuelve una referencia a un píxel ficticio.


Cada celda en la pantalla es un @ref ftxui::Pixel. Puedes modificarlas usando:

```cpp
auto& pixel = screen.PixelAt(x, y);
pixel.character = U'X';
pixel.bold = true;
pixel.foreground_color = Color::Red;
```

---

# ftxui::Color

La clase @ref ftxui::Color se utiliza para definir los colores de primer plano y fondo para cada @ref ftxui::Pixel.

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
> Puedes consultar la capacidad del terminal usando @ref ftxui::Terminal::ColorSupport();
>
> Esto puede configurarse manualmente usando @ref ftxui::Terminal::SetColorSupport().
