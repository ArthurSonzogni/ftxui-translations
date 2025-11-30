Registro de cambios
==================

Próximo
======

### Doc
- Arreglar salida de Doxygen rota. Ver @markmandel en #1029.
- Usar Doxygen awesome. Añadir nuestro propio tema.
- Dividir la documentación en varias páginas.

### Build
- Característica: Soporte para módulos C++20. 
  Esto requiere:
  - Usar el generador Ninja o MSVC
  - Un compilador Clang/GCC/MSVC reciente.
  - Cmake 3.28 o superior.
  Uso:
  ```cpp
  import ftxui;
  import ftxui.component;
  import ftxui.dom;
  import ftxui.screen;
  import ftxui.util;
  ```
  Gracias @mikomikotaishi por el PR #1015.
- Eliminar la dependencia de 'pthread'.

### Component
- Característica: Manejo de entrada POSIX con tuberías.
  - Permite a las aplicaciones FTXUI leer datos de stdin (cuando se usa tubería) mientras aún reciben entrada de teclado del terminal.
  - Habilitado por defecto.
  - Puede deshabilitarse usando `ScreenInteractive::HandlePipedInput(false)`.
  - Solo disponible en Linux y macOS.
  Gracias @HarryPehkonen por el PR #1094.
- Arreglar que la pantalla `ScreenInteractive::FixedSize` pisa la salida del terminal precedente. Gracias @zozowell en #1064.
- Arreglar `ftxui::Slider` vertical. La tecla "arriba" previamente disminuía el
  valor. Gracias @its-pablo en #1093 por reportar el problema.

### Dom
- Arreglar desbordamiento de enteros en `ComputeShrinkHard`. Gracias @its-pablo en #1137 por
  reportar y arreglar el problema.
- Añadir especialización para `vbox/hbox/dbox` para permitir un contenedor de Element como
  entrada. Gracias @nbusser en #1117.

6.1.9 (2025-05-07)
------------

### Build
Si todo va bien (pendiente), ftxui debería aparecer en el repositorio central de Bazel.
Puede importarse a su proyecto usando las siguientes líneas:

**MODULE.bazel**
```bazel
bazel_dep(name = "ftxui", version = "6.1.9")
```

Gracias @robinlinden y @kcc por las revisiones.

### dom
- Corrección de errores: Restaurar el comportamiento de `dbox` de ftxui 5.0.0. Para aplicar la mezcla de bgcolor entre las dos capas, se agregará un nuevo `dboxBlend`.

6.1.8 (2025-05-01)
------------------

### Build
- Característica: Soporte para el sistema de construcción `bazel`. Ver #1032.
  Propuesto por Kostya Serebryany @kcc

  **BUILD.bazel**
  ```bazel
  deps = [
    // Depende de toda la librería:
    "@ftxui//:ftxui",

    // Elige un submódulo específico:
    "@ftxui//:component",
    "@ftxui//:dom",
    "@ftxui//:screen",
  ]
  ```

### Component
- Corrección de errores: Arreglar un fallo con ResizeableSplit. Ver #1023.
  - Limitar el tamaño de la pantalla al tamaño del terminal.
  - Deshabilitar `ResizeableSplit` con tamaño negativo.

### Dom
- Corrección de errores: Deshabilitar la especificación de una restricción de tamaño negativo. Ver #1023.


6.0.2 (2025-03-30)
-----

### Component
- Corrección de errores: Arreglar un fallo importante en Windows que afectaba a todos los componentes. Ver #1020
- Corrección de errores: Arreglar focusRelative.

6.0.1 (2025-03-28)
-----

Igual que v6.0.0.

Debido a un problema, la etiqueta v6.0.0 fue reemplazada. Esto no es una buena práctica y afecta
a los desarrolladores que comenzaron a usarla en un corto período de tiempo. Enviar una nueva
versión con el mismo contenido es la mejor manera de arreglar esto.

Ver #1017 y #1019.

