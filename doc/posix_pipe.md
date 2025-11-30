# Entrada canalizada POSIX en FTXUI

> [!WARNING]
> Esta característica funciona solo en Linux y macOS. No es compatible con
> Windows y WebAssembly.

## ¿Qué es una tubería POSIX?

Una tubería POSIX es una forma en que dos programas separados pueden comunicarse. Un programa envía su salida directamente como entrada a otro programa. Piense en ello como un tubo unidireccional para datos.

**Ejemplo:**

Imagine que desea listar archivos y luego filtrarlos interactivamente.

- `ls`: Lista archivos.
- `interactive_grep`: Una aplicación FTXUI que filtra texto y le permite escribir.

Puede conectarlos con una tubería (`|`):

```bash
ls -l | interactive_grep
```

Esto es lo que sucede:
1. `ls -l` lista archivos con detalles.
2. El `|` envía esta lista directamente a `interactive_grep`.
3. `interactive_grep` recibe la lista y la muestra. Debido a que es una aplicación FTXUI, puede escribir para filtrar la lista, aunque recibió datos iniciales de `ls`.

## Cómo FTXUI maneja la entrada canalizada

Ahora que comprende qué es una tubería POSIX, veamos cómo las usa FTXUI.

FTXUI permite que su aplicación lea datos de otros programas (como de una tubería) sin dejar de permitirle usar su teclado para la interacción. Esto es útil para herramientas interactivas de línea de comandos que procesan datos.

Normalmente, las aplicaciones FTXUI reciben toda la entrada de `stdin`. Sin embargo, cuando FTXUI detecta que `stdin` está conectado a la salida de una tubería (lo que significa que los datos se están canalizando a su aplicación), cambia automáticamente a leer la entrada interactiva del teclado desde `/dev/tty`. Esto asegura que su aplicación aún pueda recibir la entrada del usuario incluso mientras procesa datos canalizados.

Esta característica está **activada por defecto**.

Si su aplicación FTXUI necesita leer datos canalizados y también responder a la entrada del teclado, normalmente no necesita hacer nada especial:

```cpp
auto screen = ScreenInteractive::Fullscreen();
// screen.HandlePipedInput(true); // Esto está habilitado por defecto
screen.Loop(component);
```

## Desactivación de la entrada canalizada

Si no necesita esta característica, o si entra en conflicto con su manejo de entrada personalizado, puede desactivarla.

Para deshabilitarla, llame a `HandlePipedInput(false)` antes de iniciar el bucle principal de su aplicación:

```cpp
auto screen = ScreenInteractive::Fullscreen();
screen.HandlePipedInput(false); // Desactiva el manejo de entrada canalizada
screen.Loop(component);
```