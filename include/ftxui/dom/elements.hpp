// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_ELEMENTS_HPP
#define FTXUI_DOM_ELEMENTS_HPP

#include <cstdint>
#include <functional>
#include <memory>

#include <string_view>
#include "ftxui/dom/canvas.hpp"
#include "ftxui/dom/direction.hpp"
#include "ftxui/dom/flexbox_config.hpp"
#include "ftxui/dom/linear_gradient.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/box.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/screen/terminal.hpp"
#include "ftxui/util/export.hpp"
#include "ftxui/util/ref.hpp"

namespace ftxui {
class Node;
using Element = std::shared_ptr<Node>;
using Elements = std::vector<Element>;
using Decorator = std::function<Element(Element)>;
using GraphFunction = std::function<std::vector<int>(int, int)>;

/// @brief BorderStyle es una enumeración que representa los diferentes estilos
/// de bordes que se pueden aplicar a los elementos en la interfaz de usuario de la terminal.
///
/// BorderStyle es una enumeración que representa los diferentes estilos de
/// bordes que se pueden aplicar a los elementos en la interfaz de usuario de la terminal.
/// Se utiliza para definir la apariencia visual de los bordes alrededor de los elementos,
/// como ventanas, marcos o separadores.
/// @ingroup dom
enum BorderStyle : uint8_t {
  LIGHT,
  DASHED,
  HEAVY,
  DOUBLE,
  ROUNDED,
  EMPTY,
};

// Encadena elementos con decoradores mediante pipe.
// Por ejemplo, las siguientes líneas son equivalentes:
// -> text("ftxui") | bold | underlined
// -> underlined(bold(text("FTXUI")))
FTXUI_EXPORT(DOM) Element operator|(Element, Decorator);
FTXUI_EXPORT(DOM) Element& operator|=(Element&, Decorator);
FTXUI_EXPORT(DOM) Elements operator|(Elements, Decorator);
FTXUI_EXPORT(DOM) Decorator operator|(Decorator, Decorator);

// --- Elemento de Interfaz ---
FTXUI_EXPORT(DOM) Element text(std::string_view text);
FTXUI_EXPORT(DOM) Element vtext(std::string_view text);
FTXUI_EXPORT(DOM) Element separator();
FTXUI_EXPORT(DOM) Element separatorLight();
FTXUI_EXPORT(DOM) Element separatorDashed();
FTXUI_EXPORT(DOM) Element separatorHeavy();
FTXUI_EXPORT(DOM) Element separatorDouble();
FTXUI_EXPORT(DOM) Element separatorEmpty();
FTXUI_EXPORT(DOM) Element separatorStyled(BorderStyle);
FTXUI_EXPORT(DOM) Element separator(Cell);
FTXUI_EXPORT(DOM) Element separatorCharacter(std::string_view);
FTXUI_EXPORT(DOM)
Element separatorHSelector(float left,
                           float right,
                           Color unselected_color,
                           Color selected_color);
FTXUI_EXPORT(DOM)
Element separatorVSelector(float up,
                           float down,
                           Color unselected_color,
                           Color selected_color);
FTXUI_EXPORT(DOM) Element gauge(float progress);
FTXUI_EXPORT(DOM) Element gaugeLeft(float progress);
FTXUI_EXPORT(DOM) Element gaugeRight(float progress);
FTXUI_EXPORT(DOM) Element gaugeUp(float progress);
FTXUI_EXPORT(DOM) Element gaugeDown(float progress);
FTXUI_EXPORT(DOM) Element gaugeDirection(float progress, Direction direction);

// charset[0] es el glifo "vacío", charset.back() es "completo"; las entradas
// intermedias sombrean la celda de borde.
FTXUI_EXPORT(DOM)
Element gaugeCharset(float progress,
                      std::vector<std::string> charset,
                      Direction direction = Direction::Right);
FTXUI_EXPORT(DOM) Element border(Element);
FTXUI_EXPORT(DOM) Element borderLight(Element);
FTXUI_EXPORT(DOM) Element borderDashed(Element);
FTXUI_EXPORT(DOM) Element borderHeavy(Element);
FTXUI_EXPORT(DOM) Element borderDouble(Element);
FTXUI_EXPORT(DOM) Element borderRounded(Element);
FTXUI_EXPORT(DOM) Element borderEmpty(Element);
FTXUI_EXPORT(DOM) Decorator borderStyled(BorderStyle);
FTXUI_EXPORT(DOM) Decorator borderStyled(BorderStyle, Color);
FTXUI_EXPORT(DOM) Decorator borderStyled(Color);
FTXUI_EXPORT(DOM) Decorator borderWith(const Cell&);
FTXUI_EXPORT(DOM)
Element window(Element title, Element content, BorderStyle border = ROUNDED);
FTXUI_EXPORT(DOM) Element spinner(int charset_index, size_t image_index);
FTXUI_EXPORT(DOM) Element paragraph(std::string_view text);
FTXUI_EXPORT(DOM) Element paragraphAlignLeft(std::string_view text);
FTXUI_EXPORT(DOM) Element paragraphAlignRight(std::string_view text);
FTXUI_EXPORT(DOM) Element paragraphAlignCenter(std::string_view text);
FTXUI_EXPORT(DOM) Element paragraphAlignJustify(std::string_view text);
FTXUI_EXPORT(DOM) Element graph(GraphFunction);
FTXUI_EXPORT(DOM) Element emptyElement();
FTXUI_EXPORT(DOM) Element canvas(ConstRef<Canvas>);
FTXUI_EXPORT(DOM)
Element canvas(int width, int height, std::function<void(Canvas&)>);
FTXUI_EXPORT(DOM) Element canvas(std::function<void(Canvas&)>);

// -- Decorador ---
FTXUI_EXPORT(DOM) Element bold(Element);
FTXUI_EXPORT(DOM) Element dim(Element);
FTXUI_EXPORT(DOM) Element italic(Element);
FTXUI_EXPORT(DOM) Element inverted(Element);
FTXUI_EXPORT(DOM) Element underlined(Element);
FTXUI_EXPORT(DOM) Element underlinedDouble(Element);
FTXUI_EXPORT(DOM) Element blink(Element);
FTXUI_EXPORT(DOM) Element strikethrough(Element);
FTXUI_EXPORT(DOM) Decorator color(Color);
FTXUI_EXPORT(DOM) Decorator bgcolor(Color);
FTXUI_EXPORT(DOM) Decorator color(const LinearGradient&);
FTXUI_EXPORT(DOM) Decorator bgcolor(const LinearGradient&);
FTXUI_EXPORT(DOM) Element color(Color, Element);
FTXUI_EXPORT(DOM) Element bgcolor(Color, Element);
FTXUI_EXPORT(DOM) Element color(const LinearGradient&, Element);
FTXUI_EXPORT(DOM) Element bgcolor(const LinearGradient&, Element);
FTXUI_EXPORT(DOM) Decorator focusPosition(int x, int y);
FTXUI_EXPORT(DOM) Decorator focusPositionRelative(float x, float y);
FTXUI_EXPORT(DOM) Element automerge(Element child);
FTXUI_EXPORT(DOM) Decorator hyperlink(std::string_view link);
FTXUI_EXPORT(DOM) Element hyperlink(std::string_view link, Element child);
FTXUI_EXPORT(DOM) Element selectionStyleReset(Element);
FTXUI_EXPORT(DOM) Decorator selectionColor(Color foreground);
FTXUI_EXPORT(DOM) Decorator selectionBackgroundColor(Color foreground);
FTXUI_EXPORT(DOM) Decorator selectionForegroundColor(Color foreground);
FTXUI_EXPORT(DOM) Decorator selectionStyle(std::function<void(Cell&)> style);

// --- Diseño ---
// Conjunto de elementos horizontales, verticales o apilados.
FTXUI_EXPORT(DOM) Element hbox(Elements);
FTXUI_EXPORT(DOM) Element vbox(Elements);
FTXUI_EXPORT(DOM) Element dbox(Elements);
FTXUI_EXPORT(DOM)
Element flexbox(Elements, FlexboxConfig config = FlexboxConfig());
FTXUI_EXPORT(DOM) Element gridbox(std::vector<Elements> lines);

FTXUI_EXPORT(DOM)
Element hflow(Elements);  // Ayudante: flexbox predeterminado con dirección de fila.
FTXUI_EXPORT(DOM)
Element vflow(Elements);  // Ayudante: flexbox predeterminado con dirección de columna.

// -- Flexibilidad ---
// Define cómo compartir el espacio restante cuando no todo se utiliza dentro de un
// contenedor.
FTXUI_EXPORT(DOM) Element flex(Element);  // Expandir/Minimizar si es posible/necesario.
FTXUI_EXPORT(DOM) Element flex_grow(Element);    // Expandir elemento si es posible.
FTXUI_EXPORT(DOM) Element flex_shrink(Element);  // Minimizar elemento si es necesario.
FTXUI_EXPORT(DOM) Decorator flex_factor(int grow, int shrink);
FTXUI_EXPORT(DOM) Decorator flex_grow_factor(int grow);
FTXUI_EXPORT(DOM) Decorator flex_shrink_factor(int shrink);

FTXUI_EXPORT(DOM)
Element xflex(Element);  // Expandir/Minimizar si es posible/necesario en el eje X.
FTXUI_EXPORT(DOM)
Element xflex_grow(Element);  // Expandir elemento si es posible en el eje X.
FTXUI_EXPORT(DOM)
Element xflex_shrink(Element);  // Minimizar elemento si es necesario en el eje X.
FTXUI_EXPORT(DOM) Decorator xflex_factor(int grow, int shrink);
FTXUI_EXPORT(DOM) Decorator xflex_grow_factor(int grow);
FTXUI_EXPORT(DOM) Decorator xflex_shrink_factor(int shrink);

FTXUI_EXPORT(DOM)
Element yflex(Element);  // Expandir/Minimizar si es posible/necesario en el eje Y.
FTXUI_EXPORT(DOM)
Element yflex_grow(Element);  // Expandir elemento si es posible en el eje Y.
FTXUI_EXPORT(DOM)
Element yflex_shrink(Element);  // Minimizar elemento si es necesario en el eje Y.
FTXUI_EXPORT(DOM) Decorator yflex_factor(int grow, int shrink);
FTXUI_EXPORT(DOM) Decorator yflex_grow_factor(int grow);
FTXUI_EXPORT(DOM) Decorator yflex_shrink_factor(int shrink);

FTXUI_EXPORT(DOM) Element notflex(Element);  // Restablecer el atributo flex.
FTXUI_EXPORT(DOM) Element filler();          // Un elemento expandible en blanco.

// -- Anulación de tamaño;
enum WidthOrHeight : uint8_t { WIDTH, HEIGHT };
enum Constraint : uint8_t { LESS_THAN, EQUAL, GREATER_THAN };
FTXUI_EXPORT(DOM) Decorator size(WidthOrHeight, Constraint, int value);

// --- Marco ---
// Un marco es un área desplazable. El área interna es potencialmente más grande que
// la externa. El área interna se desplaza para hacer visible el
// elemento enfocado.
FTXUI_EXPORT(DOM) Element frame(Element);
FTXUI_EXPORT(DOM) Element xframe(Element);
FTXUI_EXPORT(DOM) Element yframe(Element);
FTXUI_EXPORT(DOM) Element focus(Element);
FTXUI_EXPORT(DOM) Element select(Element e);  // Obsoleto - Alias para focus.

// --- Cursor ---
// Son similares a `focus`, pero también cambian la forma del cursor.
FTXUI_EXPORT(DOM) Element focusCursorBlock(Element);
FTXUI_EXPORT(DOM) Element focusCursorBlockBlinking(Element);
FTXUI_EXPORT(DOM) Element focusCursorBar(Element);
FTXUI_EXPORT(DOM) Element focusCursorBarBlinking(Element);
FTXUI_EXPORT(DOM) Element focusCursorUnderline(Element);
FTXUI_EXPORT(DOM) Element focusCursorUnderlineBlinking(Element);

// --- Varios ---
FTXUI_EXPORT(DOM) Element vscroll_indicator(Element);
FTXUI_EXPORT(DOM) Element hscroll_indicator(Element);
FTXUI_EXPORT(DOM) Decorator reflect(Box& box);
// Antes de dibujar el |element| limpiar el píxel de abajo. Esto es útil en
// combinación con dbox.
FTXUI_EXPORT(DOM) Element clear_under(Element element);

// --- Utilidad --------------------------------------------------------------------
FTXUI_EXPORT(DOM) Element hcenter(Element);
FTXUI_EXPORT(DOM) Element vcenter(Element);
FTXUI_EXPORT(DOM) Element center(Element);
FTXUI_EXPORT(DOM) Element align_right(Element);
FTXUI_EXPORT(DOM) Element nothing(Element element);

namespace Dimension {
FTXUI_EXPORT(DOM) Dimensions Fit(Element&, bool extend_beyond_screen = false);
}  // namespace Dimension

}  // namespace ftxui

// Hace que el contenedor pueda tomar cualquier número de hijos como entrada.
#include "ftxui/dom/take_any_args.hpp"

// Incluir definiciones antiguas usando wstring.
#include "ftxui/dom/deprecated.hpp"
#endif  // FTXUI_DOM_ELEMENTS_HPP