6.0.0 (2025-03-23)
-----

### Component
- Característica: Añadir soporte para entrada sin procesar. Permitiendo detectar más teclas.
- Característica: Añadir `ScreenInteractive::ForceHandleCtrlC(false)` para permitir que el componente
  anule completamente el manejador `Ctrl+C` predeterminado.
- Característica: Añadir `ScreenInteractive::ForceHandleCtrlZ(false)` para permitir que el componente
  anule completamente el manejador `Ctrl+Z` predeterminado.
- Característica: Añadir eventos `Mouse::WeelLeft` y `Mouse::WeelRight` en terminales compatibles.
- Característica: Añadir `Event::DebugString()`.
- Característica: Añadir soporte para el modo de inserción de `Input`. Añadir la opción `InputOption::insert`. Añadido por @mingsheng13.
- Característica: Añadir `DropdownOption` para configurar el desplegable. Ver #826.
- Característica: Añadir soporte para Selección. Gracias @clement-roblot. Ver #926.
  - Ver `ScreenInteractive::GetSelection()`.
  - Ver el oyente `ScreenInteractive::SelectionChange(...)`.
- Corrección de errores/Cambio importante: `Transición del ratón`:
  - Detectar cuando el ratón se mueve, en lugar de cuando se presiona.
    Se añadió el movimiento `Mouse::Moved`.
  - Arrastrar el ratón con el botón izquierdo presionado ahora evita activar
    múltiples casillas de verificación.
  - Un par de componentes ahora se activan cuando se presiona el ratón,
    en lugar de cuando se suelta.
  Esto arregla: https://github.com/ArthurSonzogni/FTXUI/issues/773
  Esto arregla: https://github.com/ArthurSonzogni/FTXUI/issues/792
- Corrección de errores: `mouse.control` ahora se reporta correctamente.
- Característica: Añadir `ScreenInteractive::FullscreenPrimaryScreen()`. Esto permite
  mostrar un componente de pantalla completa en la pantalla principal, en lugar de la
  pantalla alternativa.
- Corrección de errores: `Input` `onchange` no se llamaba al presionar la tecla de retroceso o suprimir.
  Arreglado por @chrysante en chrysante en PR #776.
- Corrección de errores: Restaurar correctamente la forma del cursor al salir. Ver #792.
- Corrección de errores: Arreglar la posición del cursor cuando está en la última columna. Ver #831.
- Corrección de errores: Arreglar la navegación del teclado de `ResizeableSplit`. Arreglado por #842.
- Corrección de errores: Arreglar el enfoque del `Menu`. Ver #841
- Característica: Añadir `ComponentBase::Index()`. Esto permite obtener el índice de un
  componente en su padre. Ver #932
- Característica: Añadir `EntryState::index`. Esto permite obtener el índice de una entrada de menú.
  Ver #932
- Característica: Añadir `SliderOption::on_change`. Esto permite establecer una devolución de llamada cuando el
  valor del deslizador cambia. Ver #938.
- Corrección de errores: Manejar `Dropdown` sin entradas.
- Corrección de errores: Arreglar un fallo en `LinearGradient` debido a la precisión flotante y un
          error de uno menos. Ver #998.

### Dom
- Característica: Añadir decorador `italic`. Por ejemplo:
  ```cpp
  auto italic_text = text("Italic text") | italic;
  ```
  ```cpp
  auto italic_text = italic(text("Italic text"));
  ```
  Propuesto por @kenReneris en #1009.
