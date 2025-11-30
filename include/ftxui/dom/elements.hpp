// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef FTXUI_DOM_ELEMENTS_HPP
#define FTXUI_DOM_ELEMENTS_HPP

#include <functional>
#include <memory>

#include "ftxui/dom/canvas.hpp"
#include "ftxui/dom/direction.hpp"
#include "ftxui/dom/flexbox_config.hpp"
#include "ftxui/dom/linear_gradient.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/box.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/screen/terminal.hpp"
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
enum BorderStyle {
  LIGHT,
  DASHED,
  HEAVY,
  DOUBLE,
  ROUNDED,
  EMPTY,
};

// Encadenar elementos en el decorador juntos.
// Por ejemplo, las siguientes líneas son equivalentes:
// -> text("ftxui") | bold | underlined
// -> underlined(bold(text("FTXUI")))
Element operator|(Element, Decorator);
Element& operator|=(Element&, Decorator);
Elements operator|(Elements, Decorator);
Decorator operator|(Decorator, Decorator);

// --- Elemento de Interfaz ---
Element text(std::string text);
Element vtext(std::string text);
Element separator();
Element separatorLight();
Element separatorDashed();
Element separatorHeavy();
Element separatorDouble();
Element separatorEmpty();
Element separatorStyled(BorderStyle);
Element separator(Pixel);
Element separatorCharacter(std::string);
Element separatorHSelector(float left,
                           float right,
                           Color unselected_color,
                           Color selected_color);
Element separatorVSelector(float up,
                           float down,
                           Color unselected_color,
                           Color selected_color);
Element gauge(float progress);
Element gaugeLeft(float progress);
Element gaugeRight(float progress);
Element gaugeUp(float progress);
Element gaugeDown(float progress);
Element gaugeDirection(float progress, Direction direction);
Element border(Element);
Element borderLight(Element);
Element borderDashed(Element);
Element borderHeavy(Element);
Element borderDouble(Element);
Element borderRounded(Element);
Element borderEmpty(Element);
Decorator borderStyled(BorderStyle);
Decorator borderStyled(BorderStyle, Color);
Decorator borderStyled(Color);
Decorator borderWith(const Pixel&);
Element window(Element title, Element content, BorderStyle border = ROUNDED);
Element spinner(int charset_index, size_t image_index);
Element paragraph(const std::string& text);
Element paragraphAlignLeft(const std::string& text);
Element paragraphAlignRight(const std::string& text);
Element paragraphAlignCenter(const std::string& text);
Element paragraphAlignJustify(const std::string& text);
Element graph(GraphFunction);
Element emptyElement();
Element canvas(ConstRef<Canvas>);
Element canvas(int width, int height, std::function<void(Canvas&)>);
Element canvas(std::function<void(Canvas&)>);

// -- Decorador ---
Element bold(Element);
Element dim(Element);
Element italic(Element);
Element inverted(Element);
Element underlined(Element);
Element underlinedDouble(Element);
Element blink(Element);
Element strikethrough(Element);
Decorator color(Color);
Decorator bgcolor(Color);
Decorator color(const LinearGradient&);
Decorator bgcolor(const LinearGradient&);
Element color(Color, Element);
Element bgcolor(Color, Element);
Element color(const LinearGradient&, Element);
Element bgcolor(const LinearGradient&, Element);
Decorator focusPosition(int x, int y);
Decorator focusPositionRelative(float x, float y);
Element automerge(Element child);
Decorator hyperlink(std::string link);
Element hyperlink(std::string link, Element child);
Element selectionStyleReset(Element);
Decorator selectionColor(Color foreground);
Decorator selectionBackgroundColor(Color foreground);
Decorator selectionForegroundColor(Color foreground);
Decorator selectionStyle(std::function<void(Pixel&)> style);

// --- Diseño ---
// Conjunto de elementos horizontales, verticales o apilados.
Element hbox(Elements);
Element vbox(Elements);
Element dbox(Elements);
Element flexbox(Elements, FlexboxConfig config = FlexboxConfig());
Element gridbox(std::vector<Elements> lines);

Element hflow(Elements);  // Helper: default flexbox with row direction.
Element vflow(Elements);  // Helper: default flexbox with column direction.

// -- Flexibilidad ---
// Define cómo compartir el espacio restante cuando no todo se utiliza dentro de un
// contenedor.
Element flex(Element);         // Expandir/Minimizar si es posible/necesario.
Element flex_grow(Element);    // Expandir elemento si es posible.
Element flex_shrink(Element);  // Minimizar elemento si es necesario.

Element xflex(Element);         // Expandir/Minimizar si es posible/necesario en el eje X.
Element xflex_grow(Element);    // Expandir elemento si es posible en el eje X.
Element xflex_shrink(Element);  // Minimizar elemento si es necesario en el eje X.

Element yflex(Element);         // Expandir/Minimizar si es posible/necesario en el eje Y.
Element yflex_grow(Element);    // Expandir elemento si es posible en el eje Y.
Element yflex_shrink(Element);  // Minimizar elemento si es necesario en el eje Y.

Element notflex(Element);  // Restablecer el atributo flex.
Element filler();          // Un elemento expandible en blanco.

// -- Anulación de tamaño;
enum WidthOrHeight { WIDTH, HEIGHT };
enum Constraint { LESS_THAN, EQUAL, GREATER_THAN };
Decorator size(WidthOrHeight, Constraint, int value);

// --- Marco ---
// Un marco es un área desplazable. El área interna es potencialmente más grande que
// la externa. El área interna se desplaza para hacer visible el
// elemento enfocado.
Element frame(Element);
Element xframe(Element);
Element yframe(Element);
Element focus(Element);
Element select(Element e);  // Obsoleto - Alias para focus.

// --- Cursor ---
// Son similares a `focus`, pero también cambian la forma del cursor.
Element focusCursorBlock(Element);
Element focusCursorBlockBlinking(Element);
Element focusCursorBar(Element);
Element focusCursorBarBlinking(Element);
Element focusCursorUnderline(Element);
Element focusCursorUnderlineBlinking(Element);

// --- Varios ---
Element vscroll_indicator(Element);
Element hscroll_indicator(Element);
Decorator reflect(Box& box);
// Antes de dibujar el |element|, borra el píxel de abajo. Esto es útil en
// combinación con dbox.
Element clear_under(Element element);

// --- Utilidad --------------------------------------------------------------------
Element hcenter(Element);
Element vcenter(Element);
Element center(Element);
Element align_right(Element);
Element nothing(Element element);

namespace Dimension {
Dimensions Fit(Element&, bool extend_beyond_screen = false);
}  // namespace Dimension

}  // namespace ftxui

// Hace que el contenedor pueda tomar cualquier número de hijos como entrada.
#include "ftxui/dom/take_any_args.hpp"

// Incluir definiciones antiguas usando wstring.
#include "ftxui/dom/deprecated.hpp"
#endif  // FTXUI_DOM_ELEMENTS_HPP