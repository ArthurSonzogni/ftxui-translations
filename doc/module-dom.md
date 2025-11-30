@page module-dom ftxui / dom
@tableofcontents

![title-img](https://nsm09.casimages.com/img/2025/05/31//2505310207423242518595347.png)

Este módulo define un conjunto jerárquico de `ftxui::Element`. Un elemento gestiona
el diseño y puede responder a los cambios de dimensión del terminal. Observe el
siguiente ejemplo donde este módulo se utiliza para crear un diseño simple con
una serie de operadores:

La sección @subpage module-dom-examples proporciona una colección de ejemplos.

**Ejemplo:**
```cpp
namespace ftxui {
    ...

// Definir el documento
Element document = vbox({
  text("La ventana") | bold | color(Color::Blue),
  gauge(0.5)
  text("El pie de página")
});

// Añadir un borde, llamando a la función decoradora `ftxui::border`.
document = border(document);

// Añadir otro borde, usando el operador pipe.
document = document | border.

// Añadir otro borde, usando el operador |=.
document |= border

...
}
```

**Lista de elementos**

La lista de todos los elementos está incluida y se puede acceder a ella incluyendo el
archivo de cabecera correspondiente:
```cpp
#include <ftxui/dom/elements.hpp>
```

\include{strip} "ftxui/dom/elements.hpp"

# texto # {#dom-text}

El widget más simple. Muestra un texto.
```cpp
text("Soy un trozo de texto");
```
```bash
Soy un trozo de texto.
```

# vtext {#dom-vtext}

Código:
```cpp
vtext("HELLO");
```

Salida del terminal:
```bash
H
E
L
L
O
```

# párrafo {#dom-paragraph}

Similar a `ftxui::text`, pero las palabras individuales se ajustan a lo largo de múltiples
líneas, dependiendo del ancho de su contenedor.

Código de ejemplo:
```cpp
paragraph("Un texto muy largo")
```

![ezgif com-gif-maker (4)](https://user-images.githubusercontent.com/4759106/147251370-983a06e7-6f41-4113-92b8-942f43d34d06.gif)

Para un ejemplo más detallado, consulte [ejemplo detallado](https://arthursonzogni.github.io/FTXUI/examples_2dom_2paragraph_8cpp-example.html). Párrafo también incluye una serie de otras variantes como se muestra a continuación:
```cpp
namespace ftxui {
    Element paragraph(std::string text);
    Element paragraphAlignLeft(std::string text);
    Element paragraphAlignRight(std::string text);
    Element paragraphAlignCenter(std::string text);
    Element paragraphAlignJustify(std::string text);
}
```


# borde {#dom-border}

Añade un borde alrededor de un elemento.

Código:
```cpp
border(text("El elemento"))
```

Salida del terminal:
```bash
┌───────────┐
│El elemento│
└───────────┘
```

> [!nota]
> Puedes lograr el mismo comportamiento usando el operador pipe.
> 
> Código:
> ```cpp
> text("El elemento") | border
> ```

El borde también viene en una variedad de estilos como se muestra a continuación:
```cpp
namespace ftxui {
    Element border(Element);
    Element borderLight(Element);
    Element borderHeavy(Element);
    Element borderDouble(Element);
    Element borderRounded(Element);
    Element borderEmpty(Element);
    Decorator borderStyled(BorderStyle);
    Decorator borderWith(Pixel);
}
```


# ventana # {#dom-window}

Una `ftxui::window` es un `ftxui::border`, pero con una cabecera adicional. Para añadir una
ventana alrededor de un elemento, envuélvalo y especifique una cadena como cabecera.
Código:
```cpp
window("La ventana", text("El elemento"))
```

Salida del terminal:
```bash
┌La ventana─┐
│El elemento│
└───────────┘
```

# separador {#dom-separator}

Muestra una línea vertical/horizontal para dividir visualmente el contenido de un
contenedor en dos.

Código:
```cpp
border(
  hbox({
    text("Izquierda"), 
    separator(),
    text("Derecha")
  })
)
```

Salida del terminal:
```bash
┌────┬─────┐
│izquierda│derecha│
└────┴─────┘
```


Los separadores vienen en una variedad de estilos como se muestra a continuación:
```cpp
namespace ftxui {
    Element separator(void);
    Element separatorLight();
    Element separatorHeavy();
    Element separatorDouble();
    Element separatorEmpty();
    Element separatorStyled(BorderStyle);
    Element separator(Pixel);
    Element separatorCharacter(std::string);
    Element separatorHSelector(float left,
                               float right,
                               Color background,
                               Color foreground);
    Element separatorVSelector(float up,
                               float down,
                               Color background,
                               Color foreground);
}
```

# medidor {#dom-gauge}

Este es un elemento visual que representa una proporción de progreso.

Código:
```cpp
border(gauge(0.5))
```

Salida del terminal:
```bash
┌────────────────────────────────────────────────────────────────────────────┐
│██████████████████████████████████████                                      │
└────────────────────────────────────────────────────────────────────────────┘
```

Los medidores pueden mostrarse en muchas orientaciones como se muestra a continuación:
```cpp
namespace {
    Element gauge(float ratio);
    Element gaugeLeft(float ratio);
    Element gaugeRight(float ratio);
    Element gaugeUp(float ratio);
    Element gaugeDown(float ratio);
    Element gaugeDirection(float ratio, GaugeDirection);
}
```

# gráfico {#dom-graph}

@htmlonly
<script id="asciicast-223726" src="https://asciinema.org/a/223726.js" async></script>
@endhtmlonly

See:
```cpp
Element graph(GraphFunction);
```

# Colores {#dom-colors}

La mayoría de las consolas de terminal pueden mostrar texto y fondos de colores. FTXUI
soporta todas las paletas de colores:
```cpp
Decorator color(Color);
Decorator bgcolor(Color);
```


[Galería](https://arthursonzogni.github.io/FTXUI/examples_2dom_2color_gallery_8cpp-example.html) de colores:
![image](https://user-images.githubusercontent.com/4759106/147248595-04c7245a-5b85-4544-809d-a5984fc6f9e7.png)

## Paleta16 #{#dom-colors-palette-16}

En la mayoría de los terminales se admiten los siguientes colores:
- Default

- Black
- GrayDark
- GrayLight

- White

- Blue
- BlueLight

- Cyan
- CyanLight

- Green
- GreenLight

- Magenta
- MagentaLight

- Red
- RedLight

- Yellow
- YellowLight

Ejemplo de uso de los colores anteriores usando el operador pipe:
```cpp
text("Primer plano azul") | color(Color::Blue);
text("Fondo azul") | bgcolor(Color::Blue);
text("Negro sobre blanco") | color(Color::Black) | bgcolor(Color::White);
```

## Paleta256 #{#dom-colors-palette-256}

En terminales que soportan 256 colores. 
@htmlonly
<script id="asciicast-OAUc3n6QrkmrLt7XEEb8AzbLt" src="https://asciinema.org/a/OAUc3n6QrkmrLt7XEEb8AzbLt.js" async></script>
@endhtmlonly

```cpp
text("HotPink") | color(Color::HotPink);
```

## ColorVerdadero #{#dom-colors-true-color}

En terminales que soportan TrueColor, puedes usar directamente el espacio de color RGB de 24 bits:

Usa los constructores a continuación para especificar los valores **RGB** o **HSV** para tu
color:

Hay dos constructores:
```cpp
ftxui::Color::RGB(uint8_t red, uint8_t green, uint8_t blue);
ftxui::Color::HSV(uint8_t hue, uint8_t saturation, uint8_t value);
```

@htmlonly
<script id="asciicast-dk5r8IcCH0aFIIgWG0keSEHMG" src="https://asciinema.org/a/dk5r8IcCH0aFIIgWG0keSEHMG.js" async></script>
<script id="asciicast-xwzzghmqcqzIuyLwCpQFEqbEu" src="https://asciinema.org/a/xwzzghmqcqzIuyLwCpQFEqbEu.js" async></script>
@endhtmlonly

# DegradadoLineal {#dom-linear-gradient}

FTXUI soporta degradado lineal. Ya sea en primer plano o en segundo plano.

```cpp
Decorator color(const LinearGradient&);
Decorator bgcolor(const LinearGradient&);
```

Un `ftxui::LinearGradient` se define por un ángulo en grados y una lista de paradas de color.
```cpp
auto gradient = LinearGradient()
  .Angle(45)
  .AddStop(0.0, Color::Red)
  .AddStop(0.5, Color::Green)
  .AddStop(1.0, Color::Blue);
```

También puedes usar constructores simplificados:
```cpp
LinearGradient(Color::Red, Color::Blue);
```
```cpp
LinearGradient(45, Color::Red, Color::Blue);
```

Ver [demostración](https://arthursonzogni.github.io/FTXUI/examples/?file=component/linear_gradient_gallery).


# Estilo {#dom-style}
Además de texto y fondos de colores. Muchos terminales soportan efectos de texto como: `bold`, `italic`, `dim`, `underlined`, `inverted`, `blink`.

```cpp
Element bold(Element);
Element italic(Element);
Element dim(Element);
Element inverted(Element);
Element underlined(Element);
Element underlinedDouble(Element);
Element strikethrough(Element);
Element blink(Element);
Decorator color(Color);
Decorator bgcolor(Color);
Decorator colorgrad(LinearGradient);
Decorator bgcolorgrad(LinearGradient);
```

[Example](https://arthursonzogni.github.io/FTXUI/examples_2dom_2style_gallery_8cpp-example.html)

![image](https://user-images.githubusercontent.com/4759106/147244118-380bf834-9e33-40df-9ff0-07c10f2598ef.png)

Para usar estos efectos, simplemente envuelve tus elementos con el efecto deseado:
```cpp
underlined(bold(text("Este texto está en negrita y subrayado")))
```

Alternativamente, usa el operador pipe para encadenarlo en tu elemento:
```cpp
text("Este texto está en negrita") | bold | underlined
```

# Diseño {#dom-layout}

Permite que los elementos se organicen de las siguientes maneras:
  - **Horizontalmente** con `ftxui::hbox`
  - **Verticalmente** con `ftxui::vbox`
  - **Dentro de una cuadrícula** con `ftxui::gridbox`
  - **Envuelto en una dirección** usando `ftxui::flexbox`.
  
[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2dom_2vbox_hbox_8cpp-example.html) usando `ftxui::hbox`, `ftxui::vbox` y `ftxui::filler`.

![image](https://user-images.githubusercontent.com/4759106/147242524-7103b5d9-1a92-4e2d-ac70-b3d6740061e3.png)
  
  
[Ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2dom_2gridbox_8cpp-example.htmlp)
usando `ftxui::gridbox`:

![image](https://user-images.githubusercontent.com/4759106/147242972-0db1f2e9-0790-496f-86e6-ed2c604f7a73.png)

[Ejemplo](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/hflow.cpp)
usando flexbox:

![image](https://user-images.githubusercontent.com/4759106/147243064-780ac7cc-605b-475f-94b8-cf7c4aed03a5.png)

Consulta este
[ejemplo](https://arthursonzogni.github.io/FTXUI/examples_2dom_2hflow_8cpp-example.html)
y la [demostración](https://arthursonzogni.github.io/FTXUI/examples/?file=component/flexbox) asociada.

El elemento también puede volverse flexible usando el decorador `ftxui::flex`.

Código:
```cpp
  hbox({
    text("izquierda") | border ,
    text("medio") | border | flex,
    text("derecha") | border,
  });
          ```
          Salida del terminal:
          ```bash
          ┌────┐┌─────────────────────────────────────────────────────┐┌─────┐
          │izquierda││medio                                                ││derecha│
          └────┘└─────────────────────────────────────────────────────┘└─────┘
          ```
          
          Código:
```cpp
  hbox({
    text("izquierda") | border ,
    text("medio") | border | flex,
    text("derecha") | border | flex,
  });
    ```
    
    Salida del terminal:
```bash
┌────┐┌───────────────────────────────┐┌───────────────────────────────┐
│izquierda││medio                          ││derecha                        │
└────┘└───────────────────────────────┘└───────────────────────────────┘
```

# Tabla {#dom-table}

Permite un formato sencillo de datos en una tabla visual ordenada.

[Ejemplo de código](https://arthursonzogni.github.io/FTXUI/examples_2dom_2table_8cpp-example.html):
  
![image](https://user-images.githubusercontent.com/4759106/147250766-77d8ec9e-cf2b-486d-9866-1fd9f1bd2e6b.png)

# Lienzo {#dom-canvas}

Ver la API [<ftxui/dom/canvas.hpp>](./canvas_8hpp_source.html)

```cpp
  auto c = Canvas(100, 100);
  c.DrawPointLine(10, 10, 80, 10, Color::Red);
  auto element = canvas(c);
```

El dibujo se puede realizar en un `ftxui::Canvas`, usando caracteres braille, de bloque o simples:
  
[Ejemplo](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/dom/canvas.cpp) simple:
  
![image](https://user-images.githubusercontent.com/4759106/147245843-76cc62fb-ccb4-421b-aacf-939f9afb42fe.png)

[Ejemplo](https://github.com/ArthurSonzogni/FTXUI/blob/master/examples/component/canvas_animated.cpp) complejo:
  
![ezgif com-gif-maker (3)](https://user-images.githubusercontent.com/4759106/147250538-783a8246-98e0-4a25-b032-3bd3710549d1.gif)