- Característica: Añadir `hscroll_indicator`. Muestra un indicador horizontal
  que refleja la posición actual de desplazamiento. Propuesto por @ibrahimnasson en
  [issue 752](https://github.com/ArthurSonzogni/FTXUI/issues/752)
- Característica: Añadir la opción `extend_beyond_screen` a `Dimension::Fit(..)`, permitiendo
  que el elemento sea más grande que la pantalla. Propuesto por @LordWhiro. Ver #572 y
  #949.
- Característica: Añadir soporte para Selección. Gracias @clement-roblot. Ver #926.
  - Ver el decorador `selectionColor`.
  - Ver el decorador `selectionBackgroundColor`.
  - Ver el decorador `selectionForegroundColor`.
  - Ver el decorador `selectionStyle(style)`.
  - Ver el decorador `selectionStyleReset`.
- Cambio importante: Cambiar cómo se manejan "focus"/"select". Esto corrige el
  comportamiento.
- Cambio importante: `Component::OnRender()` se convierte en el método a anular para
  renderizar un componente. Esto reemplaza `Component::Render()` que todavía se usa
  para llamar al método de renderizado en los hijos. Este cambio permite arreglar un
  par de problemas relacionados con el manejo del enfoque.

### Screen
- Característica: Añadir `Box::IsEmpty()`.
- Característica: Transparencia de color
    - Añadir `Color::RGBA(r,g,b,a)`.
    - Añadir `Color::HSVA(r,g,b,a)`.
    - Añadir `Color::Blend(Color)`.
    - Añadir `Color::IsOpaque()`

### Util
- Característica: Soporte para `Adapter` arbitrario para `ConstStringListRef`. Ver #843.

### Build
- Soporte para las compilaciones "unity/jumbo" de cmake. Arreglado por @ClausKlein.

5.0.0
-----

### Component
- Rompimiento: El enum MenuDirection se renombra a Direction
- Rompimiento: El enum GaugeDirection se renombra a Direction
- Rompimiento: El enum Direction se renombra a WidthOrHeight
- Rompimiento: Eliminar el constructor de copia/asignación de `ComponentBase`.
- Rompimiento: MenuOption::entries se renombra a MenuOption::entries_option.
- Rompimiento: `Ref<{Component}Option>` se convierte en `{Component}Option` en los constructores de componentes.
- Característica: `ResizeableSplit` ahora soporta elementos arbitrarios como separador.
- Característica: `input` ahora soporta múltiples líneas.
- Característica: El estilo de `input` ahora es personalizable.
- Corrección de errores: Soporte F1-F5 desde el terminal del sistema operativo.
- Característica: Añadir constructor basado en struct:
  ```cpp
  Component Button(ButtonOption options);
  Component Checkbox(CheckboxOption options);
  Component Input(InputOption options);
  Component Menu(MenuOption options);
  Component MenuEntry(MenuEntryOption options);
  Component Radiobox(RadioboxOption options);
  Component Slider(SliderOption<T> options);
  Component ResizableSplit(ResizableSplitOption options);
  ```
- Característica: Añadir `ScreenInteractive::TrackMouse(false)` para deshabilitar el soporte del ratón.

### Dom
- Característica: Añadir decorador `hyperlink`. Por ejemplo:
  ```cpp
  auto link = text("Click here") | hyperlink("https://github.com/FTXUI")
  ```
  Ver la [página OSC 8](https://gist.github.com/egmontkob/eb114294efbcd5adb1944c9f3cb5feda).
  Soporte FTXUI propuesto por @aaleino en [#662](https://github.com/ArthurSonzogni/FTXUI/issues/662).

### Screen
- Rompimiento: `WordBreakProperty` se convierte en un enum `uint8_t`. Esto produce una mejora de rendimiento del 0.8%.
- Rompimiento: Eliminar el constructor de Pixel definido por el usuario y el operador de igualdad.
- Rendimiento: 19% más rápido en los benchmarks.


### Build
- Comprobar la compatibilidad de la versión al usar cmake find_package().
- Añadir opciones `FTXUI_DEV_WARNING` para activar las advertencias al compilar FTXUI.
- Desactivar por defecto `FTXUI_BUILD_DOCS`.
- Desactivar por defecto `FTXUI_BUILD_EXAMPLE`.

4.1.1
-----

### Component
- Arreglar: Soporte para teclas de flecha en modo aplicación.
- Arreglar: Eliminar nueva línea inútil al usar una pantalla alternativa.

### Dom
- Característica: Añadir el estilo de guiones para borde y separador:
  - Ver el enum `DASHED` y las funciones `separatorDashed()`, `borderDashed()`.
- Característica: Añadir bordes de color.
  - Ver funciones: `borderStyled(BorderStyle, Color)` y `borderStyled(Color)`.
- Característica: Añadir `LinearGradient`. Se puede usar en `color` y `bgColor`.
- Mejora: `Color::Interpolate()` usa corrección gamma.
- Arreglar: Comprobar que el área del `graph` sea positiva.

### Build/Install
- Usar CMAKE_CXX_STANDARD configurado globalmente si está configurado.
- Exponer el archivo pkg-config.
- Comprobar la compatibilidad de la versión al usar cmake find_package().

4.1.0  (Abandonado)
-----
Esta versión está abandonada y no debe usarse. Introdujo un cambio importante en la API.

4.0.0
-----

### DOM
- Característica: más estilos:
  - `strikethrough`
  - `underlinedDouble`
- Característica: Personalizar el cursor. Añadir los siguientes decoradores:
  - `focusCursorBlock`
  - `focusCursorBlockBlinking`
  - `focusCursorBar`
  - `focusCursorBarBlinking`
  - `focusCursorUnderline`
  - `focusCursorUnderlineBlinking`
- Corrección de errores: Arreglar `focus`/`select` cuando el `vbox`/`hbox`/`dbox` contiene un
  `flexbox`
- Corrección de errores: Arreglar el área seleccionada/enfocada. Solía ser 1 celda más grande/larga de lo
  solicitado.
- Corrección de errores: Reenviar el área seleccionada/enfocada del hijo en `gridbox`.
- Corrección de errores: Arreglar dimensiones calculadas incorrectas de Canvas.
- Corrección de errores: Soporte para `vscroll_indicator` con un tamaño interno cero.
- Corrección de errores: Arreglar `vscroll_indicator` que oculta la última columna.

### Component:
- Característica: Añadir el componente `Modal`.
- Característica: `Slider` ahora soporta tomar referencias para todos sus argumentos.
- Característica: `Slider` soporta `SliderOption`. Soporta:
    - múltiples direcciones.
    - múltiples colores.
    - varios valores (valor, mínimo, máximo, incremento).
- Característica: Definir `ScreenInteractive::Exit()`.
- Característica: Añadir `Loop` para dar a los desarrolladores un mejor control sobre el bucle principal. Esto
  se puede usar para integrar FTXUI en otro bucle principal, sin tomar el control total.
- Característica: `Input` soporta CTRL+Izquierda y CTRL+Derecha.
- Característica: Usar una barra parpadeante en el componente `Input`.
- Mejora: El `Menu` mantiene el enfoque cuando se selecciona una entrada con el
  ratón.
- Corrección de errores: Añadir implementación de `ButtonOption::Border()`. Faltaba.
- Corrección de errores: Proporcionar la clave correcta para F1-F4 y F11.
- Característica: Añadir los decoradores de componente `Hoverable`.

### Screen
- Característica: añadir `Box::Union(a,b) -> Box`
- Corrección de errores: Arreglar el restablecimiento de `dim` que choca con el restablecimiento de `bold`.
- Característica: Añadir soporte de redimensionamiento de pantalla emscripten.
- Corrección de errores: Añadir soporte unicode 13 para caracteres de ancho completo.
- Corrección de errores: Arreglar MSVC tratando la función `codecvt` de C++17 deprecada como un error.

### Build
- Soporte para usar la versión de google test proporcionada por el gestor de paquetes.

3.0.0
-----

### Build
- **Rompimiento**: El prefijo de la librería vuelve a ser "lib" (el predeterminado). Esto
    significa que los usuarios que no usen cmake no deben enlazar contra "libftxui-dom", por ejemplo.

### Component
- ¡Módulo de **Animaciones**! Los componentes pueden implementar el método `OnAnimation` y
  el `animation::Animator` para definir algunas propiedades animadas.
  - `Menu` ahora soporta animaciones.
  - `Button` ahora soporta animaciones.
- Soporte SIGTSTP. (ctrl+z).
- Soporte para publicación de tareas. `ScreenInteractive::Post(Task)`.
- `Menu` ahora se puede usar en las 4 direcciones, usando `MenuOption.direction`.
- `Menu` puede mostrar un subrayado animado, usando
  `MenuOption.underline.enabled`.
- `Button` ahora toma el foco en el frame.
- **Rompimiento** Todas las opciones ahora usan una función de transformación.
- **Rompimiento** El componente `Toggle` ahora se implementa usando `Menu`.
- **Corrección de errores** Container::Tab implementa `Focusable()`.
- **Corrección de errores** Mejoras en las implementaciones predeterminadas de los métodos `Focusable()` y
  `ActiveChild()` de `ComponentBase`.
- **Corrección de errores** Convertir automáticamente las teclas '\r' en '\n' para programas Linux que
  no envían el código correcto para la tecla de retorno, como el 'bind'.
  https://github.com/ArthurSonzogni/FTXUI/issues/337
- Añadir decorador para componentes:
  - `operator|(Component, ComponentDecorator)`
  - `operator|(Component, ElementDecorator)`
  - `operator|=(Component, ComponentDecorator)`
  - `operator|=(Component, ElementDecorator)`
  - Añadir el decorador `Maybe`.
  - Añadir el decorador `CatchEvent`.
  - Añadir el decorador `Renderer`.
- **Rompimiento** eliminar el encabezado "deprectated.hpp" y el soporte de entrada para cadenas anchas.

### DOM:
- **Rompimiento**: El decorador `inverted` ahora cambia el atributo `inverted`.
- Añadir `gauge` para las 4 direcciones. Exponer la siguiente API:
```cpp
Element gauge(float ratio);
Element gaugeLeft(float ratio);
Element gaugeRight(float ratio);
Element gaugeUp(float ratio);
Element gaugeDown(float ratio);
Element gaugeDirection(float ratio, GaugeDirection);
```
- Añadir elementos `separatorHSelector` y `separatorVSelector`. Esto se puede usar
  para resaltar un área.
- Añadir el decorador `automerge`. Esto hace que los caracteres separadores se fusionen
  con otros cercanos.
- Arreglar la función de renderizado de `Table`, para permitir la fusión automática de caracteres.
- **Corrección de errores**: El `vscroll_indicator` ahora calcula su desplazamiento y tamaño
  correctamente.
- Añadir el `operator|=(Element, Decorator)`

### Screen:
- Añadir: `Color::Interpolate(lambda, color_a, color_b)`.

2.0.0
-----

### Características:

#### Screen
- Añadir `automerge` al campo de bits Pixel. Esto ahora controla qué píxeles se
  fusionan automáticamente.

#### DOM:
- Añadir la clase `Canvas` y la función `ElementFrom('canvas')`. Juntos, los usuarios de
  la librería pueden dibujar usando caracteres braille y de bloque.
- Soporte para elementos dom `flexbox`. Esto se construye simétricamente al HTML.
  Todos los siguientes atributos son compatibles: `direction`, `wrap`, `justify-content`,
  `align-items`, `align-content`, `gap`
- Añadir los elementos auxiliares dom basados en `flexbox`:
  - `paragraph`
  - `paragraphAlignLeft`
  - `paragraphAlignCenter`
  - `paragraphAlignRight`
  - `paragraphAlignJustify`
- Añadir los elementos auxiliares basados en `flexbox`: `hflow()`, `vflow()`.
- Añadir: `focusPositionRelative` y `focusPosition`
- Añadir constructor `Table` desde vector 2D de Element, en lugar de string.

#### Component
- Añadir el componente `collapsible`.
- Añadir `ScreenInteractive::WithRestoredIO`. Esto decora una devolución de llamada. Esto
  la ejecuta con los hooks del terminal temporalmente desinstalados. Esto es útil si
  se desea ejecutar comandos usando directamente stdin/stdout/stderr.

### Bug

#### Table
- Los separadores horizontal y vertical de la `table` ahora se expanden correctamente.

#### Component
- `Input` no debería tomar el foco al pasar el ratón por encima.
- Modificar los `Input` durante el evento on_enter/on_change ahora funciona correctamente.

### Cambios importantes:
- El comportamiento de `paragraph` ha sido modificado. Ahora devuelve un Element,
  en lugar de una lista de elementos.

0.11.1
------

# Component
- Característica: Soporte para los botones PageUp/PageDown/Home/End.
- Corrección de errores: Comprobar que el elemento seleccionado esté dentro de los límites para Dropdown.

# Build
- Corrección de errores: Empaquetar la librería usando la "configuración de Release". No la de depuración.

0.11
----

## flujo de trabajo de github
- Añadir artefactos de Windows y MacOS.
- Fusionar todos los flujos de trabajo.

## Bug
- En sistemas Unix, recurrir a la dimensión de pantalla {80,25} en caso de fallo.

## CMake
- Soporte para librería compartida, a través de la opción `BUILD_SHARED_LIBS`.
- Añadir versión de la librería y enlaces simbólicos.

0.10 (2021-09-30)
--------------------

## Bug
- Arreglar la fusión automatizada de bordes.

### Dom
- Clase `Table()` para construir tablas estilizadas.
   Ver https://github.com/ArthurSonzogni/FTXUI/discussions/228
- `vscroll_indicator`. Muestra un indicador de barra de desplazamiento a la derecha.
- `separatorEmpty`. Un separador que no dibuja nada.
- `separatorFixed`. Un separador que dibuja el carácter proporcionado.

### Component
- `Maybe`: Muestra un componente condicionalmente basado en un booleano.
- `Dropdown`: Una lista desplegable de selección.

0.9 (2021-09-26)
----------------

La versión inicial donde se escribieron los registros de cambios.

Esta versión incluye:

### screen
- Estilo:
  - Negrita.
  - Parpadeo.
  - Atenuado.
  - Invertido.
  - Subrayado.
  - Color de primer plano.
  - Color de fondo.
- Soporte para unicode UTF8.
  - Carácter de ancho completo: 测试.
  - Caracteres combinados: a⃒
- Un búfer de Stencil.
- Fusionar automáticamente los caracteres de dibujo de cajas.
- Detectar la dimensión del terminal.

### DOM

- Elemento:
  - `text` & `vtext`
  - `separator` y 5 variaciones.
  - `gauge`
  - `border` y 6 variaciones.
  - `window`
  - `spinner`
  - `paragraph` y `hflow`.

- Diseño:
  - `hbox`
  - `vbox`
  - `dbox`
  - `gridbox`
  - `frame`: Dibujar dentro de un área virtual, potencialmente más grande que la real.
  - `focus`, `select`: desplazar la vista interna de un frame, para que esté a la vista.
  - `flex` y 8 variaciones. `filler`
  
- Decoradores:
  - `bold`
  - `dim`
  - `inverted`
  - `blink`
  - `color`
  - `bgcolor`
  - `clearunder`

### Component

- Contenedor:
  - `Container::Vertical`
  - `Container::Horizontal`
  - `Container::Tab`
- `Button`
- `Checkbox`
- `Input`
- `Menu`
- `MenuEntry`
- `Radiobox`
- `Toggle`
- `Slider`
- `Renderer` & variaciones
- `CatchEvent`

### MISC

- Fuzzer
- Pruebas usando gtest.
- Documentación Doxygen
- IWYU
- 52 ejemplos.
- Soporte para WebAssembly.
- Soporte para Windows y fallback para terminales rotos.